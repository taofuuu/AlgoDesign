#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> virus;
int n, k;

bool checkVirus(int l, int r, bool mutated) {
    if (r - l == 1) {
        if (!mutated) {
            return (virus[l] == 0 && virus[r] == 1);
        } else {
            return (virus[l] == 1 && virus[r] == 0);
        }
    }

    int mid = (l + r) / 2;

    bool left_ok, right_ok;
    if (!mutated) { // currently not mutated
        left_ok = checkVirus(l, mid, true) || checkVirus(l, mid, false); // check possible mutation
        right_ok = checkVirus(mid + 1, r, false); // all left-subtree should not be mutated!
    } else { // currently mutated (reversed order left <=> right)
        left_ok = checkVirus(l, mid, true);
        right_ok = checkVirus(mid + 1, r, true) || checkVirus(mid + 1, r, false); // == left -> check possible mutation
    }
    return left_ok && right_ok;

}

int main() {
    cin >> n >> k;
    int size = 1 << k; // 2 ^ k
    virus.resize(size);
    
    while (n--) {
        for (size_t i = 0; i < size; ++i) cin >> virus[i];

        if (checkVirus(0, virus.size() - 1, false)) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }

    }
}