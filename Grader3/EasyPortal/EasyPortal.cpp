#include <iostream>
#include <vector>
#include <climits>
#include <queue>

using namespace std;

vector<pair<int, int>> dir = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

int main() {
    ios_base::sync_with_stdio(false); cin.tie(0);

    int R, C;
    cin >> R >> C;

    pair<int, int> start, stop, blackHole, whiteHole;
    vector<vector<char>> grid(R, vector<char>(C));
    for (int i = 0; i < R; ++i) {
        string line;
        cin >> line;
        for (int j = 0; j < C; ++j) {
            grid[i][j] = line[j];
            if (grid[i][j] == 'S') start = {i, j};
            else if (grid[i][j] == 'T') stop = {i, j};
            else if (grid[i][j] == 'B') blackHole = {i, j};
            else if (grid[i][j] == 'W') whiteHole = {i, j};
        }
    }

    vector<vector<int>> dist(R, vector<int>(C, -1));
    queue<pair<int, int>> q;
    q.push(start);
    dist[start.first][start.second] = 0;

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        // destination reached
        if (grid[r][c] == 'T') break;

        if (grid[r][c] == 'B' && dist[whiteHole.first][whiteHole.second] == -1) {
            q.push(whiteHole);
            dist[whiteHole.first][whiteHole.second] = dist[blackHole.first][blackHole.second] + 1;
        }

        for (auto &[dr, dc] : dir) {
            int nr = r + dr;
            int nc = c + dc;

            // border, wall, visted check
            if (nr < 0 || nr >= R || nc < 0 || nc >= C || grid[nr][nc] == '#' || dist[nr][nc] != -1) continue;


            dist[nr][nc] = dist[r][c] + 1;
            if (grid[nr][nc] == '.' || grid[nr][nc] == 'B' || grid[nr][nc] == 'W') {
                q.push({nr, nc});
            }
        }
    }
    
    cout << dist[stop.first][stop.second];

}