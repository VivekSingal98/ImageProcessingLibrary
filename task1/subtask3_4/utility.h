#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include<vector>
#include<fstream>
#include <cblas.h>

using namespace std;

void printMatrix(float** matrix,int m,int n);
float*** readMatrix(string filename,int m,int n,int l);
void writeMatrix(string filename,float** matrix,int m,int n);
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);
float*** createMatrix(int m,int n,int l);
float** matrix_convolution_with_mult(float** matrix,int m1,int n1,float** kernel,int m2,int n2);


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


//for debugging purpose
void printMatrix(float** matrix,int m,int n) {
    for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
            cout<<matrix[i][j]<<" ";
        }
    }
    cout<<"\n";
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

//write in column major form
void writeMatrix(string filename,float** matrix,int m,int n) {
  ofstream fout(filename);
  for(int j=0;j<n;j++) {
    for(int i=0;i<m;i++) {
      fout<<matrix[i][j]<<" ";
    }
  }
  fout.close();
}

//frees space created for dynamic arrays
void freeSpace(float** matrix,int m) {
    for(int i=0;i<m;i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


//multipies two matrices using openblas
void matrix_multiplication_openblas(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2,float* matrix3) {
  if(n1!=m2) {
    cerr<<"matrixes not compatible for multiplication";
    exit(1);
  }
  //making column major array from original matrices
  float* col_major_matrix1 = new float[m1*n1];
  float* col_major_matrix2 = new float[m2*n2];
  for(int i=0;i<n1;i++){
    for(int j=0;j<m1;j++){
      col_major_matrix1[i*m1+j] = matrix1[j][i];
    }
  }
  for(int i=0;i<n2;i++){
    for(int j=0;j<m2;j++){
      col_major_matrix2[i*m2+j] = matrix2[j][i];
    }
  } 
  //calling the matrix multiplication function in openblas
  cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m1, n2, n1, 1, col_major_matrix1, m1, col_major_matrix2, n1, 0, matrix3, m1);

}

//adds padding to the matrix
float** addPadding(float** matrix,int m,int n,int p) {
  int m1=m+2*p;
  int n1=n+2*p;
  float** matrix_padded=createMatrix(m1,n1);
  for(int i=0;i<p;i++) {
    for(int j=0;j<n1;j++) {
      matrix_padded[i][j]=0;
      matrix_padded[m1-i-1][j]=0;
    }
  }
  for(int i=p;i<m1-p;i++) {
    for(int j=0;j<p;j++) {
      matrix_padded[i][j]=0;
      matrix_padded[i][n1-j-1]=0;
    }
    for(int j=p;j<n1-p;j++) {
      matrix_padded[i][j]=matrix[i-p][j-p];
    }
  }

  return matrix_padded;
}

//convolution of two matrices(actually cross correlation) with multiplication
float** matrix_convolution_with_mult(float** matrix,int m1,int n1,float** kernel,int m2,int n2) {
  if(m1<m2 || n1<n2) {
    cerr<<"kernel bigger than matrix. Can't convolve";
    exit(1);
  }
  int m_convolveMatrix=(m1-m2+1);
  int n_convolveMatrix=(n1-n2+1);
  //converting matrix to Toeplitz form
  int n_Toeplitz=m2*n2;
  int m_Toeplitz=m_convolveMatrix*n_convolveMatrix; 
  float** matrix_ToeplitzForm=createMatrix(m_Toeplitz,n_Toeplitz);
  int index=0;
  for(int i=0;i<m_convolveMatrix;i++) {
    for(int j=0;j<n_convolveMatrix;j++) {
      int k=0;
      for(int i1=i;i1<i+m2;i1++) {
        for(int j1=j;j1<j+n2;j1++) {
          matrix_ToeplitzForm[index][k++]=matrix[i1][j1];
        }
      }
      index++;
    }
  }
  
  //converting kernel matrix to column matrix
  float** kernel_ColumnMatrix=createMatrix(m2*n2,1);
  index=0;
  for(int i=0;i<m2;i++) {
    for(int j=0;j<n2;j++) {
      kernel_ColumnMatrix[index][0]=kernel[i][j];
      index++;
    }
  }
  float** matrix_conv; 
  matrix_conv=createMatrix(m_convolveMatrix,n_convolveMatrix);

  //making an array to store multiplication result
  float* row_major_array = new float[m_Toeplitz];
  //calling the multiplication function which use openbas
  matrix_multiplication_openblas(matrix_ToeplitzForm,m_Toeplitz,n_Toeplitz,kernel_ColumnMatrix,m2*n2,1,row_major_array);
  //making final convolution matrix from the row major array
  for(int i=0;i<m_convolveMatrix;i++){
    for(int j=0;j<n_convolveMatrix;j++){
      matrix_conv[i][j] = row_major_array[i*n_convolveMatrix+j];
    }
  }  
  delete[] row_major_array;
  

  freeSpace(matrix_ToeplitzForm,m_Toeplitz);
  freeSpace(kernel_ColumnMatrix,m2*n2);
  
  return matrix_conv;
}



#endif