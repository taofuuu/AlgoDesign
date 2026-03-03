#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <climits>

using namespace std;

vector<int> road;
string forbbidenPattern = "";
int n, k;
map<pair<int, string>, int> dp;

int solve(int i, string lastChoice) {
    if (i == n) return 0;

    if (dp.count({i, lastChoice})) return dp[{i, lastChoice}];

    // == don't take 
    string dontTake = lastChoice + '0';
    // if choice exceeds forbbiden pattern, erase first choice (slide window)
    if (dontTake.size() > k) dontTake.erase(0, 1);

    int res = INT_MIN;
    if (dontTake != forbbidenPattern || dontTake.size() < k) res = solve(i + 1, dontTake);
    
    // == take (only when haven't take yet, or last was not taken)
    if (lastChoice.empty() || lastChoice.back() == '0') {
        string take = lastChoice + '1';
        if (take.size() > k) take.erase(0, 1);

        if (take != forbbidenPattern || take.size() < k)
            res = max(res, solve(i + 1, take) + road[i]);
    }

    return dp[{i, lastChoice}] = res;

    
}

int main() {
    cin >> n >> k;

    road.resize(n);
    for (int i = 0; i < n; ++i) {
        cin >> road[i];
    }
    
    for (int i = 0; i < k; ++i) {
        int choice;
        cin >> choice;
        forbbidenPattern += to_string(choice);
    }

    cout << solve(0, "");
}