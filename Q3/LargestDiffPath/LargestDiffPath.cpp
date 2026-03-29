#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<int> weights(n);
    for (int i = 0; i < n; ++i) {
        cin >> weights[i];
    }

    vector<vector<int>> adj(n);
    for (int i = 0; i < m; ++i) {
        int u1, u2;
        cin >> u1 >> u2;

        adj[u1].push_back(u2);
    }

    vector<int> inDegree(n, 0);
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) inDegree[v]++;
    }

    // dp[i] = min value ending at node 'i'
    vector<int> dp(n, INT_MAX);
    queue<int> q;

    for (int i = 0; i < n; i++) {
        if (inDegree[i] == 0) {
            q.push(i);
            dp[i] = weights[i];
        }
    }

    int maxDiff = 0;

    while (!q.empty()) {
        int u = q.front();
        q.pop();

        for (int v : adj[u]) {
            // The max diff at node v could be (Weight[v] - min weight found before v)
            maxDiff = max(maxDiff, weights[v] - dp[u]);

            // update min value (check branching from self)
            dp[v] = min({dp[v], dp[u], weights[v]});

            inDegree[v]--;
            if (inDegree[v] == 0) q.push(v);
        }
    }

    cout << maxDiff;

}