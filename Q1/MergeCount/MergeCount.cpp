#include <iostream> 
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    int n, k;
    cin >> n >> k;

    if (k % 2 == 0 || k > 2 * n - 1) { // must be odd (2 new branches + root) and < 2n - 1
        cout << -1 << endl;
        return 0;
    }

    vector<int> arr; arr.reserve(n);
    for (size_t i = 1; i <= n; i++) {
        arr.push_back(i);
    }

    // root call
    k--;

    queue<pair<int,int>> q;
    q.push({0, n - 1});

    while (k > 0) {
        auto [left, right] = q.front();
        q.pop();

        if (left < right) {
            int mid = (left + right) / 2;

            swap(arr[mid - 1], arr[mid]); // swap only adj. (guaranteed unsort)
            k -= 2; // 2 branches called

            // call on each sub array
            q.push({left, mid - 1});
            q.push({mid, right});
        }
    }

    for (size_t i = 0; i < n; ++i) {
        cout << arr[i] << " ";
    }

}