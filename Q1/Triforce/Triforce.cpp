#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

vector<int> triforce;
vector<vector<int>> prefixSum;
int n;

int getSum(int top, int left, int bottom, int right) {
    int result = prefixSum[bottom][right];

    if (top) result -= prefixSum[top-1][right];
    if (left) result -= prefixSum[bottom][left-1];
    if (top && left) result += prefixSum[top-1][left-1];

    return result;
}

int checkDup(int a, int b, int c) {
    if (!a || !b || !c) return 0;

    if (a == b) return a;
    if (b == c) return b;
    if (a == c) return a;

    return 0;
}

int checkType(int r, int c, int size) {
    // quadrants size
    int half = size / 2;

    // bottom-right quad coords
    int brTop = r + half;
    int brLeft = c + half;
    int brBottom = r + size - 1;
    int brRight = c + size - 1;

    // if sum > 0, there's non-zero
    if (getSum(brTop, brLeft, brBottom, brRight)) return 0;

    // base case
    if (size == 2) {
        int topLeft = triforce[r * n + c];
        int topRight = triforce[r * n + (c + half)];
        int bottomLeft = triforce[(r + half) * n + c];
        return checkDup(topLeft, topRight, bottomLeft);
    }

    int p = checkType(r, c, half);            // Top-Left
    int q = checkType(r, c + half, half);     // Top-Right
    int s = checkType(r + half, c, half);     // Bottom-Left

    return checkDup(p, q, s);
}

int main() {
    for (size_t num = 0; num < 3; ++num) {
        cin >> n;

        int size = n * n;
        triforce.resize(size);

        // assign empty prefixSum
        prefixSum.assign(n, vector<int>(n));

        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                cin >> triforce[(i * n) + j];
                
                // Prefix Sum - cumulative sum of a rectangle from (0,0) to (i,j)
                // Definition (1D): PS[i] = PS[0] + PS[1] + ... + PS[i]
                // |-> Implementation - 2D: PS[i][j] = Value[i][j] + top + left - topLeft
                //          j
                //   [tl tl t _]
                //   [tl tl t _]
                // i [l  l  x _] 
                //   [_  _  _ _]
                //
                // Usage (1D): sum(L, R) = PS[R] - PS[L-1]
                // |-> Implementation - 2D: sum(top, left, bottom, right) = PS[bottom][right] - PS[top-1][right] - PS[bottom][left-1] + PS[top-1][left-1]
                //
                // PS[bottom][right] => big box (0,0) to (bottom, right)
                // PS[top-1][right] => top strip (everything above top)
                // PS[bottom][left-1] => left strip (everything before left)
                // PS[top-1][left-1] => top-left intersection (deleted 2 times (top+left strip))

                prefixSum[i][j] = triforce[i * n + j];
                // if not left edge, + left
                if (i) prefixSum[i][j] += prefixSum[i - 1][j];
                // if not top edge, + top
                if (j) prefixSum[i][j] += prefixSum[i][j - 1];
                // if no edge, - intersection (top-left)
                if (i && j) prefixSum[i][j] -= prefixSum[i - 1][j - 1];
            }
        }

        cout << checkType(0, 0, n) << endl;
    }
}
