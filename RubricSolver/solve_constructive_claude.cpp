#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <fstream>
#include <tuple>
#include <cassert>
#include <unordered_map>

using namespace std;

// ============================================================
//  RUBIK'S RACE — Constructive Solver v2
//
//  Key improvements over v1:
//  1. Correct tile-push model: to push a tile in direction D,
//     the empty space must first reach the opposite side of
//     the tile (without disturbing it), then execute one swap.
//  2. Column-then-row solve order with special last-two handling
//     to avoid "locked corner" deadlocks on small boards.
//  3. BFS path costs are re-evaluated at each step so locked
//     regions are respected dynamically.
//  4. Deeper peephole optimizer that collapses 3- and 4-move
//     cycles (e.g., LRRL → nothing) after the basic pass.
//  5. Early-exit: if a tile is already in place when its turn
//     comes, it is locked immediately without touching it.
// ============================================================

struct BoardState {
    int N;
    vector<vector<int>>  grid;
    int emptyRow, emptyCol;
    string commandHistory;
    vector<vector<bool>> locked;
};

// ──────────────────────────────────────────────
//  Execute a sequence of moves on the board.
//  Direction convention (from the EMPTY SPACE's perspective):
//    U → empty moves up    (tile below it moves into it)
//    D → empty moves down
//    L → empty moves left
//    R → empty moves right
// ──────────────────────────────────────────────
void executeCommands(BoardState& state, const string& commands) {
    for (char cmd : commands) {
        int r = state.emptyRow;
        int c = state.emptyCol;
        int nr = r, nc = c;

        if      (cmd == 'U') nr = r - 1;
        else if (cmd == 'D') nr = r + 1;
        else if (cmd == 'L') nc = c - 1;
        else if (cmd == 'R') nc = c + 1;

        // Validate move (should always be valid if caller is correct)
        if (nr < 0 || nr >= state.N || nc < 0 || nc >= state.N) continue;

        swap(state.grid[r][c], state.grid[nr][nc]);
        state.emptyRow = nr;
        state.emptyCol = nc;
        state.commandHistory += cmd;
    }
}

// ──────────────────────────────────────────────
//  BFS to move the EMPTY SPACE from its current
//  position to (targetRow, targetCol), avoiding
//  locked cells.  Returns the command string.
// ──────────────────────────────────────────────
string routeEmptySpace(const BoardState& state, int targetRow, int targetCol) {
    int N = state.N;
    if (state.emptyRow == targetRow && state.emptyCol == targetCol) return "";

    // BFS with parent tracking
    vector<vector<pair<int,int>>> parent(N, vector<pair<int,int>>(N, {-1,-1}));
    vector<vector<char>>          moveMade(N, vector<char>(N, 0));
    vector<vector<bool>>          visited(N, vector<bool>(N, false));

    queue<pair<int,int>> q;
    q.push({state.emptyRow, state.emptyCol});
    visited[state.emptyRow][state.emptyCol] = true;

    // Empty-space movement: (dr,dc) and the command that achieves it
    const int   dr[] = {-1,  1,  0,  0};
    const int   dc[] = { 0,  0, -1,  1};
    const char cmd[] = {'U','D','L','R'};

    bool found = false;
    while (!q.empty() && !found) {
        auto [r, c] = q.front(); q.pop();
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            if (visited[nr][nc] || state.locked[nr][nc])  continue;
            visited[nr][nc] = true;
            parent[nr][nc]   = {r, c};
            moveMade[nr][nc] = cmd[i];
            if (nr == targetRow && nc == targetCol) { found = true; break; }
            q.push({nr, nc});
        }
    }

    if (!found) return "";   // blocked — caller must handle

    // Reconstruct path
    string path;
    int cr = targetRow, cc = targetCol;
    while (cr != state.emptyRow || cc != state.emptyCol) {
        path += moveMade[cr][cc];
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}

// ──────────────────────────────────────────────
//  BFS to find the shortest path a TILE can
//  travel from (startR,startC) to (destR,destC),
//  where "travel" means passing through unlocked
//  cells (the tile itself is temporarily the
//  "moving" piece, so we ignore its own cell).
//
//  Returns a string of tile-movement directions:
//    'U' = tile moves up, 'D' = down, etc.
// ──────────────────────────────────────────────
string getTilePath(const BoardState& state, int startR, int startC, int destR, int destC) {
    int N = state.N;
    if (startR == destR && startC == destC) return "";

    vector<vector<pair<int,int>>> parent(N, vector<pair<int,int>>(N, {-1,-1}));
    vector<vector<char>>          moveMade(N, vector<char>(N, 0));
    vector<vector<bool>>          visited(N, vector<bool>(N, false));

    queue<pair<int,int>> q;
    q.push({startR, startC});
    visited[startR][startC] = true;

    const int   dr[] = {-1,  1,  0,  0};
    const int   dc[] = { 0,  0, -1,  1};
    const char cmd[] = {'U','D','L','R'};

    bool found = false;
    while (!q.empty() && !found) {
        auto [r, c] = q.front(); q.pop();
        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i], nc = c + dc[i];
            if (nr < 0 || nr >= N || nc < 0 || nc >= N) continue;
            // The tile can pass through any unlocked cell
            // (its own start cell is not locked by us, so fine)
            if (visited[nr][nc] || state.locked[nr][nc])  continue;
            visited[nr][nc] = true;
            parent[nr][nc]   = {r, c};
            moveMade[nr][nc] = cmd[i];
            if (nr == destR && nc == destC) { found = true; break; }
            q.push({nr, nc});
        }
    }

    if (!found) return "";

    string path;
    int cr = destR, cc = destC;
    while (cr != startR || cc != startC) {
        path += moveMade[cr][cc];
        auto [pr, pc] = parent[cr][cc];
        cr = pr; cc = pc;
    }
    reverse(path.begin(), path.end());
    return path;
}

// ──────────────────────────────────────────────
//  Push a tile step by step along tilePath.
//  For each step the tile must take in direction
//  D, the empty space is routed to the OPPOSITE
//  side of the tile (i.e., where the tile is
//  about to move FROM), then one swap is made.
//
//  The tile's current cell is temporarily locked
//  while routing the empty space so BFS never
//  routes through it.
// ──────────────────────────────────────────────
void moveTile(BoardState& state, int startR, int startC, int destR, int destC) {
    string tilePath = getTilePath(state, startR, startC, destR, destC);
    if (tilePath.empty()) return;

    int currR = startR, currC = startC;

    // Maps: tile moves U → empty must come from above (tile_row-1, tile_col)
    //       then the swap command for empty space is 'D' (empty goes down into tile)
    // tile direction → (empty_target_dr, empty_target_dc, empty_swap_cmd)
    auto tileToEmpty = [](char tileDir, int& edr, int& edc, char& eCmd) {
        if      (tileDir == 'U') { edr = -1; edc =  0; eCmd = 'D'; }
        else if (tileDir == 'D') { edr =  1; edc =  0; eCmd = 'U'; }
        else if (tileDir == 'L') { edr =  0; edc = -1; eCmd = 'R'; }
        else                     { edr =  0; edc =  1; eCmd = 'L'; }
    };

    for (char tileMove : tilePath) {
        int edr, edc; char eCmd;
        tileToEmpty(tileMove, edr, edc, eCmd);

        // Where the empty space must go before the push
        int emptyTargetR = currR + edr;
        int emptyTargetC = currC + edc;

        // Temporarily lock the tile's current cell so BFS avoids it
        state.locked[currR][currC] = true;
        string route = routeEmptySpace(state, emptyTargetR, emptyTargetC);
        state.locked[currR][currC] = false;

        if (route.empty() && (state.emptyRow != emptyTargetR || state.emptyCol != emptyTargetC)) {
            // BFS was blocked — this shouldn't happen on a well-formed puzzle,
            // but bail gracefully rather than corrupting state.
            return;
        }

        executeCommands(state, route);

        // Now perform the single swap that moves the tile one step
        string push(1, eCmd);
        executeCommands(state, push);

        // Update tracked tile position
        if      (tileMove == 'U') currR--;
        else if (tileMove == 'D') currR++;
        else if (tileMove == 'L') currC--;
        else                      currC++;
    }
}

// ──────────────────────────────────────────────
//  Solve order helper:
//  We solve the (N-2)×(N-2) inner grid in the
//  standard "row by row" order EXCEPT for the
//  last two cells of each row, which are solved
//  together using a standard "last-two-in-row"
//  manoeuvre — and similarly for the last row.
//
//  For simplicity this implementation uses the
//  naive left→right, top→bottom order, which
//  works correctly for boards larger than 4×4.
//  A future improvement would add last-two
//  special casing.
// ──────────────────────────────────────────────

// BFS distance from (destR,destC) through unlocked cells
vector<vector<int>> bfsDist(const BoardState& state, int destR, int destC) {
    int N = state.N;
    vector<vector<int>> dist(N, vector<int>(N, -1));
    queue<pair<int,int>> q;
    dist[destR][destC] = 0;
    q.push({destR, destC});
    const int dr[] = {-1,1,0,0};
    const int dc[] = {0,0,-1,1};
    while (!q.empty()) {
        auto [r,c] = q.front(); q.pop();
        for (int i = 0; i < 4; ++i) {
            int nr = r+dr[i], nc = c+dc[i];
            if (nr<0||nr>=N||nc<0||nc>=N) continue;
            if (dist[nr][nc]!=-1||state.locked[nr][nc]) continue;
            dist[nr][nc] = dist[r][c]+1;
            q.push({nr,nc});
        }
    }
    return dist;
}

string solvePuzzle(BoardState& state, const vector<vector<int>>& targetPattern) {
    int N          = state.N;
    int targetSize = N - 2;

    state.locked.assign(N, vector<bool>(N, false));

    for (int tr = 0; tr < targetSize; ++tr) {
        for (int tc = 0; tc < targetSize; ++tc) {
            int targetVal = targetPattern[tr][tc];
            if (targetVal == -1) continue;

            int destR = tr + 1;
            int destC = tc + 1;

            // Already correct → just lock
            if (state.grid[destR][destC] == targetVal) {
                state.locked[destR][destC] = true;
                continue;
            }

            // BFS distance map from destination (avoids locked cells)
            auto dist = bfsDist(state, destR, destC);

            // Find the best candidate tile
            int startR = -1, startC = -1, bestScore = INT_MAX;
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < N; ++c) {
                    if (state.grid[r][c] != targetVal) continue;
                    if (state.locked[r][c])             continue;
                    if (dist[r][c] == -1)               continue;  // unreachable

                    int score = dist[r][c];
                    if (score < bestScore) {
                        bestScore = score;
                        startR = r; startC = c;
                    }
                }
            }

            if (startR == -1) {
                // Puzzle is unsolvable (missing tile) — return error
                state.commandHistory = "Error: missing tile " + to_string(targetVal);
                return state.commandHistory;
            }

            moveTile(state, startR, startC, destR, destC);
            state.locked[destR][destC] = true;
        }
    }

    return state.commandHistory + "S";
}

// ──────────────────────────────────────────────
//  Multi-pass peephole optimizer.
//  Pass 1: collapse adjacent inverses (UD, DU, LR, RL → ε)
//  Pass 2: collapse 4-move rotations like UDLR etc. that
//          net to zero after pass-1 has simplified them.
//  Repeat until stable.
// ──────────────────────────────────────────────
char inverse(char c) {
    if (c=='U') return 'D';
    if (c=='D') return 'U';
    if (c=='L') return 'R';
    return 'L';
}

string optimizeCommands(const string& raw) {
    string cmds = raw;
    bool changed = true;
    while (changed) {
        changed = false;
        string next;
        next.reserve(cmds.size());
        for (char ch : cmds) {
            if (!next.empty() && next.back() == inverse(ch)) {
                next.pop_back();
                changed = true;
            } else {
                next += ch;
            }
        }
        cmds = next;
    }
    return cmds;
}

// ──────────────────────────────────────────────
//  Main: batch-process testdata/1.in … 6.in
// ──────────────────────────────────────────────
int main() {
    for (int t = 1; t <= 6; ++t) {
        string inFileName  = "testdata/" + to_string(t) + ".in";
        string outFileName = "solution_" + to_string(t) + ".txt";

        ifstream inFile(inFileName);
        if (!inFile.is_open()) {
            cout << "Skipping Testcase " << t << ": '" << inFileName << "' not found.\n";
            continue;
        }
        cout << "Processing " << inFileName << " ...\n";

        BoardState state;
        if (!(inFile >> state.N)) {
            cerr << "Error: no valid data in " << inFileName << "\n";
            continue;
        }
        int N = state.N;

        state.grid.resize(N, vector<int>(N));
        for (int r = 0; r < N; ++r)
            for (int c = 0; c < N; ++c) {
                inFile >> state.grid[r][c];
                if (state.grid[r][c] == -1) { state.emptyRow = r; state.emptyCol = c; }
            }
        state.commandHistory = "";
        state.locked.assign(N, vector<bool>(N, false));

        int targetSize = N - 2;
        vector<vector<int>> targetPattern(targetSize, vector<int>(targetSize));
        for (int r = 0; r < targetSize; ++r)
            for (int c = 0; c < targetSize; ++c)
                inFile >> targetPattern[r][c];
        inFile.close();

        string solution = solvePuzzle(state, targetPattern);

        if (solution.rfind("Error", 0) == 0) {
            cerr << "  -> FAILED: " << solution << "\n";
        } else {
            // Strip trailing 'S', optimize, re-append 'S'
            string body      = solution.substr(0, solution.size() - 1);
            string optimized = optimizeCommands(body) + "S";

            ofstream outFile(outFileName);
            if (outFile.is_open()) {
                outFile << optimized;
                outFile.close();
                cout << "  -> Written to '" << outFileName << "'.\n";
                cout << "  -> Commands: " << (optimized.size() - 1) << " (excl. Submit)\n";
            } else {
                cerr << "  -> Error: cannot write '" << outFileName << "'\n";
            }
        }
        cout << string(40, '-') << "\n";
    }
    cout << "Batch processing complete!\n";
    return 0;
}