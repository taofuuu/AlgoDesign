#include <iostream>
#include <vector>

using namespace std;

const int M = 1997;

int main() {
    int n, k;
    cin >> n >> k;

    if (k == 1) {
        cout << 1;
        return 0;
    }

    vector<vector<int>> dp(n + 1, vector<int>(k + 1, 0)); // dp[i][j] = splitting 'i' items into 'j' groups
    
    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= k; ++j) {
            if (i < j || i == 0 || j == 0) dp[i][j] = 0; // BASE: while n < k, it is impossible
            else if (i == j) dp[i][j] = 1;
            else {
                dp[i][j] = (((dp[i - 1][j] * j) % M) + dp[i - 1][j - 1]) % M;
            }
        }
    }

    cout << dp[n][k];
}