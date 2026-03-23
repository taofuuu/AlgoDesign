#include <iostream>
#include <vector>

using namespace std;

void linear(int u, vector<vector<int>> &adj, vector<bool> &visited, int &nodeCount, int &degreeCount, bool &hasHighDegree) {
    visited[u] = true;

    nodeCount++;
    degreeCount += adj[u].size();

    if (adj[u].size() > 2) {
        hasHighDegree = true;
    }

    for (auto &x : adj[u]) {
        if (!visited[x]) {
            linear(x, adj, visited, nodeCount, degreeCount, hasHighDegree);
        }
    }
}

int main() {
    int v, e;
    cin >> v >> e;

    vector<vector<int>> adj(v);
    int u1, u2;
    for (int i; i < e; ++i) {
        cin >> u1 >> u2;
        adj[u1].push_back(u2);
        adj[u2].push_back(u1);
    }

    int linearCount = 0;
    vector<bool> visited(v, false);
    for (int i = 0; i < v; ++i) {
        if (!visited[i]) {
            int nodeCount = 0;
            int degreeCount = 0;
            bool hasHighDegree = false;

            linear(i, adj, visited, nodeCount, degreeCount, hasHighDegree);

            // sum degree = 2 * edge
            // no cycle  -> edges = node - 1
            // no branch -> max degree <= 2
            int actualEdges = degreeCount / 2;
            if (!hasHighDegree && (actualEdges == nodeCount - 1)) linearCount++;
        }
    }

    cout << linearCount;
}