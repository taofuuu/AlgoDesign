#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Node {
    int r, c;
    int dist;
    bool operator>(const Node &other) const {
        return dist > other.dist;
    }
};

vector<vector<pair<int, int>>> adj = {
    // r % 2 == 0 | even
    {{0, -1}, {0, 1}, {-1 , 0}, {-1,  1}, {1, 0}, {1,  1}},
    // r % 2 == 1 | odd
    {{0, -1}, {0, 1}, {-1 , 0}, {-1, -1}, {1, 0}, {1, -1}}
};

int main() {
    int R, C;
    cin >> R >> C;

    int a1, b1, a2, b2;
    cin >> a1 >> b1 >> a2 >> b2;

    // conversion to 0-indexing
    a1--; b1--; a2--; b2--;

    vector<vector<int>> weights(R, vector<int>(C, 0));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> weights[i][j];
        }
    }

    // dijkstra
    vector<vector<int>> dist(R, vector<int>(C, INT_MAX));
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    // initial
    dist[a1][b1] = weights[a1][b1];
    pq.push({a1, b1, dist[a1][b1]});

    while (!pq.empty()) {
        Node curr = pq.top();
        pq.pop();

        int ur = curr.r;
        int uc = curr.c;
        int d = curr.dist;

        // if this route takes longer, skip
        if (d > dist[ur][uc]) continue;

        // reached destination
        if (ur == a2 && uc == b2) break;

        for (auto &[dr, dc] : adj[ur % 2]) {
            int nr = ur + dr;
            int nc = uc + dc;

            // boundary
            if (nr >= 0 && nr < R && nc >= 0 && nc < C) {
                if (dist[ur][uc] + weights[nr][nc] < dist[nr][nc]) {
                    dist[nr][nc] = dist[ur][uc] + weights[nr][nc];
                    pq.push({nr, nc, dist[nr][nc]});
                }
            }
        }
    }

    cout << dist[a2][b2];
}