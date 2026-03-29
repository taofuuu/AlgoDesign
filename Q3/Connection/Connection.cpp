#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int main() {
    int n, e, k;
    cin >> n >> e >> k;

    vector<vector<int>> adj(n);
    int u, v;
    for (int i = 0; i < e; ++i) {
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int maxFriends = 1;
    
    for (int i = 0; i < n; ++i) {
        vector<int> dist(n, -1);
        dist[i] = 0;
        int currentFriends = 0;

        // bfs
        queue<int> q;
        q.push(i);
        while (!q.empty()) {
            u = q.front();
            q.pop();

            currentFriends++;

            if (dist[u] < k) {
                for (auto v : adj[u]) {
                    if (dist[v] == -1) {
                        dist[v] = dist[u] + 1;
                        q.push(v);
                    }
                }
            }
        }
        maxFriends = max(maxFriends, currentFriends);
    }
    
    cout << maxFriends;
}