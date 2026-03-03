#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> dp, triangle;
int n;

int bestPath(int x, int y) {
    if (x > n) return 0;

    if (dp[x][y] != -1) return dp[x][y];

    return dp[x][y] = max(bestPath(x + 1, y + 1), bestPath(x + 1, y)) + triangle[x][y];
}

int main() {
    cin >> n;

    // setup triangle [1...n][1...n] + dp setup
    triangle.resize(n + 1);
    dp.resize(n + 1);
    for (int i = 1; i <= n; ++i) {
        triangle[i].resize(i + 1); // + [0]
        dp[i].resize(i + 1);
        for (int j = 1; j <= i; ++ j) {
            cin >> triangle[i][j];
            dp[i][j] = -1;
        }
    }

    cout << bestPath(1, 1);
}