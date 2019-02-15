#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include<vector>
#include<fstream>
using namespace std;

void printMatrix(float** matrix,int m,int n);
float** createMatrix(int m,int n);
float*** createMatrix(int depth,int height,int width);
float** readMatrix(string filename,int m,int n);
float*** readMatrix(string filename,int m,int n,int l);
void writeMatrix(string filename,float** matrix,int m,int n);
void writeMatrix(string filename,float*** matrix,int dim1,int dim2,int dim3);
void freeSpace(float** matrix,int m);
float*** imageTo3dMatrix(string input_file);
void freeSpace(float** matrix,int m);


#endif