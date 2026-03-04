#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    vector<int> p(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> p[i];
    }

    vector<int> dp(n + 1, INT_MAX); // dp[i] = minimum cost to regulate all segment 1...i;

    int minCost = INT_MAX;
    for (int i = 1; i <= n; ++i) {
        // base case: must cover segment 1 -> i - k <= 1
        if (i - k <= 1) dp[i] = p[i];
        else {
            // find best prev station to connect to
            // new station 'i' covers [i-k, i+k]
            // to ensure connection -> prev station must cover at least i - k - 1
            // station at 'prev' covers 'prev + k'
            // ==> prev + k > i - k - 1 ==> prev > i - 2k - 1 
            for (int prev = max(1, i - (2 * k) - 1); prev < i; ++prev) {
                dp[i] = min(dp[i], dp[prev] + p[i]);
            }
        }
    }
    
    // to ensure coverage over n -> start checking at least n - k
    for (int i = max(1, n - k); i <= n; ++i) {
        minCost = min(minCost, dp[i]);
    }
    
    cout << minCost;

}