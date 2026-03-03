#include <iostream>
#include <vector>


using namespace std;

int m = 32717;

int main() {
    int k, n;
    cin >> k >> n;

    vector<int> c(k);
    vector<int> a(k);
    vector<int> r(n + 1);
    for (int i = 0; i < k; ++i) {
        cin >> c[i];
    }
    for (int i = 0; i < k; ++i) {
        cin >> a[i];
    }

    for (int i = 0; i <= n; ++i) {
        if (i < k) {
            r[i] = a[i] % m;
        } else {
            r[i] = 0;
            for (int j = 1; j <= k; ++j) {
                int temp = (c[j - 1] % m) * (r[i - j] % m);
                r[i] = (r[i] % m) + (temp % m);
            }
        }
    }

    cout << r[n] % m;

}