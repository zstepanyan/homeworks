#include <iostream>
#include <vector>

using namespace std;


vector<int> rotLeft(vector<int> a, int d) {
    int n = a.size();
    vector<int> rotated(n);
    
    
    for (int i = 0; i < n; i++) {
        rotated[i] = a[(i + d) % n];  
    }

    return rotated;
}