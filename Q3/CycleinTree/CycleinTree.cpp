#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int start;
int stop;

bool dfs(int u, vector<vector<int>> &adj, vector<int> &depth, int parent, int d) {
    depth[u] = d;

    for (auto &x : adj[u]) {
        if (depth[x] == -1) {
            if (dfs(x, adj, depth, u, d + 1)) return true;
        } else if (x != parent) {
            start = x;
            stop = u;
            return true;
        }
    }
    return false;
}

int main() {
    int n;
    cin >> n;

    vector<vector<int>> adjList(n);
    int u1, u2;
    for (int i = 0; i < n; ++i) {
        cin >> u1 >> u2;
        adjList[u1].push_back(u2);
        adjList[u2].push_back(u1);
    }

    vector<int> depth(n, -1);
    for (int i = 0; i < n; ++i) {
        if (depth[i] == -1) {
            if (dfs(i, adjList, depth, -1, 0)) {
                cout << abs(depth[stop] - depth[start]) + 1;
                return 0;
            }
        }

    }
}