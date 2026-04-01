#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

// Score: 75/100

using namespace std;

long long getScore(long long i, long long j) {
    return (i + j) * (i + j) * (i + j);
}

int main() {
    int n;
    cin >> n;

    vector<long long> height(n);
    for (int i = 0; i < n; ++i) {
        cin >> height[i];
    }

    vector<long long> dp(n);
    dp[0] = 0;
    for (int i = 1; i < n; ++i) {
        long long maxScore = 0;
        for (int j = 0; j < i; ++j) {
            maxScore = max(maxScore, getScore(height[j], height[i]) + dp[j]);
        }
        dp[i] = maxScore;
    }

    cout << dp[n - 1];
}