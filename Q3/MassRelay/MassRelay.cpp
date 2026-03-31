#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct DSU {
    vector<int> parent;
    int numCluster;

    // constructor
    DSU(int n) {
        parent.resize(n);
        for (int i = 0; i < n; ++i) parent[i] = i;
        numCluster = n;
    }

    int find(int i) {
        if (parent[i] == i) return i;
        return parent[i] = find(parent[i]);
    }

    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
            numCluster--;
            return true; // unite complete
        }
        return false; // already in union
    }
};

struct Edge {
    int from, to, weight;
    bool operator< (const Edge &other) const {
        return weight < other.weight;
    }
};

int main() {
    int N, M, Q;
    cin >> N >> M >> Q;

    vector<Edge> edges(N);
    for (int i = 0; i < M; ++i) {
        int a, b, weight;
        cin >> a >> b >> weight;
        edges.push_back({a, b, weight});
    }

    // kruskal's
    sort(edges.begin(), edges.end());
       
    vector<int> maxWeight(N + 1, 0); // max weight for each numCluster
    DSU dsu(N);
    for (auto &[a, b, weight] : edges) {
        if (dsu.unite(a, b)) {
            maxWeight[dsu.numCluster] = weight;
        }
    }

    // query: shop count -> cluster count
    for (int i = 0; i < Q; ++i) {
        int d;
        cin >> d;
        cout << maxWeight[d] << endl;
    }
}