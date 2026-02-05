#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> virus;
int n, k;

int checkVirus(int l, int r) {
    if (l == r) {
        return virus[l];
    }
    
    int mid = (l + r) / 2;

    // propergate error if violates viruis condition
    // else, pass 1's count
    int leftCount = checkVirus(l, mid);
    if (leftCount == -1) return -1;

    int rightCount = checkVirus(mid + 1, r);
    if (leftCount == -1) return -1;
    
    if (abs(leftCount - rightCount) > 1) return -1;

    return leftCount + rightCount;
}

int main() {
    cin >> n >> k;

    int size = 1 << k;
    virus.resize(size);
    
    while (n--) {
        for (size_t i = 0; i < size; ++i) cin >> virus[i];

        if (checkVirus(0, size - 1) != -1) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }


}