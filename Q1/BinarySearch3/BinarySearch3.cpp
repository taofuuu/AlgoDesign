#include <iostream>
#include <vector>

using namespace std;

vector<int> bs;
int n;

int main() {
    int m;
    cin >> n >> m;
    
    bs.resize(n);
    for (size_t i = 0; i < n; ++i) cin >> bs[i];

    while (m--) {
        int querry;
        cin >> querry;

        int ans = -1;
        int l = 0;
        int r = n - 1;

        while (l <= r) {
            int mid = (l + r) / 2;
            
            if (bs[mid] <= querry) {
                ans = mid;
                l = mid + 1;
            } else {
                r = mid - 1;
            }

        }

        cout << ans << endl;
    }
}