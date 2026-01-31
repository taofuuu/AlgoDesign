#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    int n, a, b;
    cin >> n;

    vector<pair<int, int>> line(n);
    for (int i = 0; i < n; ++i) cin >> line[i].first >> line[i].second;

    sort(line.begin(), line.end());

    vector<pair<int, int>> merged;
    for (auto const &section : line) {
        if (merged.empty() || section.first - 1 > merged.back().second) {
            merged.push_back(section);
        } else {
            merged.back().second = max(section.second, merged.back().second);
        }
    }

    for (auto const &p : merged) {
        cout << p.first << " " << p.second << " ";
    }
}