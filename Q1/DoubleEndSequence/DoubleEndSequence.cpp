#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int n;
    cin >> n;

    vector<int> seq(n);
    vector<int> prefixSum(n); 
    
    // store the minimum prefix sum before the occurrence of a value amongst their duplicate
    map<int, int> minPrevPrefix;

    int currentSum = 0;
    int maxSum = INT_MIN;

    for (int i = 0; i < n; ++i) {
        cin >> seq[i];

        int prevSum = currentSum;

        currentSum += seq[i];
        prefixSum[i] = currentSum;

        // number seen, calc subarray sum
        if (minPrevPrefix.count(seq[i])) {
            minPrevPrefix[seq[i]] = min(minPrevPrefix[seq[i]], prevSum);
        } else {
            // record prevPrefix sum
            minPrevPrefix[seq[i]] = prevSum;
        }

        int candidate = currentSum - minPrevPrefix[seq[i]];

        maxSum = max(maxSum, candidate);
    }

    cout << maxSum;
    return 0;
}