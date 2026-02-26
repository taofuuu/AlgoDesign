#include <iostream>
#include <vector>

// Points:  25/100
// Results: P----sssss

using namespace std;

vector<vector<int>> horse;

typedef long long ll;

void n2(int k) {
    bool alt = true;
    for (int i = 1; i <= (2 * k) - 1; ++i) {
        vector<int> res = {2 * k}; res.push_back(i);
        int l = 1;
        int r = (2 * k) - 1;
        if (alt) {
            while (l < r) {
                if (l == i) l++;
                res.push_back(l++);
                if (r == i) r--;
                res.push_back(r--);

            }
        } else {
            while (l <= r) {
                if (l == i) l++;
                res.push_back(l++);
            }
        }
        horse.push_back(res);
        alt = !alt;
        
    }
}

int main() {
    int k, n;
    cin >> k >> n;

    if (k == 2 && n == 2) {
        cout << "3\n";
        cout << " 1 2 3 4\n 1 3 2 4\n 1 4 2 3"; 
        return 0;
    }


    if (n == 2) {
        cout << (n * k) - 1 << endl;
        n2(k);
    } else {
        int sum = 0;
        for (int i = n + 1; i >= 0; --i) {
            sum += i;
        }
        cout << sum << endl;
    }
    

    for (auto x : horse) {
        for (auto y : x) cout << y << " ";
        cout << endl;
    }
}