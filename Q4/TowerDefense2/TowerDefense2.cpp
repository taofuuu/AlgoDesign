#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

using namespace std;

int main() {
    int n, m, k, w;
    cin >> n >> m >> k >> w;

    vector<int> monsterPos(m);
    for (int i = 0; i < m; ++i) {
        cin >> monsterPos[i];
    }

    map<int, int> grid;
    int totalHp = 0;
    for (int i = 0; i < m; ++i) {
        int hp;
        cin >> hp;
        grid[monsterPos[i]] += hp;
        totalHp += hp;
    }

    int towerPos = 0;
    for (auto  &[pos, hp] : grid) {
        int left = max({1, pos - w, towerPos + 1});
        int right = min(n, pos + w);

        for (int t = left; t <= right; ++t) {
            totalHp--;
            hp--;
            k--;
            towerPos = t;

            if (k == 0 || hp == 0) break;
        }

        if (k == 0) break;
    }

    cout << totalHp;
}