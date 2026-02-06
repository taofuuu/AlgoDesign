#include <iostream>
#include <vector>

using namespace std;

vector<int> v;

// plus vector
vector<int> combineVector(vector<int> v1, vector<int> v2, bool addition) {
    vector<int> res(v1.size());
    for (size_t i = 0; i < v1.size(); ++i) {
        if (addition) {
            res[i] = v1[i] + v2[i];
        } else {
            res[i] = v1[i] - v2[i];
        }
    }
    return res;
}
// KEY IDEA:
//
// Hn * v = [ Hn/2 |  Hn/2 ] * (v1, v2, ... , vn)
//          [ Hn/2 | -Hn/2 ]
//        = [ Hn/2 * vn/2 +  Hn/2 * vn/2 ]
//          [ Hn/2 * vn/2 -  Hn/2 * vn/2 ] (sub-problem)
//
// @Base case (n == 2);
//  = [1 |  1] * (v1, v2)
//    [1 | -1]
//  = [v1 + v2]
//    [v1 - v2]

vector<int> hadamard(int n, int vl, int vr) {
    if (n == 1) {
        return {v[vl]};
    }
    
    if (n == 2) {
        return {v[vl] + v[vr], 
                v[vl] - v[vr]};
    }
    
    // spliting front/back for vector
    int vm = (vl + vr) / 2;

    vector<int> front = hadamard(n/2, vl, vm);
    vector<int> back = hadamard(n/2, vm + 1, vr);

    vector<int> res; res.reserve(2 * front.size());
    vector<int> top = combineVector(front, back, true);
    vector<int> bottom = combineVector(front, back, false);

    // put top and bottom vectors into the result vector
    res.insert(res.end(), top.begin(), top.end());
    res.insert(res.end(), bottom.begin(), bottom.end());

    return res;
}

int main() {
    int n;
    cin >> n;
    
    v.resize(n);
    for (size_t i = 0; i < n; ++i) {
        cin >> v[i];
    }

    vector<int> result = hadamard(n, 0, n - 1);

    for (auto x : result) cout << x << " ";
}