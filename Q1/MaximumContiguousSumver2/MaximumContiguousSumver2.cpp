#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

using namespace std;

vector<int> kadane;
int minSum = INT_MAX;
int maxSum = INT_MIN;

int main() {
    int n;
    cin >> n;

    kadane.resize(n);
    for (size_t i = 0; i < n; ++i) {
        cin >> kadane[i];
    }

    int currentMaxSum = 0;
    int currentMinSum = 0;
    int totalSum = 0;
    for (int x : kadane) {
        totalSum += x;

        currentMaxSum += x;
        currentMinSum += x;

        if (currentMaxSum > maxSum) maxSum = currentMaxSum;
        if (currentMaxSum < 0) currentMaxSum = 0; 

        if (currentMinSum < minSum) minSum = currentMinSum;
        if (currentMinSum > 0) currentMinSum = 0;
    }

    // all negative number -> choose max num
    if (maxSum < 0) {
        cout << maxSum << endl;
    // check possible circular max num
    // Idea: think of a pie (circular), max possible weighted cut is taken out
    //       -> what left is the lightest piece
    //   | Total = maxSum(circular) + minSum |
    //   | maxSum(circular) = total - minSum |
    } else {
        int maxCircular = totalSum - minSum;
        cout << max(maxSum, maxCircular) << endl; // still check for possible linear sequence
    }
}