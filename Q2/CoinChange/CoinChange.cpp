#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

vector<int> dp;

int main() {
    int n, target;
    cin >> n >> target;

    dp.assign(target + 1, INT_MAX);
    dp[0] = 0;

    // dp[x] = y; y coins to make x
    for (int i = 1; i <= n; ++i) {
        int coin;
        cin >> coin;

        // look every possible sum 'j' from 'coin' to 'target'
        for (int j = coin; j <= target; ++j) {
            if (dp[j - coin] != INT_MAX) {
                // dp[j - coin] -> amount needed to make before accounting 'coin'
                dp[j] = min(dp[j], dp[j - coin] + 1); // update if this coin reduce the count
            }
        }
    }

    cout << dp[target];

}