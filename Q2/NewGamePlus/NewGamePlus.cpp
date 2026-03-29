#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

const int M = 100000007;

int main() {
    int r, c;
    cin >> r >> c;
    
    vector<vector<int>> maze(r + 2, vector<int>(c + 2, 0)); // +2 for all side border
    for (int i = 1; i <= r; ++i) {
        for (int j = 1; j <= c; ++j) {
            int val;
            cin >> val;
            maze[i][j] = 1 - val; // flip value
        }
    }

    // dp[row][col][dir]
    // dir: 0 - up(from i + 1) | 1 - straight(from i) | 2 - down(from i - 1)
    vector<vector<vector<int>>> dp(r + 2, vector<vector<int>>(c + 1, vector<int>(3, 0)));
    // r + 2: border up-down
    // c + 1: border start

    // BASE: config col 2 previous choice (since col 1 have no previous choice)
    for (int i = 1; i <= r; ++i) { // in col
        if (!maze[i][2]) continue;

        for (int prev_i = 1; prev_i <= r; ++prev_i) { 
            if (!maze[prev_i][1]) continue;
            
            // take all possible route from col 1 w/o constraint
            if (prev_i == i + 1)      dp[i][2][0] = (dp[i][2][0] + 1) % M; // up from col1
            else if (prev_i == i)     dp[i][2][1] = (dp[i][2][1] + 1) % M; // straight from col1
            else if (prev_i == i - 1) dp[i][2][2] = (dp[i][2][2] + 1) % M; // down from col1
        }
    }

    for (int j = 3; j <= c; ++j) {
        for (int i = 1; i <= r; ++i) {
            if (!maze[i][j]) continue;
            
            // arrive [i][j] by going up (0)
            dp[i][j][0] = (dp[i + 1][j - 1][1] + dp[i + 1][j - 1][2]) % M;

            // arrive [i][j] by going straight (1)
            dp[i][j][1] = (dp[i][j - 1][0] + dp[i][j - 1][2]) % M;

            // arrive [i][j] by going down (2)
            dp[i][j][2] = (dp[i - 1][j - 1][0] + dp[i - 1][j - 1][1]) % M;
        }
    }

    // sum dir
    int total = 0;
    for (int i = 1; i <= r; ++i) {
        for (int d = 0; d < 3; ++d) total = (total + dp[i][c][d]) % M;
    }
    cout << total;
}