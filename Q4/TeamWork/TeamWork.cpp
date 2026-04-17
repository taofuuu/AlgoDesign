#include <iostream>
#include <vector>
#include <queue>
#include <iomanip>

using namespace std;

int main() {
    int n, m;
    cin >> n >> m;

    priority_queue<int, vector<int>, greater<int>> subtask;
    for (int i = 0; i < m; ++i) {
        int s;
        cin >> s;
        subtask.push(s);
    }

    priority_queue<int, vector<int>, greater<int>> stream; // (timestamp, studentId)

    for (int i = 0; i < n; ++i) {
        stream.push(subtask.top());
        subtask.pop();
    }

    int timer = 0;
    double totalInterval = 0;
    while (!stream.empty()) {
        int t = stream.top();
        stream.pop();
        timer = t;
        totalInterval += t;

        if (!subtask.empty()) {
            stream.push(timer + subtask.top());
            subtask.pop();
        }
    }

    cout << fixed << setprecision(3) << totalInterval / (double) m;

}