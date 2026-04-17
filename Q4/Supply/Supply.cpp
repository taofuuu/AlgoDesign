#include <iostream>
#include <vector>
#include <queue>

using namespace std;

struct Event {
    int day, event, label;
    bool operator>(const Event &other) const {
        return day > other.day;
    }
};

int main() {
    int n, m, k;
    cin >> n >> m >> k;

    priority_queue<Event, vector<Event>, greater<Event>> events;

    for (int day = 1; day <= k; ++day) {
        int d, e, l;
        cin >> d >> e >> l;
        events.push({d, e, l});
    }

    queue<int> plantQueue, storeQueue;

    while (!events.empty()) {
        auto [d, e, l] = events.top();
        events.pop();

        // production
        if (!e) {
            if (!storeQueue.empty()) {
                cout << storeQueue.front() << endl;
                storeQueue.pop();
            } else {
                plantQueue.push(l);
                cout << 0 << endl;
            }
        // store request
        } else {
            if (!plantQueue.empty()) {
                cout << plantQueue.front() << endl;
                plantQueue.pop();
            } else {
                storeQueue.push(l);
                cout << 0 << endl;
            }
        }
    }
}