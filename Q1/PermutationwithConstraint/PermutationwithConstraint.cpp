#include <iostream>
#include <vector>
#include <queue>

using namespace std;

void genPermu(int n, vector<vector<int>> graph, vector<int> indegree, vector<bool> used, vector<int> current) {

    if (current.size() == n) {
        for(auto &x : current) cout << x << ' ';
        cout << '\n';
        return;
    }

    for (int i = 0; i < n; ++i) {
        if (!used[i] && indegree[i] == 0) { // not used + aall 'before' constraint is met
            
            // choose i
            used[i] = true;
            current.push_back(i);

            for (int v : graph[i]) --indegree[v]; // remove 'before' constraint condition

            genPermu(n, graph, indegree, used, current);

            // backtrack for new 'i'
            for (int v : graph[i]) ++indegree[v];
            current.pop_back();
            used[i] = false;
        }
    }

}

int main() {
    int n, m, a, b;
    cin >> n >> m;
    vector<pair<int,int>> constraints;
    for (int i = 0; i < m; ++i) {
        cin >> a >> b;
        constraints.emplace_back(a, b);
    }

    vector<vector<int>> graph(n);
    vector<int> indegree(n);
    vector<bool> used(n);
    vector<int> current;

    for (auto [a, b] : constraints) {
        graph[a].push_back(b);
        indegree[b]++;
    }

    genPermu(n, graph, indegree, used, current);
}