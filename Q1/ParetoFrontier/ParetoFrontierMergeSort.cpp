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
    if (lhs.x != rhs.x) return lhs.x < rhs.x;
    return lhs.y < rhs.y;
}

vector<int> pareto(int l, int r) {
    if (l == r) {
        return {l};
    }

    int mid = (l + r) / 2;

    // 'a' gets cotrolled: ∃b {a.x <= b.x && a.y <= b.y}
    // -> 'a' not cotrolled: ∀b {a.x > b.x || a.y > b.y}
    //
    // |     x
    // | o  o     x
    // |     o    o     x
    // |  o      o   o       x
    // |    o         o   o      x
    // |  o    o   o        o      x
    // |____________________________
    //
    // new pareto -> pareto of l and r
    vector<int> lPareto = pareto(l, mid);
    vector<int> rPareto = pareto(mid + 1, r);

    vector<int> result;

    // Process RIGHT Subarray (higher x, a.x > b.x)
    // - have higher X, CANNOT be dominated by the left side
    // - keep ALL of them (they are safe)
    // - 'maxRightY' to set what left side must beat
    // - pushing first => sorted by X descending
    // - although only first entry from RIGHT pass ∀b {a.x > b.x},
    //   but other also pass from ∀b {a.y > b.y} (from prev conquer node)
    int maxRightY = INT_MIN;
    for (int i : rPareto) {
        maxRightY = max(maxRightY, points[i].y);
        result.push_back(i);
    }

    // 1 2 3 4 5 6 7 8
    // 1 2 3 4|5 6 7 8 
    // 1 2|3 4|5 6|7 8
    // 1|2|3|4|5|6|7|8 <- start here
    // 2 1|4 3|6 5|8 7
    // 4 3 2 1|8 7 6 5
    // 8 7 6 5 4 3 2 1

    // Process LEFT Subarray (lower x, can be frontier *if p.y > max Y from right*)
    // - compare for MAX y to pass ∀b {a.y > b.y}
    // - putting greater y later => sorted by Y Ascending
    for (int i : lPareto) {
        if (points[i].y > maxRightY) {
            result.push_back(i);
        }
    }

    return result;
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

    cout << pareto(0, n-1).size();

    
}