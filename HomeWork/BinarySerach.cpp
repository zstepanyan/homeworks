#include <iostream>
#include <vector>

using namespace std;


int BinarySearch(const vector<int>& arr, int left, int right, int target) {
    while (left <= right) {
        int mid = left + (right - left) / 2;  

       
        if (arr[mid] == target)
            return mid;

        
        if (arr[mid] > target)
            right = mid - 1;
        
        else
            left = mid + 1;
    }

    return -1;
}
