#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;


int main() {
    int n;
    cin >> n;
    
    vector<int> goat(n);
    for (int i = 0; i < n; ++i) {
        cin >> goat[i];
    }

    sort(goat.begin(), goat.end());

    int currentWinner = goat[0];
    long long totalCost = 0;
    for (int i = 1; i < n; ++i) {
        totalCost += currentWinner * goat[i];
        if (goat[i] > currentWinner) currentWinner = goat[i];
    }

    cout << totalCost;
}