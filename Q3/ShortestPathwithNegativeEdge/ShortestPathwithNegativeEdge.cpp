#include <iostream>
#include <vector>
#include <climits>

using namespace std;

struct Edge {
    int from, to, weight;
};

int main() {
    int n, e, s;
    cin >> n >> e >> s;

    vector<Edge> edges(e);
    for (int i = 0; i < e; ++i) {
        int from, to, weight;
        cin >> from >> to >> weight;
        edges[i] = {from, to, weight};
    }


    // bellman-ford
    vector<int> dist(n, INT_MAX);
    dist[s] = 0;

    for (int i = 1; i <= n - 1; ++i) {
        for (int j = 0; j < e; ++j) {
            int u = edges[j].from;
            int v = edges[j].to;
            int weight = edges[j].weight;
            
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
                dist[v] = dist[u] + weight;
            }
        }
    }

    for (auto &edge : edges) {
        if (dist[edge.from] != INT_MAX && 
            dist[edge.from] + edge.weight < dist[edge.to]) {
                cout << -1;
                return 0;
        }
    }

    for (auto &d : dist) {
        cout << d << " ";
    }
}