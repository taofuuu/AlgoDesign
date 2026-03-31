#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <climits>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    vector<vector<int>> dist(n, vector<int>(n, INT_MAX));

    for (int i = 0; i < n; ++i) dist[i][i] = 0;

    for (int i = 0; i < m; ++i) {
        int a, b, w;
        cin >> a >> b >> w;
        dist[a][b] = w;
        dist[b][a] = w;
    }

    // floyd-warshall
    for (int k = 0; k < n; ++k) {
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX)
                dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
            }
        }
    }

    int maxFuel = 0;
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            maxFuel = max(maxFuel, dist[i][j]);
        }
    }

    cout << maxFuel;

    
}