#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

typedef long long ll;

vector<int> base;
vector<int> prefixSum;
ll p, k, a, b;

int getCount(int l, int r) {
    auto it1 = lower_bound(base.begin(), base.end(), l);
    auto it2 = upper_bound(base.begin(), base.end(), r);
    return distance(it1, it2);
}

ll miniMax(int l, int r) {
    // get avenger count between [l, r]
    int m = getCount(l, r);
    if (!m) return a; // DO NOT Branch if NO avenger inside [l,r]
    // ex. branching -> left = a | right = a => total = 2a (still no avenger after branching)
    // which is always greater than 'a' - no need to branch

    if (l == r) {
        return b * m;
    }

    ll noDivide = b * m * (r - l + 1);

    int mid = (l + r) / 2;
    ll divide = miniMax(l, mid) + miniMax(mid + 1, r); // cost of spliting

    return min(divide, noDivide); // take lowest route
}

int main() {
    cin >> p >> k >> a >> b;

    base.resize(k);
    
    for (int i = 0; i < k; ++i) {
        cin >> base[i];
    }
    
    // sort to allow lower/upper bound uses (bsearch O(log n))
    sort(base.begin(), base.end());
    
    int size = 1 << p;
    cout << miniMax(1, size);
}