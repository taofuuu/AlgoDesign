#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>

// Score: 90/100

using namespace std;

int runCount = 0;

void merge(vector<char> &a, int l, int m, int r) {
    int i = l;
    int j = m + 1;
    vector<char> b;

    while (i <= m && j <= r) {
        if (a[i] <= a[j]) {
            b.push_back(a[i++]);
        } else {
            b.push_back(a[j++]);
        }
    }

    while (i <= m) {
        b.push_back(a[i++]);
        runCount++;
    }

    while (j <= r) {
        b.push_back(a[j++]);
        runCount++;
    }

    for (int i = l, j = 0; i <= r; ++i, ++j) {
        a[i] = b[j];
    }
}

void merge_sort(vector<char> &a, int l, int r) {
    if (l >= r) return;
    
    int m = (l + r) / 2;
    merge_sort(a, l, m);
    merge_sort(a, m + 1, r);
    merge(a, l, m, r);
}

void merge_op(vector<char> &a, int l, int m, int r) {
    runCount += r - m;
}

void merge_sort_op(vector<char> &a, int l, int r) {
    if (l >= r) return;
    
    int m = (l + r) / 2;
    merge_sort_op(a, l, m);
    merge_sort_op(a, m + 1, r);
    merge_op(a, l, m, r);
}

int main() {
    int n;
    cin >> n;
    string line;
    cin >> line;

    vector<char> bs(n);
    for (int i = 0; i < n; ++i) {
        bs[i] = line[i];
    }

    if (n <= 1000000) {
        merge_sort(bs, 0, n - 1);
    } else {
        merge_sort_op(bs, 0, n - 1);
    }

    cout << runCount;
}