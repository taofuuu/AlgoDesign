#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

vector<int> lawn;
vector<int> grassPrefix, costPrefix;
int k;

int main() {
    int n, m;
    cin >> n >> m >> k;

    lawn.resize(n);
    costPrefix.resize(n + 1);
    grassPrefix.resize(n + 1);

    for (size_t i = 0; i < n; ++i) {
        cin >> lawn[i];
    }

    for (size_t i = 0; i < n; ++i) {
        grassPrefix[i + 1] = grassPrefix[i] + lawn[i];
        // costPrefix[length] = currentTotalGrass @length + (length * cost)
        costPrefix[i + 1] = grassPrefix[i + 1] + ((i + 1) * k);
    }

    while (m--) {
        int start, budget;
        cin >> start >> budget;

        // add costPrefix[start] for offset
        int target_val = costPrefix[start] + budget;

        // bsearch for target
        auto it = upper_bound(costPrefix.begin() + start + 1, costPrefix.end(), target_val);

        // find grass length
        // upper_bound find next min value -> -1 prev under budget
        int r_plus_1 = distance(costPrefix.begin(), it) - 1;

        if (r_plus_1 <= start) {
            cout << 0 << endl;
        } else {
            cout << grassPrefix[r_plus_1] - grassPrefix[start] << endl;
        }
    }
}