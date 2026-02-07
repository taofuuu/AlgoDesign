#include <iostream>
#include <vector>
#include <deque>
#include <climits>
#include <algorithm>

using namespace std;

vector<int> prefixSum;

void printTracker(vector<int> &v, int l, int r) {
    cout << "Current Deque: ";
    while (l <= r) {
        cout << v[l++] << " ";    
    }
    cout << endl;
}

int main() {
    int n, w;
    cin >> n >> w;

    prefixSum.resize(n + 1);
    prefixSum[0] = 0;
    int x;
    for (size_t i = 1; i <= n; ++i) {
        cin >> x;
        prefixSum[i] = prefixSum[i - 1] + x;
    }

    // KEY IDEA
    // to find max sum -> prefix sum: P[j] - P[x]
    //  - P[x] is minimized (to get max value)
    //  - j - w < x < j (within the window)
    //
    // => Iterate over every end point, and find the minimized P[x] of each window
    //  - gets the maximum value possible from all iteration
    //
    // USES DEQUE to keep track of the sliding window
    // keep minimized possible P[x] at the front

    deque<int> dq;
    dq.push_back(0);

    int mcs = INT_MIN; // track max value
    for (int j = 1; j <= n; ++j) {
        if (!dq.empty() && dq.front() < j - w) {
            // slides the window if current end is extending over 'w'
            dq.pop_front();
        }

        if (!dq.empty()) {
            // track possible max value 
            mcs = max(mcs, prefixSum[j] - prefixSum[dq.front()]);
        }

        while (!dq.empty() && prefixSum[dq.back()] >= prefixSum[j]) {
            // if this P[currentEnd] is lower than pushed index, replace them
            // in order to push minimized P[x] to the front
            // Note: discard all P[x] that is higher than P[currentEnd]
            dq.pop_back();
        }

        dq.push_back(j);
    }  

    cout << mcs;
}