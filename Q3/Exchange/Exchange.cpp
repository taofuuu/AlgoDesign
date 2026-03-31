#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

struct Edge {
    int from, to;
    float weight;
};


int main() {
    int k;
    cin >> k;

    for (int m = 0; m < k; ++m) {
        int n;
        cin >> n;

        vector<vector<float>> rateTable(n, vector<float>(n));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> rateTable[i][j];
            }
        }

        // bellman-ford
        vector<Edge> edges;
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // normal bellman-ford: a + w < b
                // conversion rate    : a * r > b
                // => log(a * r) = log(a) + log(r)
                // =>  log(a) + log(r) >  log(b)
                // => -log(a) - log(r) < -log(b)
                edges.push_back({i, j, -log(rateTable[i][j])});
            }
        }

        vector<float> dist(n, 0.0);

        for (int i = 0; i < n - 1; ++i) {
            for (auto &[u, v, w] : edges) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                }
            }
        }

        bool found = false;
        for (auto &[u, v, w] : edges) {
            if (dist[u] + w < dist[v]) {
                cout << "YES\n";
                found = true;
                break;
            }
        }
        if (!found) cout << "NO\n";
    }
}