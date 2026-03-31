#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

struct Edge {
    pair<int, int> from, to;    
};

vector<pair<int,int>> dir = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

int main() {
    int R, C;
    cin >> R >> C;
    
    vector<vector<int>> grid(R, vector<int>(C));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> grid[i][j];
        }
    }

    // dijkstra
    vector<vector<int>> dist(R, vector<int>(C, INT_MAX));
    queue<pair<int,int>> q;
    q.push({0,0});
    dist[0][0] = 0;

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        for (auto &[dr, dc] : dir) {
            int nr = dr + r;
            int nc = dc + c;

            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;

            if (dist[nr][nc] > dist[r][c] + grid[nr][nc]) {
                dist[nr][nc] = dist[r][c] + grid[nr][nc];
                q.push({nr, nc});
            }
        }
    }

    for (auto &x : dist) {
        for (auto &y : x) {
            cout << y << " ";
        }
        cout << endl;
    }

}