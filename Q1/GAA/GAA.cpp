#include <iostream>
#include <vector>

using namespace std;

vector<int> lengths;


// G(i) = G(i-1) + "ga...a" (a = i + 2) + G(i-1)
// G(i) = 2 * G(i-1) + (i + 3)
void getLengths(int n) {
    int current = 3; // G(0) = "gaa" = 3
    lengths.push_back(current);
    
    // generate lengths, until passes over n (n lies in here)
    int i = 1;
    while (current < n) {
        current = 2 * current + (i + 3);
        lengths.push_back(current);
        ++i;
    }
}

char gaa(int n) {
    int k = lengths.size() - 1;
    
    while (k >= 0) {
        if (k == 0) {
            if (n == 1) return 'g';
            else return 'a';
        }
        
        int midLength = k + 3;
        // go into the subsequence
        int prevLength = lengths[k - 1];
        
        // n in first part - iterate into the subsequence
        if (n <= prevLength) {
            --k;
            
        // n in middle part - remove first part, then answer
        } else if (n <= prevLength + midLength) {
            // left with middle part
            int offset = n - prevLength;
            if (offset == 1) return 'g';
            else return 'a';
        
        // n in right part - remove offset (first part + middle part), then iterate into the subsequence
        } else {
            n -= (prevLength + midLength);
            --k;
        }
        
    }
}

int main() {
    int n;
    cin >> n;
    
    getLengths(n);

    cout << gaa(n);
}