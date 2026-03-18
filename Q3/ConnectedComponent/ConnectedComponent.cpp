#include <iostream>
#include <vector>

using namespace std;

void dfs(int u, int id, vector<int> &visited, vector<vector<int>> &adjList) {
    visited[u] = id;
    for (auto &adj : adjList[u]) {
        if (visited[adj] == 0)
            dfs(adj, id, visited, adjList);
    }
}

int main() {
    int v, e;
    cin >> v >> e;

    vector<int> visited(v + 1, 0);
    vector<vector<int>> adjList(v + 1);

    int u1, u2;
    for (int i = 0; i < e; ++i) {
        cin >> u1 >> u2;
        adjList[u1].push_back(u2); 
        adjList[u2].push_back(u1); 
    }

    int id = 0;
    for (int i = 1; i <= v; ++i) {
        if (visited[i] == 0) {
            id++;
            dfs(i, id, visited, adjList);
        }
    }

    cout << id;
}