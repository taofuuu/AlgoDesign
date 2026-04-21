#include <iostream>
#include <vector>
#include <climits>

using namespace std;

int n;
vector<vector<int>> f;
vector<int> b;
int maxPower = INT_MIN;

void solve(int member, vector<int> &candidate, int curPower) {
    if (candidate.empty()) {
        if (curPower > maxPower) maxPower = curPower;
        return;
    }

    int potentialPower = curPower;
    for (int &c : candidate) potentialPower += b[c];
    if (potentialPower <= maxPower) return;

    while (!candidate.empty()) {
        int nMember = candidate.back();
        candidate.pop_back();

        vector<int> nCandidate;
        for (int c : candidate) {
            if (f[nMember][c]) {
                nCandidate.push_back(c);
            }
        }

        solve(nMember, nCandidate, curPower + b[nMember]);
    }
}

int main() {
    cin >> n;

    b.resize(n);
    vector<int> candidates(n);
    for (int i = 0; i < n; ++i) {
        cin >> b[i];
        candidates[i] = i;
    }

    f.resize(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cin >> f[i][j];
        }
    }

    solve(-1, candidates, 0);

    cout << maxPower;
}