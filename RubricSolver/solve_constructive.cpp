#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <fstream>
#include <tuple>

using namespace std;


struct BoardState {
    int N;
    vector<vector<int>> grid;
    int emptyRow, emptyCol; 
    string commandHistory; 
    
    vector<vector<bool>> locked; 
};

// physically execute a command string on each state
void executeCommands(BoardState& state, const string& commands) {
    for (char cmd : commands) {
        int r = state.emptyRow;
        int c = state.emptyCol;
        
        if (cmd == 'U') { 
            swap(state.grid[r][c], state.grid[r + 1][c]); 
            state.emptyRow++; 
        } else if (cmd == 'D') { 
            swap(state.grid[r][c], state.grid[r - 1][c]); 
            state.emptyRow--; 
        } else if (cmd == 'L') { 
            swap(state.grid[r][c], state.grid[r][c + 1]); 
            state.emptyCol++; 
        } else if (cmd == 'R') { 
            swap(state.grid[r][c], state.grid[r][c - 1]); 
            state.emptyCol--; 
        }
        state.commandHistory += cmd;
    }
}


// navigate empty space to target pos while avoiding locked grid
string routeEmptySpace(const BoardState& state, int targetRow, int targetCol) {
    int N = state.N;
    if (state.emptyRow == targetRow && state.emptyCol == targetCol) return "";

    queue<pair<int, int>> q;
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    
    // Arrays to track our path without copying strings
    vector<vector<pair<int, int>>> parent(N, vector<pair<int, int>>(N, {-1, -1}));
    vector<vector<char>> moveMade(N, vector<char>(N, ' '));

    q.push({state.emptyRow, state.emptyCol});
    visited[state.emptyRow][state.emptyCol] = true;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char cmd[] = {'D', 'U', 'R', 'L'}; 
    bool found = false;

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        if (r == targetRow && c == targetCol) {
            found = true;
            break;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                if (!visited[nr][nc] && !state.locked[nr][nc]) {
                    visited[nr][nc] = true;
                    parent[nr][nc] = {r, c};
                    moveMade[nr][nc] = cmd[i];
                    q.push({nr, nc});
                }
            }
        }
    }

    if (!found) return "";

    // trace backwards to build the final string
    string path = "";
    int currR = targetRow;
    int currC = targetCol;
    while (currR != state.emptyRow || currC != state.emptyCol) {
        path += moveMade[currR][currC];
        int nextR = parent[currR][currC].first;
        int nextC = parent[currR][currC].second;
        currR = nextR;
        currC = nextC;
    }
    
    reverse(path.begin(), path.end());
    return path;
}

// generate optimal tile pathing to destination (dijkstra w/ cost)
string getTilePath(const BoardState& state, int startR, int startC, int destR, int destC) {
    int N = state.N;
    if (startR == destR && startC == destC) return "";

    // Tuple: {total_cost, r, c, incoming_direction}
    // incoming_direction: 0=U, 1=D, 2=L, 3=R, 4=Start
    priority_queue<tuple<int, int, int, int>, vector<tuple<int, int, int, int>>, greater<tuple<int, int, int, int>>> pq;
    
    // dist[r][c][dir]
    vector<vector<vector<int>>> dist(N, vector<vector<int>>(N, vector<int>(5, 999999)));
    
    struct ParentNode { 
        int r, c, dir; 
        char cmd; 
    };

    // parent[nr][nc][dir]
    vector<vector<vector<ParentNode>>> parent(N, vector<vector<ParentNode>>(N, vector<ParentNode>(5)));

    pq.push({0, startR, startC, 4});
    dist[startR][startC][4] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char cmd[] = {'U', 'D', 'L', 'R'};

    int bestFinalCost = 999999;
    int finalDir = -1;

    while (!pq.empty()) {
        auto [cost, r, c, inDir] = pq.top();
        pq.pop();

        if (cost > dist[r][c][inDir]) continue;

        if (r == destR && c == destC) {
            if (cost < bestFinalCost) {
                bestFinalCost = cost;
                finalDir = inDir;
            }
            continue;
        }

        for (int i = 0; i < 4; ++i) {
            int nr = r + dr[i];
            int nc = c + dc[i];

            if (nr >= 0 && nr < N && nc >= 0 && nc < N && !state.locked[nr][nc]) {
                int stepCost = 0;
                
                if (inDir == 4) {
                    stepCost = 1; // first move
                } else if (inDir == i) {
                    stepCost = 5; // empty space must wrap around
                } else if ((inDir == 0 && i == 1) || (inDir == 1 && i == 0) || 
                           (inDir == 2 && i == 3) || (inDir == 3 && i == 2)) {
                    stepCost = 10; // reversal penalty
                } else {
                    stepCost = 3; // 90-degree turn (Staircasing). preferable
                }

                int nextCost = cost + stepCost;

                if (nextCost < dist[nr][nc][i]) {
                    dist[nr][nc][i] = nextCost;
                    parent[nr][nc][i] = {r, c, inDir, cmd[i]};
                    pq.push({nextCost, nr, nc, i});
                }
            }
        }
    }

    if (finalDir == -1) return "";

    string path = "";
    int currR = destR, currC = destC, currDir = finalDir;

    // trace back the optimal path
    while (currR != startR || currC != startC) {
        ParentNode p = parent[currR][currC][currDir];
        path += p.cmd;
        currR = p.r;
        currC = p.c;
        currDir = p.dir;
    }

    reverse(path.begin(), path.end());
    return path;
}

// relocates empty by routeEmptySpace so that the tile can move accordingly to the getTilePath
void moveTile(BoardState& state, int startR, int startC, int destR, int destC) {
    // get safe path that avoids locked tiles
    string tilePath = getTilePath(state, startR, startC, destR, destC);
    
    int currR = startR;
    int currC = startC;

    for (char moveCmd : tilePath) {
        int emptyTargetR = currR;
        int emptyTargetC = currC;

        if (moveCmd == 'U') emptyTargetR = currR - 1;
        else if (moveCmd == 'D') emptyTargetR = currR + 1;
        else if (moveCmd == 'L') emptyTargetC = currC - 1;
        else if (moveCmd == 'R') emptyTargetC = currC + 1;

        state.locked[currR][currC] = true;
        string route = routeEmptySpace(state, emptyTargetR, emptyTargetC);
        executeCommands(state, route);
        state.locked[currR][currC] = false;

        string pushStr = "";
        pushStr += moveCmd;
        executeCommands(state, pushStr);

        if (moveCmd == 'U') currR--;
        else if (moveCmd == 'D') currR++;
        else if (moveCmd == 'L') currC--;
        else if (moveCmd == 'R') currC++;
    }
}

bool isAlreadyInPlace(int r, int c, int val, int N, const vector<vector<int>>& targetPattern) {
    // check if in zone
    if (r >= 1 && r < N - 1 && c >= 1 && c < N - 1) {
        return targetPattern[r - 1][c - 1] == val;
    }
    return false;
}

// constructive solver
string solvePuzzle(BoardState& state, const vector<vector<int>>& targetPattern) {
    int N = state.N;
    int targetSize = N - 2;

    // initialize the locked grid to all false
    state.locked.assign(N, vector<bool>(N, false));

    // iterate through every tile in target pattern
    for (int tr = 0; tr < targetSize; ++tr) {
        for (int tc = 0; tc < targetSize; ++tc) {
            int targetVal = targetPattern[tr][tc];

            // skip empty space
            if (targetVal == -1) continue;

            // offset for target space
            int destR = tr + 1; 
            int destC = tc + 1;

            // lock tiles if located at target pattern
            if (isAlreadyInPlace(destR, destC, state.grid[destR][destC], N, targetPattern)) {
                state.locked[destR][destC] = true;
                continue; 
            }

            // scan the board for 'best' unlocked tile (avoiding locked walls)
            vector<vector<int>> trueDist(N, vector<int>(N, 999999));
            queue<pair<int, int>> dq;
            
            dq.push({destR, destC});
            trueDist[destR][destC] = 0;
            
            int dr[] = {-1, 1, 0, 0};
            int dc[] = {0, 0, -1, 1};

            while (!dq.empty()) {
                int r = dq.front().first;
                int c = dq.front().second;
                dq.pop();

                for (int i = 0; i < 4; ++i) {
                    int nr = r + dr[i];
                    int nc = c + dc[i];
                    if (nr >= 0 && nr < N && nc >= 0 && nc < N) {
                        // flood only unlocked spaces
                        if (!state.locked[nr][nc] && trueDist[nr][nc] == 999999) {
                            trueDist[nr][nc] = trueDist[r][c] + 1;
                            dq.push({nr, nc});
                        }
                    }
                }
            }

            // find best source (start) to move to destination
            int startR = -1, startC = -1;
            int bestScore = 9999999;
            
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < N; ++c) {
                    if (state.grid[r][c] == targetVal && !state.locked[r][c] && trueDist[r][c] != 999999) {
                        
                        // base score
                        int score = trueDist[r][c]; 
                        
                        // don't take tile that already in place, but not yet to lock
                        if (isAlreadyInPlace(r, c, targetVal, N, targetPattern)) {
                            score += 10000;
                        }

                        if (score < bestScore) {
                            bestScore = score;
                            startR = r;
                            startC = c;
                        }
                    }
                }
            }

            // move the tile to its destination, then lock it
            moveTile(state, startR, startC, destR, destC);
            state.locked[destR][destC] = true;
        }
    }

    // all target tiles are locked in place.
    return state.commandHistory + "S";
}

// Post-processing optimizer to remove redundant opposing moves
string optimizeCommands(string cmds) {
    bool changed = true;
    while (changed) {
        changed = false;
        string nextCmds = "";
        
        for (char curr : cmds) {
            if (!nextCmds.empty()) {
                char last = nextCmds.back();
                // if the current move exactly undoes the last move, delete both
                if ((last == 'U' && curr == 'D') || (last == 'D' && curr == 'U') ||
                    (last == 'L' && curr == 'R') || (last == 'R' && curr == 'L')) {
                    nextCmds.pop_back(); // Remove the previous move
                    changed = true;      // Flag that we found a reduction
                    continue;            // Skip adding the current move
                }
            }
            nextCmds += curr;
        }
        cmds = nextCmds;
    }
    return cmds;
}

int main() {
    // loop through testcases 1 to 6
    for (int t = 2; t <= 6; ++t) {
        // ===== EXTRACTION =====
        string inFileName = "testdata/" + to_string(t) + ".in";
        string outFileName = "solution_" + to_string(t) + ".txt";

        ifstream inFile(inFileName);

        cout << "Processing " << inFileName << "..." << endl;

        BoardState startState;
        
        // read grid size
        inFile >> startState.N;
        int N = startState.N;
        
        // read the starting grid
        startState.grid.resize(N, vector<int>(N));
        for (int r = 0; r < N; ++r) {
            for (int c = 0; c < N; ++c) {
                inFile >> startState.grid[r][c];
                
                if (startState.grid[r][c] == -1) {
                    startState.emptyRow = r;
                    startState.emptyCol = c;
                }
            }
        }

        // read the target pattern
        int targetSize = N - 2;
        vector<vector<int>> targetPattern(targetSize, vector<int>(targetSize));
        for (int r = 0; r < targetSize; ++r) {
            for (int c = 0; c < targetSize; ++c) {
                inFile >> targetPattern[r][c];
            }
        }
        
        inFile.close();


        // ===== SOLVER =====
        // run the solver
        string solution = solvePuzzle(startState, targetPattern);
        
        // catch error
        if (solution.rfind("Error", 0) == 0) { 
            cerr << "Failed on Testcase " << t << ": " << solution << endl;
        } else {
            // run post-processed optimization
            string optimizedSolution = optimizeCommands(solution.substr(0, solution.length() - 1)) + "S";

            // saves to log file
            ofstream outFile(outFileName);
            
            if (outFile.is_open()) {
                outFile << optimizedSolution;
                outFile.close();     
                
                cout << "  -> Success! Written to '" << outFileName << "'." << endl;
                cout << "  -> Total Commands: " << optimizedSolution.length() - 1 << endl;
            } else {
                cerr << "  -> Error: Unable to create '" << outFileName << "' for writing." << endl;
            }
        }
        cout << string(40, '-') << endl;
    }

    return 0;
}