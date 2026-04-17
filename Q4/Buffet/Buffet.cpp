#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    int f, w, n;
    cin >> f >> w >> n;

    priority_queue<int, vector<int>, greater<int>> table;
    for (int i = 0; i < f; ++i) {
        int food;
        cin >> food;
        table.push(food);
    }

    if (table.empty()) {
        cout << 0;
        return 0;
    }
    
    int lightCnt = 0;
    int currentReach = -w;
    while (!table.empty()) {
        int food = table.top();
        table.pop();

        if (food > currentReach + w) {
            ++lightCnt;
            currentReach = food + w;
        }
    }

    cout << lightCnt;

}