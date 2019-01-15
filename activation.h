#ifndef ACTIVATION_H
#define ACTIVATION_H
#include <iostream>
#include<list>
#include<iterator>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include "utility.h"

using namespace std;

void relu(float** matrix,int m,int n);
void tanh(float** matrix,int m,int n);

//relu function
void relu(float** matrix,int m,int n) {
  for(int i=0;i<m;i++) {
    for(int j=0;j<n;j++) {
      if(matrix[i][j]<(float)0) {
        matrix[i][j]=0;
      }
    }
  } 
}

//tanh function
void tanh(float** matrix,int m,int n) {
  for(int i=0;i<m;i++) {
    for(int j=0;j<n;j++) {
      matrix[i][j]=tanh(matrix[i][j]);
    }
  }
}
#endif