#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> endPoint = {0, 1, 3}; // value 'i' will ends at endPoint[i]
int value = 3; // value to be added
int pointer = 2; // pointer to endPoint => how many times 'value (An)' will appear

void generateSequence(int n) {
    while (endPoint.back() < n) {
        while (endPoint[pointer] < value) {
            ++pointer;
        }

        endPoint.push_back(pointer + endPoint.back());
        ++value;
    }
}

int main() {
    int n;
    cin >> n;

    while (n--) {
        int x;
        cin >> x;
        if (x > endPoint.back()) {
            generateSequence(x);
        }
        // if 'x' lies in the range [endPoint[a], endPoint[b]], then x = a
        cout << lower_bound(endPoint.begin(), endPoint.end(), x) - endPoint.begin() << endl;
    }
}