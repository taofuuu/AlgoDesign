#include <iostream>
#include <map>
#include <vector>

using namespace std;

int main() {
    int n, q, x, c;
    cin >> n >> q;

    map<int, int> xc;
    for (int i = 0; i < n; ++i) {
        cin >> x >> c;
        xc[x] += c;
    }
    
    vector<pair<int, int>> arr; arr.reserve(xc.size());
    int count = 0;
    for (auto const &[val, amount] : xc) {
        count += amount;
        arr.push_back({count, val});
    }

    int querry;
    for (int i = 0; i < q; ++i) {
        cin >> querry;

        auto it = lower_bound(arr.begin(), arr.end(), make_pair(querry, -1));

        cout << it->second << endl;
    }
}