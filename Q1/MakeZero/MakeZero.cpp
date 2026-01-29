#include <iostream>
#include <queue>

using namespace std;

size_t makeZero(int x) {
    if (!x) return 0;

    size_t min = 0;
    int val1, val2;
    queue<int> q;
    q.push(x);
    while (!q.empty()) {
        size_t levelSize = q.size();
        min++;
        for (size_t i = 0; i < levelSize; ++i) {
            int x = q.front();
            q.pop();
            
            val1 = (x + 1) % 16777216;
            val2 = (x * 2) % 16777216;
            if (val1 == 0 || val2 == 0) {
                return min;
            }
            q.push(val1);
            q.push(val2);
        }
    }
    
}

int main() {
    int x;
    cin >> x;
    cout << makeZero(x);
}