#include <iostream>
#include <vector>

using namespace std;

vector<vector<pair<int,int>>> adj = {
    {},
    {{0, -1}, {0,  1}},
    {{-1, 0}, {1,  0}},
    {{-1, 0}, {0,  1}},
    {{0,  1}, {1,  0}},
    {{1,  0}, {0, -1}},
    {{0, -1}, {-1, 0}},
};
int R, C;
int maxLoop = 0;

bool dfs(int r, int c, vector<vector<int>> &grid, vector<vector<int>> &visited, vector<vector<int>> &depth, int pr, int pc, int d) {
    visited[r][c] = 1;
    depth[r][c] = d;
    
    for (auto &[dr, dc] : adj[grid[r][c]]) {
        int nr = r + dr;
        int nc = c + dc;

        if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue;
        if (nr == pr && nc == pc) continue;
        
        bool canConnectBack = false;
        for (auto &[ndr, ndc] : adj[grid[nr][nc]]) {
            if (nr + ndr == r && nc + ndc == c) {
                canConnectBack = true;
                break;
            }
        }
        if (!canConnectBack) continue;


        if (visited[nr][nc] == 1) {
            int loopLength = d - depth[nr][nc] + 1;
            maxLoop = max(maxLoop, loopLength);
            return true; 
        }

        if (!visited[nr][nc]) {
            dfs(nr, nc, grid, visited, depth, r, c, d + 1);
        }
    }

    visited[r][c] = 2;
    return false;
}

int main() {
    cin >> R >> C;

    vector<vector<int>> grid(R, vector<int>(C, 0));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> grid[i][j];
        }
    }

    int cycleCount = 0;
    vector<vector<int>> visited(R, vector<int>(C, 0));
    vector<vector<int>> depth(R, vector<int>(C, 0));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (!visited[i][j]) {
                if (dfs(i, j, grid, visited, depth, -1, -1, 0)) cycleCount++;
            }
        }
    }

    cout << cycleCount << " " << maxLoop;
    
}