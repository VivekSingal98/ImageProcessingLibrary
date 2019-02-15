#ifndef SAMPLING_H
#define SAMPLING_H
#include <iostream>
#include "utility.h"
using namespace std;

float** maxpool(float** matrix, int m1, int n1, int m2,int n2);
float** avgpool(float** matrix, int m1, int n1, int m2,int n2);
float*** max_pool(float*** inputMatrix, int dim1,int dim2,int dim3, int kernelsize2,int kernelsize3);


#endif