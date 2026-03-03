#include <iostream>
#include <vector>

using namespace std;

vector<vector<int>> dp;
vector<int> futomaki;
int currentMax = 0;

int eat(int l, int r) {
    if (l > r) return 0;
    
    if (dp[l][r] != -1) return dp[l][r];
    
    int ll = max(futomaki[l], futomaki[l + 1]) + eat(l + 2, r);
    int rr = max(futomaki[r], futomaki[r - 1]) + eat(l, r - 2);
    int lr = max(futomaki[l], futomaki[r])     + eat(l + 1, r - 1);
    
    return dp[l][r] = max(lr, max(ll, rr));
}

int main() {
    int n;
    cin >> n;

    futomaki.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> futomaki[i];
    }

    // dp setup
    dp.resize(n);
    for (int i = 0; i < n; ++i) {
        dp[i].resize(n);
        for (int j = 0; j < n; ++j) {
            dp[i][j] = -1;
        }
    }

    cout << eat(0, n - 1);
}