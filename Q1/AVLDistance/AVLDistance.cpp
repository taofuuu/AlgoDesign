#include <iostream>
#include <vector>

using namespace std;

int n, k;
long long totalPairs = 0;
vector<int> adj[50005]; // 50005 block of vectors
int dp[50005][505]; // dp[node][distance] -> count how many pair 'node' with paired 'distance' has

void dfs(int parent) {
    dp[parent][0] = 1; // distance 0 is the node itself

    for (int child : adj[parent]) {
        dfs(child); // go down to the leaf first

        // if total distance K + child has nodes at distance 'j'
        // -> need nodes in parent's other subtrees at distance K - (j + 1)
        for (int j = 0; j < k; j++) {
            totalPairs += (long long)dp[parent][k - 1 - j] * dp[child][j];
        }

        // 2. After counting, add the child's distances to the parent's record
        // A node at distance 'j' from child is distance 'j+1' from parent
        for (int j = 0; j < k; j++) {
            dp[parent][j + 1] += dp[child][j];
        }
    }
}

int main() {
    
    cin >> n >> k;
    
    for (int i = 0; i < n - 1; i++) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
    }

    dfs(1); // start at the root
    cout << totalPairs << endl;
    
    return 0;
}