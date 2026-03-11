#include <iostream>
#include <vector>

using namespace std;


int main() {
    int n, m, k;
    cin >> n >> m >> k;
    
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= m; ++j) {
            if (i == 0 || j == 0) {
                dp[i][j] = 0;
                continue;
            }

            int val;
            cin >> val;

            // calculate
            dp[i][j] = val + dp[i - 1][j] + dp[i][j - 1] - dp[i - 1][j - 1];
        }
    }

    for (int i = 0; i < k; ++i) {
        int r1, c1, r2, c2;
        cin >> r1 >> c1 >> r2 >> c2;
        r1++; r2++; c1++; c2++;
        int a = dp[r2][c1 - 1];
        int b = dp[r1 - 1][c2];
        cout << dp[r2][c2] - a - b + dp[r1 - 1][c1 - 1] << endl;
    }
}


// - 0 1 2 3 4 5 6
// 0 x x x b x x x
// 1 x 1 x x x x x
// 2 x x x x x x x
// 3 a x x 2 x x x
// 4 x x x x x x x
// 5 x x x x x x x
// 6 x x x x x x x
// r1 r2 c1 c2 += 1 (offset)
// a = (r2, c1 - 1)
// b = (r1 - 1, c2)
// 1 - 2 = 2 - a - b + 1