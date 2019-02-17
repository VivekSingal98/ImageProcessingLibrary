#include "sampling.h"

//m1,n1 is the size of input matrix. m2,n2 is the size of kernel of the maxpool
float** maxpool(float** matrix, int m1, int n1, int m2,int n2){
  if(m1%m2!=0 || n1%n2!=0) {
    cerr<<"kernel dimention not divisible by matrix. Can't do maxpool";
    exit(1);
  }
  int m_afterPool=m1/m2;
  int n_afterPool=n1/n2;
  float** matrix_afterPool=createMatrix(m_afterPool,n_afterPool);
  for(int i=0;i<m_afterPool;i++) {
    for(int j=0;j<n_afterPool;j++) {
      float elem=matrix[i*m2][j*n2];
      for(int k=0;k<m2;k++) {
        for(int l=0;l<n2;l++) {
          if(elem<matrix[i*m2+k][j*n2+l]) {
            elem=matrix[i*m2+k][j*n2+l];
          }
        }
      }
      matrix_afterPool[i][j]=elem;
    }
  }
  return matrix_afterPool;
} 

//m1,n1 is the size of input matrix. m2,n2 is the size of kernel of the avgpool
float** avgpool(float** matrix, int m1, int n1, int m2,int n2){
  if(m1%m2!=0 || n1%n2) {
    cerr<<"kernel dimention not divisible by matrix. Can't do maxpool";
    exit(1);
  }
  int m_afterPool=m1/m2;
  int n_afterPool=n1/n2;
  float** matrix_afterPool=createMatrix(m_afterPool,n_afterPool);
  for(int i=0;i<m_afterPool;i++) {
    for(int j=0;j<n_afterPool;j++) {
      float elem=0;
      for(int k=0;k<m2;k++) {
        for(int l=0;l<n2;l++) {
          elem+=matrix[i*m2+k][j*n2+l];
        }
      }
      matrix_afterPool[i][j]=elem/(m2*n2);
    }
  }
  return matrix_afterPool;
}


//maxpool for 3d matrix using previously made maxpool function
float*** max_pool(float*** inputMatrix, int dim1,int dim2,int dim3, int kernelsize2,int kernelsize3){
  float*** pool = new float**[dim1];
  for(int i=0;i<dim1;i++){
    pool[i] = maxpool(inputMatrix[i],dim2,dim3,kernelsize2,kernelsize3);
  }
  return pool;  
}