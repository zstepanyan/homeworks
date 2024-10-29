#include <iostream>
#include <vector>
#include <algorithm>  
#include <climits>    

using namespace std;

int hourglassSum(vector<vector<int>> arr) {
    int max_sum = INT_MIN;  
    
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            int current_sum = arr[i][j] + arr[i][j+1] + arr[i][j+2]    
                            + arr[i+1][j+1]                           
                            + arr[i+2][j] + arr[i+2][j+1] + arr[i+2][j+2]; 

            max_sum = max(max_sum, current_sum);  
        }
    }
    return max_sum;  
}