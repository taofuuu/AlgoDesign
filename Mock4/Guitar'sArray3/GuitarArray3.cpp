#include <iostream>
#include <vector>
#include <string>
#include <climits>

using namespace std;

int N, M;
vector<string> grid;
vector<string> bestBoard;
int minTowers = INT_MAX;
int possibleScore = 0;

// Movement Deltas
vector<pair<int, int>> qDir = {
    {-1, 0},
    {1, 0},
    {0, -1},
    {0, 1},
    {-1, -1},
    {-1, 1},
    {1, -1},
    {1, 1}
};
vector<pair<int, int>> knDir = {
    {-2, -1},
    {-2, 1},
    {-1, -2},
    {-1, 2},
    {1, -2},
    {1, 2},
    {2, -1},
    {2, 1}
};

bool isValid(int r, int c) {
    return r >= 0 && r < N && c >= 0 && c < M;
}

void updateCoverage(int r, int c, int delta, vector<vector<int>> &coverage, vector<pair<int, int>> &changes) {

    if (grid[r][c] == '.') {
        if (delta > 0 && coverage[r][c] == 0) changes.push_back({r, c});
    }
    coverage[r][c] += delta;

    // q
    for (auto &[dr, dc] : qDir) {
        int nr = r + dr;
        int nc = c + dc;
        while (isValid(nr, nc)) {
            if (grid[nr][nc] == '.') {
                if (delta > 0 && coverage[nr][nc] == 0) changes.push_back({nr, nc});
            }
            coverage[nr][nc] += delta;
            nr += dr;
            nc += dc;
        }
    }
    // kn
    for (auto &[dr, dc] : knDir) {
        int nr = r + dr;
        int nc = c + dc;
        if (isValid(nr, nc)) {
            if (grid[nr][nc] == '.') {
                if (delta > 0 && coverage[nr][nc] == 0) changes.push_back({nr, nc});
            }
            coverage[nr][nc] += delta;
        }
    }
}

void solve(int towersPlaced, int score, vector<vector<int>>& coverage, vector<vector<bool>>& isTower) {
    // prune
    if (towersPlaced >= minTowers) return;

    if (score == possibleScore) {
        minTowers = towersPlaced;
        bestBoard = grid;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                if (isTower[i][j]) bestBoard[i][j] = 'X';
            }
        }
        return;
    }

    // find first uncovered
    int tr = -1, tc = -1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == '.' && coverage[i][j] == 0) {
                tr = i; tc = j; break;
            }
        }
        if (tr != -1) break;
    }

    // heuristic: place where tower can see (tr, tc)
    vector<pair<int, int>> candidates;
    if (grid[tr][tc] != '#') candidates.push_back({tr, tc});

    for (auto &[dr, dc] : qDir) {
        int nr = tr + dr;
        int nc = tc + dc;
        while (isValid(nr, nc)) {
            if (grid[nr][nc] != '#') candidates.push_back({nr, nc});
            nr += dr;
            nc += dc;
        }
    }
    for (auto &[dr, dc] : knDir) {
        int nr = tr + dr;
        int nc = tc + dc;
        if (isValid(nr, nc) && grid[nr][nc] != '#') {
            candidates.push_back({nr, nc});
        }
    }

    // try candidates
    for (auto &[r, c]: candidates) {
        if (isTower[r][c]) continue;

        vector<pair<int, int>> changes;
        isTower[r][c] = true;
        updateCoverage(r, c, 1, coverage, changes);

        solve(towersPlaced + 1, score + (int)changes.size(), coverage, isTower);

        // backtrack
        isTower[r][c] = false;
        vector<pair<int, int>> dummy;
        updateCoverage(r, c, -1, coverage, dummy);
    }
}

int main() {
    cin >> N >> M;

    grid.resize(N);
    vector<vector<int>> coverage(N, vector<int>(M, 0));
    vector<vector<bool>> isTower(N, vector<bool>(M, false));

    for (int i = 0; i < N; i++) {
        cin >> grid[i];
        for (int j = 0; j < M; j++) {
            if (grid[i][j] == '.') possibleScore++;
        }
    }

    solve(0, 0, coverage, isTower);

    for (string& row : bestBoard) cout << row << "\n";

    return 0;
}