#include <iostream>
#include <vector>

using namespace std;

typedef long long ll;

vector<ll> arr;
vector<ll> prefixSum; // sum(l, r) = p[r + 1] - p[l];

ll sumArr(ll l, ll r) {
    return prefixSum[r + 1] - prefixSum[l];
}

bool solve(ll l, ll r, ll x) {
    if (l == r) {
        return arr[l] == x;
    }

    ll mid = (l + r) / 2;

    ll length = mid - l + 1;

    // case 1
    if (sumArr(l, mid) == x * length) {
        return solve(mid + 1, r, x + 1);

    // case 2
    } else if (sumArr(mid + 1, r) == x * length) {
        return solve(l, mid, x + 1);

    } else {
        return false;
    }
}

int main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0);

    ll t;
    cin >> t;

    while (t--) {
        ll n, x;
        cin >> n >> x;

        ll size = 1 << n;
        arr.resize(size);
        prefixSum.resize(size + 1);
        for (ll i = 0; i < size; ++i) {
            cin >> arr[i];
            prefixSum[i + 1] = prefixSum[i] + arr[i];
        }

        // for (ll i = 0; i < size + 1; ++i) cout << prefixSum[i] << " ";

        if (solve(0, size - 1, x)) {
            cout << "YES\n";
        } else {
            cout << "NO\n";
        }
    }
}