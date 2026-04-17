#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, l;
    cin >> n >> l;

    vector<int> hole(n);
    for (int i = 0; i < n; ++i) {
        cin >> hole[i]; 
    }
    sort(hole.begin(), hole.end());

    int halfBoard = l / 2;
    int boardCnt = 0;
    int boardCoverage = -1;
    for (int &pos : hole) {
        if (pos > boardCoverage) {
            ++boardCnt;
            boardCoverage = pos + l - 1;
        }
    }

    cout << boardCnt;
}