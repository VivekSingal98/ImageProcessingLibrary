#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include <iostream>
#include <fstream>
#include "utility.h"
#include <cblas.h>
using namespace std;

float**** createConvWeightMatrix(int C_new,int C_prev,int m,int n);
void readConvWeightsAndBias(string filename,int C_new,int C_prev,int m,int n,float**** convWeights,float* bias);
void matrix_multiplication_openblas(float** matrix1,int m1,int n1,float* matrix2,int m2,float* matrix3);
float** matrix_convolution_with_mult(float** matrix,int m1,int n1,float** kernel,int m2,int n2,string method);
float*** convolution(float*** inputMatrix, int dim1,int dim2,int dim3, string kerneltextfile, int channels, int height,int width,int relu);


#endif