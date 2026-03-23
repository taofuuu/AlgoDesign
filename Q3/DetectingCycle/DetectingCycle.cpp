#include <iostream>
#include <vector>

using namespace std;

bool dfs(int u, vector<vector<int>> &adj, vector<bool> &visited, int parent) {
    visited[u] = true;

    for (auto &x : adj[u]) {
        if (!visited[x]) {
            if (dfs(x, adj, visited, u)) return true;
        } else if (x != parent) {
            return true;
        }
    }
    return false;
}

bool cycle(int n, vector<vector<int>> &adj) {
    vector<bool> visited(n, false);
    for (int i = 0; i < n; ++i) {
        if (!visited[i]) {
            if (dfs(i, adj, visited, -1)) return true;
        }
    }
    return false;
}

int main() {
    int t;
    cin >> t;
    while (t--) {
        int n, e;
        cin >> n >> e;

        vector<vector<int>> adjList(n);
        int u1, u2;
        for (int i = 0; i < e; ++i) {
            cin >> u1 >> u2;
            adjList[u1].push_back(u2);
            adjList[u2].push_back(u1);
        }

        if (cycle(n, adjList)) cout << "YES" << endl;
        else cout << "NO" << endl;

    }
}