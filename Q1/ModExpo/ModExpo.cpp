#include <iostream>

using namespace std;

typedef long long ll;

ll n, x, k;

ll multMod(ll x, ll y, ll k) {
    return ((x % k) * (y % k)) % k;
}

int main() {
    cin >> x >> n >> k;
    ll base = 1 % k;
    while (n > 0) {
        if (n % 2 == 1) {
            base = multMod(base, x, k);
        }
        x = multMod(x, x, k);
        n /= 2;
    }

    cout << base;
}