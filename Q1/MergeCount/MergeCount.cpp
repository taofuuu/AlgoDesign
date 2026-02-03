#include <iostream> 
#include <vector>
#include <queue>
#include <algorithm>
#include <numeric>

using namespace std;

vector<int> arr;
int k;

void solve(int l, int r) {
    // stop if k swap have been done (1 for root)
    // or on leaf node
    if (k <= 1 || l >= r - 1) {
        return;
    }

    // split mergesort on Left and Right
    k -= 2;

    int mid = (l + r) / 2;

    // swap first (reversal)
    swap(arr[mid - 1], arr[mid]);
    
    solve(l, mid);
    solve(mid, r);
}

int main() {
    int n;
    cin >> n >> k;

    // must be odd (root + binaray tree)
    // cannot exceed binary tree nodes (2n - 1)
    if (k % 2 == 0 || k > 2 * n - 1) {
        cout << -1 << endl;
        return 0;
    }

    arr.resize(n);
    // generate [1..n]
    iota(arr.begin(), arr.end(), 1);

    solve(0, n);

    for (size_t i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }

}