#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<vector<int>> dp(n + 1, vector<int>(n + 1));
    vector<int> seq(n + 1, 0);
    for (int i = 1; i <= n; ++i) {
        cin >> seq[i];
    }

    vector<int> sorted(seq);
    sort(sorted.begin(), sorted.end());

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= n; ++j) {
            if (i == 0 || j == 0) dp[i][j] = 0;
            else if (seq[i] == sorted[j]) dp[i][j] = dp[i - 1][j - 1] + 1;
            else dp[i][j] = max(dp[i - 1][j], dp[i][j - 1]);
        }
    }

    cout << dp[n][n];
}