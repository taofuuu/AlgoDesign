#include <iostream>
#include <vector>
#include <climits>

using namespace std;

vector<int> a;
vector<vector<int>> dp;

int mcm(int l, int r) {
    if (l >= r) return 0;
    if (dp[l][r] != -1) return dp[l][r];

    int minCost = INT_MAX;
    for (int i = l; i < r; i++) {
        int cost = mcm(l, i) + mcm(i + 1, r) + (a[l] * a[i + 1] * a[r + 1]);
        minCost = min(minCost, cost);
    }

    return dp[l][r] = minCost;
}

int main() {
    int n;
    cin >> n;
    
    a.resize(n + 1);
    for (int i = 0; i < n + 1; ++i) {
        cin >> a[i];
    }

    // dp setup [0...n][0...n]
    dp.resize(n);
    for (int i = 0; i < n; ++i) {
        dp[i].resize(n);
        for (int j = 0; j < n; ++j) {
            dp[i][j] = -1;
        }
    }

    cout << mcm(0, n - 1);
}