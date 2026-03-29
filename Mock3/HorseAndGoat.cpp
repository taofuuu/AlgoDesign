#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

struct Node {
    int r, c;
};

vector<Node> getAdj(Node u, int N, int M, vector<string>& grid) {
    vector<Node> res;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nr = u.r + dr[i];
        int nc = u.c + dc[i];

        if (nr >= 0 && nr < N && nc >= 0 && nc < M && grid[nr][nc] != 'X') {
            res.push_back({nr, nc});
        }
    }
    
    return res;
}

int main() {
    int N, M;
    cin >> N >> M;

    vector<string> grid(N);
    Node start, stop;

    for (int i = 0; i < N; ++i) {
        cin >> grid[i];
        for (int j = 0; j < M; ++j) {
            if (grid[i][j] == 'S') start = {i, j};
            if (grid[i][j] == 'T') stop = {i, j};
        }
    }

    vector<vector<int>> dist(N, vector<int>(M, -1));
    queue<Node> q;

    q.push(start);
    dist[start.r][start.c] = 0;


    while (!q.empty()) {
        Node u = q.front();
        q.pop();

        if (u.r == stop.r && u.c == stop.c) break;

        for (auto &[nr, nc] : getAdj(u, N, M, grid)) {
            if (dist[nr][nc] == -1) {
                dist[nr][nc] = dist[u.r][u.c] + 1;
                q.push({nr, nc});
            }
        }
    }

    cout << dist[stop.r][stop.c] << endl;

    return 0;
}