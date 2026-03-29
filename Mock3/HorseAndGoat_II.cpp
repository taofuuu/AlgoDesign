#include <iostream>
#include <vector>
#include <string>
#include <queue>

using namespace std;

struct Node {
    int r, c;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

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
    vector<bool> row_done(N, false);
    vector<bool> col_done(M, false);

    queue<Node> q;
    q.push(start);
    dist[start.r][start.c] = 0;

    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    while (!q.empty()) {
        Node u = q.front();
        q.pop();

        int d = dist[u.r][u.c];

        // reached stop
        if (u.r == stop.r && u.c == stop.c) {
            cout << d << endl;
            return 0;
        }

        // normal (.)
        for (int i = 0; i < 4; i++) {
            int nr = u.r + dr[i];
            int nc = u.c + dc[i];

            if (nr >= 0 && nr < N && 
                nc >= 0 && nc < M && 
                grid[nr][nc] != 'X' && dist[nr][nc] == -1
            ) {
                dist[nr][nc] = d + 1;
                q.push({nr, nc});
            }
        }

        // rocket pad (#)
        if (grid[u.r][u.c] == '#') {
            // if visited earlier, skip
            if (!row_done[u.r]) {
                for (int j = 0; j < M; j++) {
                    if (grid[u.r][j] != 'X' && dist[u.r][j] == -1) {
                        dist[u.r][j] = d + 1;
                        q.push({u.r, j});
                    }
                }
                row_done[u.r] = true;
            }
            if (!col_done[u.c]) {
                for (int i = 0; i < N; i++) {
                    if (grid[i][u.c] != 'X' && dist[i][u.c] == -1) {
                        dist[i][u.c] = d + 1;
                        q.push({i, u.c});
                    }
                }
                col_done[u.c] = true;
            }
        }
    }

    cout << (dist[stop.r][stop.c]) << endl; 
    return 0;
}