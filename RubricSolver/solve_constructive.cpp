#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <fstream>
#include <tuple>

using namespace std;

// Medium Table ()

// Updated struct for constructive solving
struct BoardState {
    int N;
    vector<vector<int>> grid;
    int emptyRow, emptyCol; 
    string commandHistory; 
    
    // NEW: Tracks tiles we have solved so the router avoids them
    vector<vector<bool>> locked; 
};

// Function to physically execute a command string on our single board
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

// 1. OPTIMIZED EMPTY SPACE ROUTER
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

    // Trace backwards to build the final string
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

// 2. OPTIMIZED TILE PATHFINDER (Prevents crashing into locked tiles)
string getTilePath(const BoardState& state, int startR, int startC, int destR, int destC) {
    int N = state.N;
    if (startR == destR && startC == destC) return "";

    queue<pair<int, int>> q;
    vector<vector<bool>> visited(N, vector<bool>(N, false));
    vector<vector<pair<int, int>>> parent(N, vector<pair<int, int>>(N, {-1, -1}));
    vector<vector<char>> moveMade(N, vector<char>(N, ' '));

    q.push({startR, startC});
    visited[startR][startC] = true;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};
    char cmd[] = {'U', 'D', 'L', 'R'};
    bool found = false;

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        if (r == destR && c == destC) {
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

    string path = "";
    int currR = destR;
    int currC = destC;
    while (currR != startR || currC != startC) {
        path += moveMade[currR][currC];
        int nextR = parent[currR][currC].first;
        int nextC = parent[currR][currC].second;
        currR = nextR;
        currC = nextC;
    }
    
    reverse(path.begin(), path.end());
    return path;
}

// 3. UPDATED TILE PUSHER
void moveTile(BoardState& state, int startR, int startC, int destR, int destC) {
    // Get the safe path that avoids locked tiles
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
    // If the tile is inside the target zone
    if (r >= 1 && r < N - 1 && c >= 1 && c < N - 1) {
        // Return true if it already perfectly matches the target requirement
        return targetPattern[r - 1][c - 1] == val;
    }
    return false;
}


// The Constructive Master Solver
string solvePuzzle(BoardState& state, const vector<vector<int>>& targetPattern) {
    int N = state.N;
    int targetSize = N - 2;

    // 1. Initialize the locked grid to all false
    state.locked.assign(N, vector<bool>(N, false));

    // 2. Iterate through every space in the target pattern
    for (int tr = 0; tr < targetSize; ++tr) {
        for (int tc = 0; tc < targetSize; ++tc) {
            int targetVal = targetPattern[tr][tc];

            // If we don't care about this spot (e.g., it's a -1), skip it
            if (targetVal == -1) continue;

            // Offset by 1 because the target area starts at grid[1][1]
            int destR = tr + 1; 
            int destC = tc + 1;

            // If the correct tile happens to already be there, just lock it!
            if (state.grid[destR][destC] == targetVal) {
                state.locked[destR][destC] = true;
                continue; 
            }

            // 3. Scan the board for the BEST UNLOCKED tile
            // First, run BFS from destination to find TRUE path distances (avoiding locked walls)
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
                        // Spread distance only through unlocked spaces
                        if (!state.locked[nr][nc] && trueDist[nr][nc] == 999999) {
                            trueDist[nr][nc] = trueDist[r][c] + 1;
                            dq.push({nr, nc});
                        }
                    }
                }
            }

            int startR = -1, startC = -1;
            int bestScore = 9999999;
            bool found = false;
            
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < N; ++c) {
                    if (state.grid[r][c] == targetVal && !state.locked[r][c] && trueDist[r][c] != 999999) {
                        
                        // Base score is the TRUE step distance
                        int score = trueDist[r][c]; 
                        
                        // MASSIVE PENALTY: Don't steal a tile that is already perfectly placed for the future!
                        if (isAlreadyInPlace(r, c, targetVal, N, targetPattern)) {
                            score += 10000;
                        }

                        if (score < bestScore) {
                            bestScore = score;
                            startR = r;
                            startC = c;
                            found = true;
                        }
                    }
                }
            }

            // 4. Move the tile to its destination, then lock it down
            if (found) {
                moveTile(state, startR, startC, destR, destC);
                state.locked[destR][destC] = true;
            } else {
                return "Error: The board is missing a required tile!";
            }
        }
    }

    // Once the loops finish, all target tiles are locked in place. We win!
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
                // If the current move exactly undoes the last move, delete both!
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
    // Loop through testcases 1 to 6
    for (int t = 1; t <= 6; ++t) {
        string inFileName = "testdata/" + to_string(t) + ".in";
        string outFileName = "solution_" + to_string(t) + ".txt";

        ifstream inFile(inFileName);
        
        // Check if the input file actually exists
        if (!inFile.is_open()) {
            cout << "Skipping Testcase " << t << ": '" << inFileName << "' not found." << endl;
            continue; 
        }

        cout << "Processing " << inFileName << "..." << endl;

        BoardState startState;
        
        // 1. Read N (grid size) from the file
        if (!(inFile >> startState.N)) {
            cerr << "Error: No valid data in " << inFileName << endl;
            inFile.close();
            continue; 
        }
        int N = startState.N;
        
        // 2. Read the starting grid from the file
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
        startState.commandHistory = "";
        // Initialize the locked grid to entirely false
        startState.locked.assign(N, vector<bool>(N, false));

        // 3. Read the target pattern from the file
        int targetSize = N - 2;
        vector<vector<int>> targetPattern(targetSize, vector<int>(targetSize));
        
        for (int r = 0; r < targetSize; ++r) {
            for (int c = 0; c < targetSize; ++c) {
                inFile >> targetPattern[r][c];
            }
        }
        
        inFile.close(); // Done reading this file

        // 4. Run the solver
        string solution = solvePuzzle(startState, targetPattern);
        
        // 5. Optimize and Output the result directly to the text file
        if (solution.rfind("Error", 0) == 0) { 
            cerr << "Failed on Testcase " << t << ": " << solution << endl;
        } else {
            // Strip the 'S', run the peephole optimizer, and add 'S' back
            string optimizedSolution = optimizeCommands(solution.substr(0, solution.length() - 1)) + "S";

            ofstream outFile(outFileName);
            
            if (outFile.is_open()) {
                outFile << optimizedSolution;
                outFile.close();     
                
                cout << "  -> Success! Written to '" << outFileName << "'." << endl;
                cout << "  -> Total Commands: " << optimizedSolution.length() - 1 << " (excluding Submit)" << endl;
            } else {
                cerr << "  -> Error: Unable to create '" << outFileName << "' for writing." << endl;
            }
        }
        cout << string(40, '-') << endl; // Visual separator
    }

    cout << "Batch processing complete!" << endl;
    return 0;
}