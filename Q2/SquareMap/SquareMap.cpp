#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int maxSquare = 0;

int main() {
    int r, c;
    cin >> r >> c;

    vector<vector<int>> dp(r, vector<int>(c, 0));
    int maxSquare = 0;

    for (int i = 0; i < r; ++i) {
        string row;
        cin >> row;
        for (int j = 0; j < c; ++j) {
            
            if (row[j] == '1') {
                if (i == 0 || j == 0) dp[i][j] = 1;
                else {
                    // Look at the neighbors: Top, Left, and Top-Left Diagonal
                    dp[i][j] = min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]}) + 1;
                }
                maxSquare = max(maxSquare, dp[i][j]);
            }
        }
    }

    cout << maxSquare;

}

// 1 1 1 1
// 1 1 1 1
// 1 1 1 1
// 1 1 1 1
