#include <iostream>
#include <string>

using namespace std;

void consecOnes(int k, int a, int count, string bitString, bool consec) {
    if (!a) {
        if (consec)
            cout << bitString << endl;
            return;
    }

    // Gen 0
    consecOnes(k, a - 1, 0, bitString + '0', consec);

    // Gen 1
    consecOnes(k, a - 1, count + 1, bitString + '1', consec || count + 1 >= k);

}

int main() {
    int a, k;
    cin >> a >> k;
    consecOnes(k, a, 0, "", false);
}
