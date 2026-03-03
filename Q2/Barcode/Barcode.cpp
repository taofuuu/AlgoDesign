#include <iostream>
#include <vector>

using namespace std;


int main() {
    int n, m, k;
    cin >> n >> m >> k;
    
    // 'k' color changes -> 'k + 1' blocks 

    // dp[i][j] = number of ways to use 'i' block to reach total length 'j'
    // needed k + 1 to reach n -> reserve k + 2 (one-offset)
    vector<vector<int>> dp(k + 2, vector<int>(n + 1, 0));

    // base case: 1 block
    for (int len = 1; len <= m && len <= n; ++len) { // limit by 'm' consecutive color AND 'n' length
        dp[1][len] = 1;
    }

    // start from 2 blocks -> k + 1 blocks
    for (int i = 2; i <= k + 1; ++i) { // for each block
        for (int j = 1; j <= n; ++j) { // for each length target
            // get total length 'j' with 'i' blocks
            // the i-th block (last) must have length x (1 <= x <= m);
            // so the rest must have summed up to 'j-x'
            // try every possible x from 1 -> m AND 'j-x' must still have a length (>= 1); 
            for (int x = 1; x <= m && j - x >= 1; x++)  
                dp[i][j] += dp[i - 1][j - x];
        }
    }

    cout << dp[k + 1][n];
}