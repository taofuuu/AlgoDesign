#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Edge {
    int weight, node;

    // for prim's algo - pick least weight first
    bool operator> (const Edge &other) const {
        return weight > other.weight;
    }
};

int main() {
    int n;
    cin >> n;

    vector<vector<Edge>> adj(n);

    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < n - i - 1; ++k) {
            int weight;
            cin >> weight;

            int neighbor = i + k + 1;

            adj[i].push_back({weight, neighbor});
            adj[neighbor].push_back({weight, i});
        }
    }

    // prim
    int minTotal = 0;
    int edgeCount = 0;
    vector<bool> visited(n, false);
    priority_queue<Edge, vector<Edge>, greater<Edge>> pq;

    pq.push({0, 0});

    // spanning tree req -> edge = n - 1 (edge < n)
    while (!pq.empty() && edgeCount < n) {
        int cost = pq.top().weight;
        int u    = pq.top().node;
        pq.pop();

        if (!visited[u]) {
            visited[u] = true;
            minTotal += cost;
            edgeCount++;

            for (auto &e : adj[u]) {
                int weight = e.weight;
                int v      = e.node;
                if (!visited[v]) {
                    pq.push({weight, v});
                }
            }
        }
    }

    cout << minTotal;
}