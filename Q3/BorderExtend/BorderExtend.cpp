#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Node {
    int r, c, dist;
};

vector<pair<int, int>> dir = {
    {-1, 0},
    {0, 1},
    {1, 0},
    {0, -1}
};

int main() {
    int R, C, k;
    cin >> R >> C >> k;

    vector<vector<int>> grid(R, vector<int>(C, 0));
    queue<Node> land;
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> grid[i][j];
            if (grid[i][j] == 1) land.push({i, j, 0});
        }
    }
    
    while (!land.empty()) {
        Node u = land.front();
        land.pop();

        if (u.dist >= k) continue;

        for (auto &[dr, dc] : dir) {
            int nr = u.r + dr;
            int nc = u.c + dc;

            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;

            if (grid[nr][nc] == 0) {
                grid[nr][nc] = 2;
                land.push({nr, nc, u.dist + 1});
            }
        }
    }

    // output
    for (auto &x : grid) {
        for (auto &y : x) cout << y << " ";
        cout << "\n";
    }
}