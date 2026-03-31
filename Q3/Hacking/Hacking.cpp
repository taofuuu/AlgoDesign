#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int n, m, k;
    cin >> n >> m >> k;

    vector<int> startNode(k);
    for (int i = 0; i < k; ++i) {
        cin >> startNode[i];
    }

    vector<int> defense(n);
    for (int i = 0; i < n; ++i) {
        cin >> defense[i];
    }

    vector<vector<int>> cable(n);
    for (int i = 0; i < m; ++i) {
        int a, b;
        cin >> a >> b;
        cable[a].push_back(b);
        cable[b].push_back(a);
    }

    // dijkstra
    vector<int> dist(n, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    
    for (auto &start : startNode) {
        dist[start] = defense[start];
        pq.push({dist[start], start});
    }
    
    while (!pq.empty()) {
        int u = pq.top().second;
        int time = pq.top().first;
        pq.pop();

        if (time > dist[u]) continue;
        
        for (auto &v : cable[u]) {
            if (dist[u] + defense[v] < dist[v]) {
                dist[v] = dist[u] + defense[v];
                pq.push({dist[v], v});
            }
        }
    }
    
    int maxTime = 0;
    for (auto &time : dist) maxTime = max(maxTime, time);

    cout << maxTime;
}