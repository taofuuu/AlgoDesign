#include <iostream>
#include <vector>

using namespace std;

int totalCount = 0;
int N;

vector<bool> colQueen; // track col
vector<bool> diagLR; // track topleft-bottomright diagonol (top index)
vector<bool> diagRL; // track topright-bottomleft diagonal (top index)

//   -3 -2 -1   0  1  2  3 => total size (2*n - 1)
// 0  a  b  c | d  e  f  g <- track this
// 1     a  b | c  d  e  f
// 2        a | b  c  d  e
// 3          | a  b  c  d
// top index: row - col (+n for offset negative number) | row + col

void generateConfiguration(int row) {
    if (row == N) {
        totalCount++;
        return;
    }

    for (int col = 0; col < N; ++col) {
        int dlr = row - col + N;
        int drl = row + col;

        if (!colQueen[col] && !diagLR[dlr] && !diagRL[drl]) {
            colQueen[col] = diagLR[dlr] = diagRL[drl] = true;
            
            generateConfiguration(row + 1);
            
            // back tracking
            colQueen[col] = diagLR[dlr] = diagRL[drl] = false;
        }
    }

    
}

int main() {
    cin >> N;

    colQueen.resize(N);
    diagLR.resize(N * 2);
    diagRL.resize(N * 2);

    generateConfiguration(0);

    cout << totalCount;
}