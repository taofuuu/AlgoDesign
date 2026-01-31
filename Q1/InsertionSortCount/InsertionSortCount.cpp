#include <iostream>
#include <vector>
#include <map>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;
    vector<int> v(n);
    for (int i = 0; i < n; ++i) cin >> v[i];
    
    for (int i = 0; i < m; ++i) {
        int query;
        cin >> query;
        
        int pos = 0;
        int count = 0;
        while (v[pos] != query) {
            if (v[pos++] > query) count++;
        }
        cout << count << endl;
    }

}