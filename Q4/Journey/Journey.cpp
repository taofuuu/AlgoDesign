#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

int n;
int bestVal = INT_MIN;
vector<int> bestExit;
vector<vector<int>> grid;

void solve(int u, int count, int val, int potential, vector<bool> &visited) {
    if (u == n - 1) { // check if it's the last town
        if (count == n) {
            if (val > bestVal) bestVal = val;
        }
        return;
    }

    if (val + bestExit[u] + potential <= bestVal) return;

    for (int v = 0; v < n; ++v) {
        if (!visited[v]) {
            // don't visit last town until it's the last town
            if (v == n - 1 && count < n - 1) continue;

            visited[v] = true;

            int nextPotential = potential;
            // v visited, subtract bestExit
            if (v != n - 1) nextPotential -= bestExit[v];

            solve(v, count + 1, val + grid[u][v], nextPotential, visited);
            visited[v] = false;
        }
    }
} 

int main() {
    cin >> n;

    grid.resize(n, vector<int>(n));
    bestExit.resize(n, INT_MIN);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> grid[i][j];

            // for heuristic: best exit from each town
            if (i != j) {
                bestExit[i] = max(bestExit[i], grid[i][j]);
            }
        }
    }

    // cal heuristic: potential, sum of all exit
    int potential = 0;
    for (int i = 1; i < n - 1; ++i) potential += bestExit[i];

    vector<bool> visited(n, false);
    visited[0] = true;
    solve(0, 1, 0, potential, visited);

    cout << bestVal;

}