#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;

bool sortByFirst(pair<double, double> l, pair<double, double> r) {
    return l.first * r.second > r.first * l.second;
}

int main() {
    double w;
    int n;
    cin >> w >> n;

    vector<pair<double, double>> valueWeight(n);
    for (int i = 0; i < n; ++i) {
        cin >> valueWeight[i].first;
    }
    for (int i = 0; i < n; ++i) {
        cin >> valueWeight[i].second;
    }

    sort(valueWeight.begin(), valueWeight.end(), sortByFirst);

    double totalValue = 0.0;
    for (int i = 0; i < n && w > 0; ++i) {
        if (valueWeight[i].second <= w) {
            w -= valueWeight[i].second;
            totalValue += valueWeight[i].first;
        } else {
            totalValue += (valueWeight[i].first / valueWeight[i].second) * w;
            w = 0;
        }
    }

    cout << fixed << setprecision(4) << totalValue;
}