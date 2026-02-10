#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <climits>

using namespace std;

struct Point {
    int x, y;
};

bool compareX(const Point &a, const Point &b) {
    if (a.x != b.x) return a.x < b.x;
    return a.y < b.y;
}

bool compareY(const Point &a, const Point &b) {
    return a.y < b.y;
}

int distSquared(Point p1, Point p2) {
    return (p1.x - p2.x) * (p1.x - p2.x) + 
           (p1.y - p2.y) * (p1.y - p2.y);
}

// Brute force for stopping recursion
int bruteForce(const vector<Point>& P, int n) {
    int minDistance = INT_MAX;
    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            minDistance = min(minDistance, distSquared(P[i], P[j]));
        }
    }
    return minDistance;
}

// Find closest distance in the strip
int stripClosest(vector<Point>& strip, int stripWidth) {
    int minDistance = stripWidth;
    sort(strip.begin(), strip.end(), compareY); // Sort strip by Y

    for (size_t i = 0; i < strip.size(); ++i) {
        for (size_t j = i + 1; j < strip.size(); ++j) {
            // check Y distance is in the strip (Y-axis)
            // if this strip[i] Y-diff is alr >= strip[j] -> skip others
            // (gonna be >= anyways, cuz it sorted)
            if ((strip[j].y - strip[i].y) * (strip[j].y - strip[i].y) >= minDistance) {
                break;
            }
            minDistance = min(minDistance, distSquared(strip[i], strip[j]));
        }
    }
    return minDistance;
}

int closestUtil(vector<Point>& P, int l, int r) {
    // If few points, use brute force
    if (r - l + 1 <= 3) {
        vector<Point> temp;
        for(int i=l; i<=r; ++i) temp.push_back(P[i]);
        return bruteForce(temp, temp.size());
    }

    int mid = (l + r) / 2;
    Point midPoint = P[mid];

    int dl = closestUtil(P, l, mid);
    int dr = closestUtil(P, mid + 1, r);
    int d = min(dl, dr);

    // select point in strip
    vector<Point> strip;
    for (int i = l; i <= r; i++) {
        // use x*x for distance calc
        int dx = P[i].x - midPoint.x;
        if (dx * dx < d) {
            strip.push_back(P[i]);
        }
    }

    return min(d, stripClosest(strip, d));
}

int main() {
    int n;
    if (!(cin >> n)) return 0;

    vector<Point> P(n);
    for (int i = 0; i < n; i++) {
        cin >> P[i].x >> P[i].y;
    }

    sort(P.begin(), P.end(), compareX);

    cout << closestUtil(P, 0, n - 1) << endl;

    return 0;
}