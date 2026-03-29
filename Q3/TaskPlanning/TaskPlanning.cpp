#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

int main() {
    int n, m;
    cin >> n;

    vector<vector<int>> prereq(n);
    vector<int> zeroDegree;
    vector<int> inDegree(n);
    for (int i = 0; i < n; ++i) {
        cin >> m;

        if (m == 0) zeroDegree.push_back(i);
        else {
            for (int j = 0; j < m; ++j) {
                int e;
                cin >> e;
                prereq[e].push_back(i);
                inDegree[i]++;         
            }
        }
    }

    vector<int> order;
    queue<int> q;
    for (auto &z : zeroDegree) {
        q.push(z);
    }

    int u;
    while (!q.empty()) {
        u = q.front();
        q.pop();

        order.push_back(u);

        for (auto &v : prereq[u]) {
            inDegree[v]--;
            if (inDegree[v] == 0) {
                q.push(v);
            }
        }
    }

    for (auto &x : order) cout << x << " ";    
}