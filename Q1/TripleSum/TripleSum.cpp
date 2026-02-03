#include <iostream>
#include <vector>

using namespace std;

vector<long long> v;
int n;

void findNum(int x) {
    long long sum;
    for (size_t i = 0; i < n - 2; ++i) { // i - anchor
        // sub-problem - two-sum (2 pointer)
        size_t l_ptr = i + 1;
        size_t r_ptr = n - 1;

        while (l_ptr < r_ptr) {
            sum = v[l_ptr] + v[r_ptr] + v[i];
            if (sum == x) {
                cout << "YES\n";
                return;
            } else if (sum < x) { // increase value (left side)
                l_ptr += 1;
            } else { // decrease value (right side)
                r_ptr -= 1;
            }
        }

    }
    cout << "NO\n";
    return;
}

int main() {
    int m;
    long long x;

    cin >> n >> m;
    v.reserve(n);

    for (size_t i = 0; i < n; ++i) {
        cin >> x;
        v.push_back(x);
    }

    while (m--) {
        cin >> x;
        findNum(x);
    }

}