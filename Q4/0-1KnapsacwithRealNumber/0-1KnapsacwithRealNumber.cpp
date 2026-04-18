#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Item {
    double v, w, ratio;
};

int N;
double W;
double bestValue = 0;
vector<Item> items;

// cal heuristic val: assume fractional (always > 0/1)
double getBound(int i, double curWeight, double curValue) {
    if (curWeight >= W) return 0;
    
    double bound = curValue;
    double remainingW = W - curWeight;
    
    for (int j = i; j < N; j++) {
        if (items[j].w <= remainingW) {
            remainingW -= items[j].w;
            bound += items[j].v;
        } else {
            // fill with fraction of next item
            bound += items[j].v * (remainingW / items[j].w);
            break;
        }
    }
    return bound;
}

void solve(int i, double curWeight, double curValue) {
    if (i == N) {
        if (curValue > bestValue) bestValue = curValue;
        return;
    }


    if (curWeight + items[i].w <= W) {
        solve(i + 1, curWeight + items[i].w, curValue + items[i].v);
    }

    if (getBound(i + 1, curWeight, curValue) > bestValue) {
        solve(i + 1, curWeight, curValue);
    }
}

int main() {
    cin >> W >> N;

    items.resize(N);
    vector<double> v(N), w(N);
    for (int i = 0; i < N; i++) cin >> v[i];
    for (int i = 0; i < N; i++) cin >> w[i];

    for (int i = 0; i < N; i++) {
        items[i] = {v[i], w[i], v[i] / w[i]};
    }

    // sort item based on value (to be able to cal frational knapsack correctly)
    sort(items.begin(), items.end(), [](Item a, Item b) {
        return a.ratio > b.ratio;
    });

    solve(0, 0, 0);

    cout << fixed << setprecision(4) << bestValue << endl;
    return 0;
}