#include <iostream>
#include <vector>

using namespace std;

int totalCount = 0;
int N;

void generateSequence(int n, int back) {
    if (n == N) {
        ++totalCount;
        return;
    }

    for (int i = back; i <= N; ++i) {
        if (n + i <= N) {
            generateSequence(n + i, i);
        }
    }
}

int main() {
    cin >> N;

    generateSequence(0, 1);

    cout << totalCount;
}