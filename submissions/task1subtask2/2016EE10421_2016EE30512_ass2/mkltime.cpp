#include<iostream>
#include <cstdlib>
#include<vector>
#include <chrono>
#include<fstream>
#include <mkl.h>

using namespace std;
using namespace std::chrono;
float* createMatrix(int m,int n);
void writeMatrix(string filename,float* matrix,int m);
float* matrix_multiplication_mkl(float* matrix1,int m1,int n1,float* matrix2,int m2,int n2);


//creates a random m*n matrix along 1 dimention
float* createMatrix(int m,int n) {
    float* matrix=new float[m*n];

    for(int i=0;i<m;i++) {
      for(int j=0;j<n;j++) {
        float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100));
        matrix[i*n+j]=r;
      }
    }
    return matrix;
}

//write array to a file
void writeArray(string filename,float* matrix,int m) {
  ofstream fout(filename);
  for(int i=0;i<m;i++) {
    fout<<matrix[i];
    fout<<"\n";
  }
  fout.close();
}

//multipies two matrices using openblas
float* matrix_multiplication_mkl(float* matrix1,int m1,int n1,float* matrix2,int m2,int n2) {
  if(n1!=m2) {
    cerr<<"matrixes not compatible for multiplication";
    exit(1);
  }
  float* matrix3= new float[m1*n2];
  //calling the matrix multiplication function in openblas
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1, n2, n1, 1, matrix1, n1, matrix2, n2, 0, matrix3, n2);
  return matrix3;
}

int main() {

  int noofvalues=100;  //will test time taken by an operation 100 times
  int kernel_m=200;    //kernel size is fixed to be 200*200 
  int kernel_n=200;
  vector<int> sizes{10,50,100,500,1000};  //matrix size is 10*200,50*200,100*200,500*200,1000*200
  
  float* kernel=createMatrix(kernel_m,kernel_n);  //kernel of 200*200 matrix with random numbers generated
  float* timeMatrix=new float[noofvalues]; //time taken will be stored in file

  
  for(int k=0;k<sizes.size();k++) {

    for(int i=0;i<noofvalues;i++) {

      float* matrix=createMatrix(sizes[k],kernel_m);
      high_resolution_clock::time_point t1 = high_resolution_clock::now();
      float* result=matrix_multiplication_mkl(matrix,sizes[k],kernel_m,kernel,kernel_m,kernel_n);
      high_resolution_clock::time_point t2 = high_resolution_clock::now();
      auto time_span = duration_cast<microseconds>( t2 - t1 );
      timeMatrix[i]=(float)time_span.count();
      delete[] matrix;
    }

    //writing to file
    string concat="mkl"+to_string(sizes[k])+".txt";
    writeArray("./timefiles/"+concat,timeMatrix,noofvalues);
  }
  delete[] kernel;
  delete[] timeMatrix;
}

