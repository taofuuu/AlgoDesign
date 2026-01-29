#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, m, k, w;
    cin >> n >> m >> k >> w;

    // Setup

    vector<int> p(m);
    for (int i = 0; i < m; ++i) {
        cin >> p[i];
    }

    vector<int> h(m);
    for (int i = 0; i < m; ++i) {
        cin >> h[i];
    }

    vector<int> t(k);
    for (int i = 0; i < k; ++i) {
        cin >> t[i];
    }
    // sort position
    sort(t.begin(), t.end());

    // Algo

    // pairs position <=> health
    vector<pair<int,int>> enemyInfo(m);
    for (int i = 0; i < m; ++i) {
        enemyInfo[i] = make_pair(p[i], h[i]);
    }

    // sort position
    sort(enemyInfo.begin(), enemyInfo.end());

    size_t enemPtr = 0, towerPtr = 0;
    while (enemPtr < m && towerPtr < k) {
        pair<int, int> &enemy = enemyInfo[enemPtr];

        if (enemy.second == 0)  {
            ++enemPtr;
            continue;
        } // skip if health == 0

        if (t[towerPtr] - w <= enemy.first &&
            enemy.first <= t[towerPtr] + w
        ) {
            --enemy.second; // tower shoots
            ++towerPtr; // next tower (<-|-<)
        } else if (t[towerPtr] - w > enemy.first) {
            ++enemPtr; // out of tower entry range (next target)
        } else if (t[towerPtr] + w < enemy.first) {
            ++towerPtr; // out of tower end range (skip tower)
        }

    }

    int totalHealth = 0;
    for (auto &[_, health] : enemyInfo) {
        totalHealth += health;
    }

    cout << totalHealth;
}