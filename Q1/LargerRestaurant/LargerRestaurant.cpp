#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

long long countServed(long long time, const vector<int> &times) {
    long long total = 0;
    for (int t : times) {
        total += (time / t) + 1;
    }
    return total;
}

int main() {
    int n, a;
    cin >> n >> a;

    vector<int> time; time.reserve(n);
    int minTime = 1000;
    for (size_t i = 0; i < n; i++) {
        int x;
        cin >> x;
        time.push_back(x);
        if (time[i] < minTime) minTime = x;
    }

    // Binary search for each query
    while (a--) {
        long long k;
        cin >> k;

        long long low = 0;
        long long high = k * minTime; // least highest time possible
        long long ans = high;

        while (low <= high) {
            long long mid = low + (high - low) / 2;
            if (countServed(mid, time) >= k) {
                ans = mid;
                high = mid - 1;
            } else {
                low = mid + 1;
            }
        }

        cout << ans << "\n";
    }
}