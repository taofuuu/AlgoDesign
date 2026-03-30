#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

struct Node {
    int r, c;
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

    vector<vector<int>> N(R, vector<int>(C, 0));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> N[i][j];
        }
    }

    vector<vector<int>> S(R, vector<int>(C, 0));
    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            cin >> S[i][j];
        }
    }

    vector<vector<bool>> isPortal(R, vector<bool>(C, false));
    for (int i = 0; i < k; ++i) {
        int r, c; 
        cin >> r >> c;
        isPortal[r - 1][c - 1] = true;
    }

    // pre-calculate span s - connected island (for each tp)
    vector<vector<int>> islandID(R, vector<int>(C, -1));
    vector<int> islandSize;
    int idCounter = 0;

    for (int i = 0; i < R; ++i) {
        for (int j = 0; j < C; ++j) {
            if (S[i][j] == 0 && islandID[i][j] == -1) {
                // Found a new island on Map S
                int size = 0;
                queue<Node> q;
                q.push({i, j});
                islandID[i][j] = idCounter;
                
                while (!q.empty()) {
                    Node u = q.front();
                    q.pop();

                    size++;

                    for (auto &[dr, dc] : dir) {
                        int nr = u.r + dr;
                        int nc = u.c + dc;
                        if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue; 
                        
                        if (S[nr][nc] == 0 && islandID[nr][nc] == -1) {
                            islandID[nr][nc] = idCounter;
                            q.push({nr, nc});
                        }
                    }
                }
                islandSize.push_back(size);
                idCounter++;
            }
        }
    }

    // span N
    queue<Node> qN;
    vector<vector<bool>> visitedN(R, vector<bool>(C, false));
    int travelN = 0;
    int travelS = 0;

    qN.push({0, 0});
    visitedN[0][0] = true;

    while (!qN.empty()) {
        Node u = qN.front();
        qN.pop();

        travelN++;

        if (isPortal[u.r][u.c]) {
            int id = islandID[u.r][u.c];
            if (id != -1) travelS = max(travelS, islandSize[id]);
        }

        for (auto &[dr, dc] : dir) {
            int nr = u.r + dr;
            int nc = u.c + dc;
            if (nr < 0 || nr >= R || nc < 0 || nc >= C) continue; 
            if (!visitedN[nr][nc] && N[nr][nc] == 0) {
                visitedN[nr][nc] = true;
                qN.push({nr, nc});
            }
        }
    }

    cout << travelN + travelS << endl;
}