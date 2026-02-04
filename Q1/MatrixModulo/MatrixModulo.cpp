#include <iostream>
#include <vector>

using namespace std;

vector<long long> res = {1, 0, 0, 1};
int n, k;

// 0 1
// 2 3

vector<long long> matrixMultMod (const vector<long long> &A, const vector<long long> &B, int k) {
    vector<long long> result; result.resize(4);

    result[0] = (A[0]*B[0] + A[1]*B[2]) % k;
    result[1] = (A[0]*B[1] + A[1]*B[3]) % k;
    result[2] = (A[2]*B[0] + A[3]*B[2]) % k;
    result[3] = (A[2]*B[1] + A[3]*B[3]) % k;

    return result;
}

int main() {
    cin >> n >> k;

    vector<long long> base(4);
    for (size_t i = 0; i < 4; ++i) cin >> base[i]; 

    // n = 13 = b'1101
    // starting from LSB (right)
    // 1 -> res * base (1 * A)      | square A -> A^2   | res = A
    // 0 -> don't take              | square A -> A^4   | res = A
    // 1 -> res * base (A * A^4)    | square A -> A^8   | res = A^5
    // 1 -> res * base (A^5 * A^8)  | square A -> A^16  | res = A^13


    while (n > 0) {
        if (n % 2 == 1) { // LSB == 1
            // multiply to res
            res = matrixMultMod(res, base, k);
        }
        // square the multiplier (switching to a higher significant bit)
        base = matrixMultMod(base, base, k);
        n /= 2;
    }

    for (size_t i = 0; i < 4; ++i) cout << res[i] << " ";
}