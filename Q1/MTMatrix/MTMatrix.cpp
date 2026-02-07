#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

using namespace std;

typedef long long ll;

vector<int> mBase;

int transposeMatrix(vector<int> &matrix, int pos) {
    int width = sqrt(matrix.size());
    int r = pos / width;
    int c = pos % width;
    return matrix[(c * width) + r];
}

int getVal(int n, ll r, ll c) {
    if (n == 1) {
        return mBase[(r * 2) + c];
    }

    ll halfwidth = 1LL << (n - 1);


    // recursively find the position lies in each quadrant
    // sends (r, c) relatively the sub quadrant
    // [ 11 12 13 14 ]    [ 11 12 | 11 12 ]    [    (r, c)     |    (r, c - half)     ]
    // [ 21 22 23 24 ]    [ 21 22 | 21 22 ]    [ (r - half, c) | (r - half, c - half) ]
    // [ 31 32 33 34 ] - > --------------- 
    // [ 41 42 43 44 ]    [ 11 12 | 11 12 ]
    //                    [ 21 22 | 21 22 ]
    if (r < halfwidth) {
        if (c < halfwidth) {
            // do nothing
            return getVal(n - 1, r, c);
        } else {
            // search (c, r) instead (transpose)
            return getVal(n - 1, c - halfwidth, r);
        }
    } else {
        if (c < halfwidth) {
            // return negative
            return -getVal(n - 1, r - halfwidth, c);
        } else {
            // search (c, r) (transpose) + negative 
            return -getVal(n - 1, c - halfwidth, r - halfwidth);
        }
    }
}

int main() {
    int n, m, u, v, w, p;
    cin >> n >> m;

    mBase.resize(4);
    int width = 1 << n;
    for (size_t i = 0; i < 4; ++i) cin >> mBase[i];

    // Calculate value on call - no matrix created
    while (m--) {
        ll r, c;
        cin >> r >> c;
        cout << getVal(n, r - 1, c - 1) << endl;
    }


}
