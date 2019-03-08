
#ifndef utility_hpp
#define utility_hpp

#include <stdio.h>
#include <map>
#include <iostream>
using namespace std;

void printMap(map<string, float> mapVar);
int** createMatrix(int m,int n);
void setZeros(int** matrix,int m,int n);
void printMatrix(int** matrix,int m,int n);
#endif /* utility_hpp */
