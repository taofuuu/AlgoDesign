#include <iostream>

using namespace std;

void genBarcode(int a, int b, string barcode) {
    if (a + b == 0) {
        cout << barcode << endl;
        return;
    }

    if (a) {
        genBarcode(a - 1, b, barcode + "1");
    }

    if (b) {
        genBarcode(a, b - 1, barcode + "0");
    } 
}

int main() {
    int a, b;

    cin >> a >> b;

    string barcode = "";
    genBarcode(a, b-a, barcode);
}