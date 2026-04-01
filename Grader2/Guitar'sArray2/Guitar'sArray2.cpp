#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int n, m;

void create(vector<int> &v, bool appendGreater) {
    if (v.size() == n) {
        for (auto &x : v) cout << x << " ";
        cout << '\n';
    } else if (v.size() == 1) {
        for (int i = 1; i <= m; ++i) {
            if (i != v.back()) {
                vector<int> vNew(v);
                vNew.push_back(i);
                create(vNew, i < v.back());
            }
        }
    } else {
        if (appendGreater) {
            for (int i = v.back() + 1; i <= m; ++i) {
                vector<int> vNew(v);
                vNew.push_back(i);
                create(vNew, false);
            }
        } else {
            for (int i = 1; i < v.back(); ++i) {
                vector<int> vNew(v);
                vNew.push_back(i);
                create(vNew, true);
            }
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false); std::cin.tie(0);

    cin >> n >> m;

    for (int i = 1; i <= m; ++i) {
        vector<int> sub = {i};
        create(sub, false);
    }
}