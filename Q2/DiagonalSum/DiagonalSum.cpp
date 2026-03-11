#include <iostream>
#include <vector>

using namespace std;


int main() {
    int n;
    cin >> n;

    vector<vector<int>> a(n, vector<int>(n));

    int maxDiag = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> a[i][j];

            if (i - 1 >= 0 && j - 1 >= 0) {
                a[i][j] = max(a[i][j], a[i][j] + a[i - 1][j - 1]);
            }
            maxDiag = max(maxDiag, a[i][j]);
        }
    }

    // for (auto &x : a) {
    //     for (auto &y : x) cout << y << " ";
    //     cout << endl;
    // }

    cout << maxDiag;
}