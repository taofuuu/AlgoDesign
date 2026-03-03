#include <iostream>
#include <vector>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    int cnk[n + 1][k + 1];

    for (int i = 0; i <= n; ++i) {
        for (int j = 0; j <= i && j <= k; ++j) {
            if (i == j || j == 0) {
                cnk[i][j] = 1;
            } else {
                cnk[i][j] = cnk[i - 1][j - 1] + cnk[i - 1][j];
            }
        }
    }

    cout << cnk[n][k];
}