#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include<vector>
#include<fstream>

using namespace std;

float*** readMatrix(string filename,int m,int n,int l);
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);
float*** createMatrix(int m,int n,int l);


//read in row major form
float*** readMatrix(string filename,int m,int n,int l) {
  ifstream fin(filename);
  float*** matrix=createMatrix(m,n,l);
  for(int i=0;i<m;i++) {
    for(int j=0;j<n;j++) {
      for(int k=0;k<l;k++) {
        fin>>matrix[i][j][k];
      }
    }
  }
  fin.close();
  return matrix;
}


float** createMatrix(int m,int n) {
    float** matrix=new float*[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new float[n];
    }
    return matrix;
}

float*** createMatrix(int m,int n,int l) {
    float*** matrix=new float**[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new float*[n];
        for(int j=0;j<n;j++) {
          matrix[i][j]=new float[l];
        }
    }
    return matrix;
}


//frees space created for dynamic arrays
void freeSpace(float** matrix,int m) {
    for(int i=0;i<m;i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

#endif