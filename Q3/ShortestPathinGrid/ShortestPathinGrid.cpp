#include <iostream>
#include <vector>
#include <queue>

using namespace std;

vector<pair<int, int>> dir = {
    {0, 1},
    {-1, 0},
    {0, -1},
    {1, 0}
};

int main() {
    int R, C;
    cin >> R >> C;

    vector<vector<int>> maze(R, vector<int>(C, 0));

    string row;
    for (int i = 0; i < R; ++i) {
        cin >> row;
        for (int j = 0; j < C; ++j) {
            maze[i][j] = (row[j] == '.');
        }
    }
    
    // bfs
    vector<vector<int>> dist(R, vector<int>(C, -1));
    queue<pair<int, int>> q;
    q.push({0, 0});
    dist[0][0] = 0;

    while (!q.empty()) {
        int r = q.front().first;
        int c = q.front().second;
        q.pop();

        // exit reached
        if (r == R - 1 && c == C - 1) break;
        
        for (auto &[dr, dc] : dir) {
            int nr = r + dr;
            int nc = c + dc;
            
            // boundary + obstruction check
            if (nr < 0 || nr >= R || nc < 0 || nc >= C ||
                maze[nr][nc] == 0) continue;
            
            if (dist[nr][nc] == -1) {
                dist[nr][nc] = dist[r][c] + maze[nr][nc];
                q.push({nr, nc});
            }
        }
    }

    cout << dist[R - 1][C - 1];
}