#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Edge {
    int from, to, weight;
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(NULL); 

    int n, m, k, v;
    cin >> n >> m >> k >> v;

    vector<int> startNode(k);
    for (int i = 0; i < k; ++i) {
        cin >> startNode[i];
    }


    vector<Edge> edges(m);
    for (int i = 0; i < m; ++i) {
        int from, to, weight;
        cin >> from >> to >> weight;
        edges[i]  = {to, from, weight};
    }

    // bellman-ford
    vector<int> dist(n, INT_MAX);
    dist[v] = 0;

    for (int i = 1; i <= n - 1; ++i) {
        for (int j = 0; j < m; ++j) {
            int u = edges[j].from;
            int v = edges[j].to;
            int w = edges[j].weight;

            if (dist[u] != INT_MAX && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
            }
        }
    }

    int minStart = INT_MAX;
    for (auto &start : startNode) {
        minStart = min(minStart, dist[start]);
    }

    cout << minStart;

}