#include <iostream>
#include <vector>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false); cin.tie(0);

    int n;
    cin >> n;

    vector<int> degrees(n + 1, 0);
    int maxDegree = 0;
    for (int i = 1; i <= n; ++i) {
        int degreeSum = 0;
        for (int j = 1; j <= n; ++j) {
            int e;
            cin >> e;
            if (e) degreeSum++;
        }   
        maxDegree = max(maxDegree, degreeSum);
        degrees[degreeSum]++;
    }

    for (int i = 0; i <= maxDegree; ++i) cout << degrees[i] << " ";
}