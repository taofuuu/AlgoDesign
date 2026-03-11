#include <iostream>
#include <vector>

using namespace std;


int main() {
    int n;
    cin >> n;
    
    vector<int> dp(n), score(n);
    for (int i = 0; i < n; ++i) {
        cin >> score[i];

        if (i == 0) dp[i] = score[i];
        else {
            if (i - 1 >= 0) dp[i] = dp[i - 1] + score[i];
            if (i - 2 >= 0) dp[i] = max(dp[i], dp[i - 2] + score[i]);
            if (i - 3 >= 0) dp[i] = max(dp[i], dp[i - 3] + score[i]);
        }
    }

    cout << dp[n - 1];
}