#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

int m = 32717;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    int n, k;
    cin >> n >> k;

    vector<int> r(n + 1);
    for (int i = 1; i <= k; ++i) {
        int a;
        cin >> a;
        r[i] = a % m;
    }
    
    for (int i = k + 1; i <= n; ++i) {
        int p;
        cin >> p;

        ll totalSum = 0;
        for (int j = 0; j < p; ++j) {
            int b;
            cin >> b;

            if (i - b > 0) totalSum = (totalSum + r[i - b]) % m;;
        }
        r[i] = (int)totalSum;

    }

    for (int i = 1; i <= n; ++i) cout << r[i] << " ";
}