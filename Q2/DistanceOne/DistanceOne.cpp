#include <iostream>
#include <vector>

using namespace std;

const int M = 100000007;
int count = 0;
int n, k;

int main() {
    cin >> n >> k;
    
    vector<int> dp(n + 1);

    // base case: empty string
    dp[0] = 1;

    for (int i = 0; i <= n; ++i) {
        // add zero = same possible outcome
        dp[i] = dp[i - 1];

        // add one, only if the there are 'k' zeros before
        if (i - k >= 0) dp[i] = (dp[i] + dp[i - k]) % M;
        // not enough zeros before -> no '1' have been placed yet
        else dp[i] = (dp[i] + 1) % M;
    }

    cout << dp[n];
}