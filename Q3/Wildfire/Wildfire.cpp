#include <iostream>
#include <vector>

using namespace std;

void spread(int u, vector<vector<int>> &adj, vector<int> &b, int &totalB) {
    if (b[u] == 0) return;
    
    totalB -= b[u];
    b[u] = 0;

    for (auto &x : adj[u]) {
        spread(x, adj, b, totalB);
    }
}

int main() {
    int n, m, k;
    cin >> n >> m >> k;

    int totalB = 0;
    vector<int> b(n);
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
        totalB += b[i];
    }

    vector<int> fire(k);
    for (int i = 0; i < k; ++i) cin >> fire[i];
    
    vector<vector<int>> adj(n);
    int u1, u2;
    for (int i = 0; i < m; ++i) {
        cin >> u1 >> u2;
        adj[u1].push_back(u2);
    }

    for (auto &p : fire) {
        spread(p, adj, b, totalB);
        cout << totalB << " ";
    }
}