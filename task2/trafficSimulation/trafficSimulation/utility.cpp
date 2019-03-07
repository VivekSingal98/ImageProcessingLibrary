#include "utility.hpp"

void printMap(map<string, float> mapVar) {
    for (auto const& x : mapVar)
    {
        cout << x.first  // string (key)
        << ':'
        << x.second // string's value
        << endl ;
    }
}
int** createMatrix(int m,int n) {
    int** matrix=new int*[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new int[n];
        for(int j=0;j<n;j++){
            matrix[i][j] = 0;
        }
        
    }
    return matrix;
}
