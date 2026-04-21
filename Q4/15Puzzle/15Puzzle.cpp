#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

const int N = 4;

int grid[N][N];

int emptyRow, emptyCol;
int threshold;

vector<pair<int, int>> dir = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1}
};

bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < N;
}

int getHeuristic() {
    int totalDist = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            int value = grid[i][j];
            if (value != 0) {
                // expected pos
                int targetRow = (value - 1) / N;
                int targetCol = (value - 1) % N;
                // dist from expected
                totalDist += abs(i - targetRow) + abs(j - targetCol);
            }
        }
    }
    return totalDist;
}

void makeMove(int dr, int dc, int delta) {
    // reverse if undo (delta == -1)
    int nr = dr * delta;
    int nc = dc * delta;
    
    swap(grid[emptyRow][emptyCol], grid[emptyRow + nr][emptyCol + nc]);
    
    emptyRow += nr;
    emptyCol += nc;
}

int solve(int moves, int prevDr, int prevDc) {
    int h = getHeuristic();
    int f = moves + h; // total estimated cost

    // all pieces are in expected pos
    if (h == 0) return 0;

    // exceed current limit, return cost
    if (f > threshold) return f;

    int nextThreshold = INT_MAX;

    for (auto &[dr, dc] : dir) {
        // skip reverse move
        if (dr == -prevDr && dc == -prevDc) continue;

        if (isValid(emptyRow + dr, emptyCol + dc)) {
            makeMove(dr, dc, 1);

            int result = solve(moves + 1, dr, dc);

            // propergates success
            if (result == 0) return 0;

            // find next least moves
            if (result < nextThreshold) nextThreshold = result;

            // backtrack
            makeMove(dr, dc, -1);
        }
    }
    return nextThreshold;
}

int main() {
    // retrieve grid
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            cin >> grid[i][j];

            // track empty pos
            if (grid[i][j] == 0) {
                emptyRow = i;
                emptyCol = j;
            }
        }
    }
    
    // Iterative Deepening A* (IDA*): dfs with threshold
    // Each iteration of DFS, limit threshold by the least move possible of last iteration
    threshold = getHeuristic();
    while (true) {
        int result = solve(0, 0, 0);
        // least found
        if (result == 0) {
            cout << threshold;
            break;
        }

        // increase the limit to next smallest possible cost
        threshold = result;
    }

    return 0;
}