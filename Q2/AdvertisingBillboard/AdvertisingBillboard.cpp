#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n;
    cin >> n;

    int sumExclude = 0;
    int sumInclude = 0;
    int currentVal;
    for (int i = 0; i < n; ++i) {
        cin >> currentVal;

        int nextMax = max(sumExclude, (currentVal + sumInclude));

        sumInclude = sumExclude;
        sumExclude = nextMax;
    }

    cout << max(sumInclude, sumExclude);
}