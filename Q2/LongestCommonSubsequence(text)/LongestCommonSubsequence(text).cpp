#include <iostream>
#include <vector>

using namespace std;

int main() {
    int na, nb;
    string a, b;
    cin >> na >> nb >> a >> b;

    vector<vector<int>> dp(na + 1, vector<int>(nb + 1));
    for (int i = 0; i <= na; ++i) {
        for (int j = 0; j <= nb; ++j) {
            cin >> dp[i][j];
        }
    }

    int i = na;
    int j = nb;
    string res = "";
    while (i > 0 && j > 0) {
        if (dp[i - 1][j - 1] + 1 == dp[i][j]) {
            res = a[i - 1] + res;
            --i;
            --j;
        }
        else --i;
    }

    cout << res;
}