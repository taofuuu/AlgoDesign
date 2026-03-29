#include <iostream>
#include <vector>

using namespace std;

const int M = 1000003;

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> steps(k);
    for (int i = 0; i < k; ++i) {
        cin >> steps[i];
    }

    vector<int> dp(n + 1, 0); // dp[i] = possible ways to reach 'i' pieces
    dp[0] = 1; // 1 way of achieving 0 -> {} (do nothing)

    for (int i = 1; i <= n; ++i) {
        for (int step : steps) {
            if (i - step >= 0) {
                dp[i] = (dp[i] + dp[i - step]) % M;
            }
        }
    }

    cout << dp[n];
}