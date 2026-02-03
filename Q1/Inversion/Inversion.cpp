#include <iostream>
#include <vector>

using namespace std;

// mergesort
int mergeAndCount(vector<int> &arr, int left, int mid, int right) {
    vector<int> leftSub(arr.begin() + left, arr.begin() + mid + 1);
    vector<int> rightSub(arr.begin() + mid + 1, arr.begin() + right + 1);

    int i = 0;
    int j = 0;
    int k = left;
    int inversion_count = 0;

    while (i < leftSub.size() && j < rightSub.size()) {
        if (leftSub[i] <= rightSub[j]) { // ordered normally A[i] < A[j]
            arr[k++] = leftSub[i++]; 
        } else { // ordered inversively A[i] > A[j]
            arr[k++] = rightSub[j++];
            inversion_count += (leftSub.size() - i); // other leftSub is larger than rightSub[j++]
        }
    }

    // fill left over sub array
    while (i < leftSub.size()) arr[k++] = leftSub[i++];
    while (j < rightSub.size()) arr[k++] = rightSub[j++];

    return inversion_count;
}

int mergeSortAndCount(vector<int> &arr, int left, int right) {
    int count = 0;
    if (left >= right) {
        return count;
    }

    int mid = left + (right - left) / 2;

    count += mergeSortAndCount(arr, left, mid); // merge left
    count += mergeSortAndCount(arr, mid + 1, right); // merge right
    count += mergeAndCount(arr, left, mid, right); // sort split
    
    return count;
}

int main() {
    int n;
    cin >> n;

    vector<int> arr; arr.reserve(n);
    for (size_t i = 0; i < n; ++i) {
        int x; 
        cin >> x;
        arr.push_back(x);
    }

    cout << mergeSortAndCount(arr, 0, n - 1) << endl;
}