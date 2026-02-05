#include <iostream>
#include <vector>
#include <climits>

using namespace std;

vector<int> kadane;
int maxSum = INT_MIN;

int main() {
    int n;
    cin >> n;

    kadane.resize(n);
    for (size_t i = 0; i < n; ++i) cin >> kadane[i];

    int currentSum = 0;
    for (auto x : kadane) {
        currentSum += x;

        if (currentSum > maxSum) maxSum = currentSum;

        if (currentSum < 0) currentSum = 0;
    }

    cout << maxSum;
}