#include <iostream>
#include <vector>

using namespace std;


vector<int> recurMatrix(int a, int b) {
    
    if (a == 0) {
        return {b};
    }
    
    int width = 1 << a;
    int half = width / 2;

    vector<int> res(width * width);
    vector<int> tl = recurMatrix(a-1, b); // = br
    vector<int> tr = recurMatrix(a-1, b-1);
    vector<int> bl = recurMatrix(a-1, b+1);
    int tl_ptr = 0, tr_ptr = 0, bl_ptr = 0, br_ptr = 0;
    

    for (int i = 0; i < width; ++i) {
        for (int j = 0; j < width; ++j) {
            if (i < half) {
                if (j < half) {
                    res[(i * width) + j] = tl[tl_ptr++];
                } else {
                    res[(i * width) + j] = tr[tr_ptr++];
                }
            } else {
                if (j < half) {
                    res[(i * width) + j] = bl[bl_ptr++];
                } else {
                    res[(i * width) + j] = tl[br_ptr++];
                }
            }
        }
    }

    return res;

}

void printMatrix(vector<int> matrix, int n) {
    if (n == 0) {
        cout << matrix[0];
        return;
    }
    
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            cout << matrix[(i*n) + j] << " ";
        }
        cout << '\n';
    }
}


int main() {
    int a, b;
    cin >> a >> b;
    
    int size = 1 << a;
    vector<int> res = recurMatrix(a, b);
    printMatrix(res, size);
}