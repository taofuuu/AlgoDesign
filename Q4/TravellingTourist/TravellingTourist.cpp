#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <climits>

using namespace std;

int n, m, k;

vector<int> attractions;
vector<vector<pair<int, int>>> adjList;
vector<vector<int>> dists;
vector<vector<int>> memo;

int leastCost = INT_MAX;

void dijkstra(int startAttraction) {
    vector<int> d(n + 1, INT_MAX);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;

    int startNode = attractions[startAttraction];
    d[startNode] = 0;
    pq.push({0, startNode});

    while (!pq.empty()) {
        int currDist = pq.top().first;
        int u = pq.top().second;
        pq.pop();

        if (currDist > d[u]) continue;

        for (auto &edge : adjList[u]) {
            int v = edge.first;
            int weight = edge.second;
            if (d[u] + weight < d[v]) {
                d[v] = d[u] + weight;
                pq.push({d[v], v});
            }
        }
    }

    for (int i = 0; i < k; ++i) {
        dists[attractions[startAttraction]][attractions[i]] = d[attractions[i]];
    }
}

// State-Space Search with Memoization
void solve(int station, int cost, map<int, bool> &visited, int visitCount) {
    if (visitCount == k) {
        if (cost < leastCost) leastCost = cost;
        return;
    }

    int res = INT_MAX;
    for (auto &a : attractions) {
        if (!visited[a]) {
            visited[a] = true;

            int moveCost = dists[station][a];
            solve(a, cost + moveCost, visited, visitCount + 1);

            visited[a] = false;
        }
    }
}

int main() {
    cin >> n >> m >> k;

    map<int, bool> visited;
    attractions.resize(k);
    for (int i = 0; i < k; ++i) {
        cin >> attractions[i];
        visited[attractions[i]] = false;
    }

    dists.resize(n + 1, vector<int>(n + 1));
    adjList.resize(n + 1);
    for (int i = 0; i < m; ++i) {
        int a, b, c;
        cin >> a >> b >> c;
        adjList[a].push_back({b, c});
        adjList[b].push_back({a, c});
    }

    // calc all shortest path from attraction to attraction
    for (int i = 0; i < k; ++i) {
        dijkstra(i);
    }

    for (auto &a : attractions) {
        visited[a] = true;
        solve(a, 0, visited, 1);
        visited[a] = false;
    }

    cout << leastCost;
}