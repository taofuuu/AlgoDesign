#include <iostream>
#include <vector>

using namespace std;

int main() {
    int f;
    cin >> f;

    vector<int> fibo(f + 1);
    fibo[1] = 1;
    fibo[2] = 1;
    
    for (int i = 3; i <= f; ++i) {
        fibo[i] = fibo[i - 1] + fibo[i - 2];
    }

    cout << fibo[f];
}