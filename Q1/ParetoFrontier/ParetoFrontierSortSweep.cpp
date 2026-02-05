#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>

using namespace std;

struct Point {   
    int x, y;
};

vector<Point> points;

bool comparePoints(const Point &lhs, const Point &rhs) {
    if (lhs.x != rhs.x) return lhs.x > rhs.x;
    return lhs.y > rhs.y;
}

int main() {
    int n;
    cin >> n;

    points.resize(n);
    for (size_t i = 0; i < n; ++i) {
        Point a;
        cin >> a.x >> a.y;
        points[i] = a;
    }

    sort(points.begin(), points.end(), comparePoints);

    int frontier = 0;
    int maxY = INT_MIN;

    for (const auto &p : points) {
        if (p.y > maxY) {
            maxY = p.y;
            ++frontier;
        }
    }

    cout << frontier;
}