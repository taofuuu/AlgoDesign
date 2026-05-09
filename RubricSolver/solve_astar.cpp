#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <set>
#include <cmath>
#include <climits>

using namespace std;

// snapshot of the puzzle board
struct BoardState {
    int N;
    vector<vector<int>> grid;
    int emptyRow, emptyCol;
    string commandHistory; // sequence of commands to get here

    // A* implementation
    int g_cost = 0; // moves taken
    int h_cost = 0; // estimated moves to target

    // pq sort: priority lower cost first
    bool operator>(const BoardState& other) const {
        return (g_cost + h_cost) > (other.g_cost + other.h_cost);
    }

    void printGrid() {
        for (int i = 0; i < N; ++i) cout << "- ";
        cout << endl;

        for (auto &x : grid) {
            for (auto &y : x) {
                cout << y << " ";
            }
            cout << endl;
        }
    }

};

// checks if the board matched the target
bool isTargetAchieved(const BoardState& current, const vector<vector<int>>& targetPattern) {
    int N = current.N;
    
    // Loop through the inner grid. 
    for (int r = 1; r < N - 1; ++r) {
        for (int c = 1; c < N - 1; ++c) {
            if (current.grid[r][c] != targetPattern[r - 1][c - 1]) {
                return false; 
            }
        }
    }
    return true;
}

// generates all valid next move from current board
vector<BoardState> getValidMoves(const BoardState& current) {
    vector<BoardState> nextStates;
    int r = current.emptyRow;
    int c = current.emptyCol;
    int N = current.N;

    // U: empty moves down
    if (r + 1 < N) {
        BoardState nextState = current;
        swap(nextState.grid[r][c], nextState.grid[r + 1][c]);
        nextState.emptyRow = r + 1;
        nextState.commandHistory += 'U';
        nextStates.push_back(nextState);
    }

    // D: empty moves up
    if (r - 1 >= 0) {
        BoardState nextState = current;
        swap(nextState.grid[r][c], nextState.grid[r - 1][c]);
        nextState.emptyRow = r - 1;
        nextState.commandHistory += 'D';
        nextStates.push_back(nextState);
    }

    // L: empty moves right
    if (c + 1 < N) {
        BoardState nextState = current;
        swap(nextState.grid[r][c], nextState.grid[r][c + 1]);
        nextState.emptyCol = c + 1;
        nextState.commandHistory += 'L';
        nextStates.push_back(nextState);
    }

    // R: empty moves left
    if (c - 1 >= 0) {
        BoardState nextState = current;
        swap(nextState.grid[r][c], nextState.grid[r][c - 1]);
        nextState.emptyCol = c - 1;
        nextState.commandHistory += 'R';
        nextStates.push_back(nextState);
    }

    return nextStates;
}

// calculates Manhattan distance heuristic
int calculateHeuristic(const BoardState& state, const vector<vector<int>>& targetPattern) {
    int h = 0;
    int N = state.N;
    int targetSize = N - 2;

    // loop every tile in target
    for (int tr = 0; tr < targetSize; ++tr) {
        for (int tc = 0; tc < targetSize; ++tc) {
            int targetVal = targetPattern[tr][tc];
            
            // skip empty space
            if (targetVal == -1) continue; 

            int minDist = INT_MAX;

            // find the closest matching tile on the board
            for (int r = 0; r < N; ++r) {
                for (int c = 0; c < N; ++c) {
                    if (state.grid[r][c] == targetVal) {
                        // offset tr and tc by +1
                        int dist = abs(r - (tr + 1)) + abs(c - (tc + 1));
                        if (dist < minDist) {
                            minDist = dist;
                        }
                    }
                }
            }
            h += minDist; // add to total heuristic score
        }
    }
    return h;
}

// A*
string solvePuzzle(const BoardState& startState, const vector<vector<int>>& targetPattern) {
    // evaluate most promising paths first
    priority_queue<BoardState, vector<BoardState>, greater<BoardState>> pq;
    
    set<vector<vector<int>>> visited;

    // Initialize start state
    BoardState initial = startState;
    initial.h_cost = calculateHeuristic(initial, targetPattern);
    pq.push(initial);
    visited.insert(initial.grid);

    int maxDepth = -1;
    int statesEvaluated = 0;

    while (!pq.empty()) {
        BoardState current = pq.top();
        pq.pop();
        statesEvaluated++;

        // --- PROGRESS TRACKER ---
        if (current.g_cost > maxDepth) {
            maxDepth = current.g_cost;
            cerr << "Exploring move depth: " << maxDepth 
                 << " | Best f(n) score: " << current.g_cost + current.h_cost
                 << " | States evaluated: " << statesEvaluated 
                 << " | Queue size: " << pq.size() << endl;
        }

        // check if we've won
        if (isTargetAchieved(current, targetPattern)) {
            cerr << "\nTarget found at depth " << current.g_cost << "!" << endl;
            return current.commandHistory + "S"; 
        }

        // generate next moves
        vector<BoardState> nextMoves = getValidMoves(current);
        
        for (BoardState& nextState : nextMoves) {
            if (visited.find(nextState.grid) == visited.end()) {
                visited.insert(nextState.grid);
                
                // Update costs for the new state
                nextState.g_cost = current.g_cost + 1;
                nextState.h_cost = calculateHeuristic(nextState, targetPattern);
                
                pq.push(nextState);
            }
        }
    }

    return "No solution found."; 
}

int main() {
    // Initialize
    BoardState startState;

    // size
    cin >> startState.N;
    int N = startState.N;

    // grid
    startState.grid.resize(N, vector<int>(N));
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cin >> startState.grid[i][j];

            // empty pos
            if (startState.grid[i][j] == -1) {
                startState.emptyRow = i;
                startState.emptyCol = j;
            }
        }
    }

    // command
    startState.commandHistory = "";

    // target
    int targetSize = N - 2;
    vector<vector<int>> targetPattern(targetSize, vector<int>(targetSize));
    for (int i = 0; i < targetSize; ++i) {
        for (int j = 0; j < targetSize; ++j) {
            cin >> targetPattern[i][j];
        }
    }

    // Solver
    string solution = solvePuzzle(startState, targetPattern);
    
    if (solution == "No solution found.") {
        cout << "Result: " << solution << endl;
    } else {
        cout << "Target Achieved!" << endl;
        cout << "Command Sequence: " << solution << endl;
        cout << "Total Moves: " << solution.length() - 1 << endl;
    }

    return 0;
}