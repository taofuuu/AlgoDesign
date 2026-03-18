#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

struct Interval
{
    int start;
    int stop;
};

bool sortByStop(Interval l, Interval r) {
    if (l.stop == r.stop) return l.start < r.start;
    return l.stop < r.stop;
}


int main() {
    int n;
    cin >> n;

    vector<Interval> gd(n);
    for (int i = 0; i < n; ++i) cin >> gd[i].start;
    for (int i = 0; i < n; ++i) cin >> gd[i].stop;

    sort(gd.begin(), gd.end(), sortByStop);

    int count = 1;
    Interval lastPick = gd[0];
    for (int i = 1; i < n; ++i) {
        Interval pick = gd[i];
        if (pick.start >= lastPick.stop) {
            count++;
            lastPick = pick;
        }
    }

    cout << count;
}