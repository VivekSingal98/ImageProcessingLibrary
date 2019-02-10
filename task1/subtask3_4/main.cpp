#include<iostream>
#include<string>
#include<fstream>
#include <cblas.h>
#include "utility.h"

float**** createConvWeightMatrix(int C_new,int C_prev,int m,int n) {
  float**** matrix=new float***[C_new];
  for(int i=0;i<C_new;i++) {
      matrix[i]=new float**[C_prev];
      for(int j=0;j<C_prev;j++) {
        matrix[i][j]=new float*[m];
        for(int k=0;k<m;k++) {
          matrix[i][j][k]=new float[n];
        }
      }
  }
  return matrix;
}

void readConvWeightsAndBias(string filename,int C_new,int C_prev,int m,int n,float**** convWeights,float* bias) {
  ifstream fin(filename); 
  for(int c_new=0;c_new<C_new;c_new++) {
    for(int c_prev=0;c_prev<C_prev;c_prev++) {
      for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
          fin>>convWeights[c_new][c_prev][i][j];
        }
      }
    }
  }
  for(int i=0;i<C_new;i++) {
    fin>>bias[i];
  }
  fin.close();  
}

//multipies two matrices using openblas
void matrix_multiplication_openblas(float** matrix1,int m1,int n1,float* matrix2,int m2,float* matrix3) {
 
  //making row major array from original matrices
  float* row_major_matrix1 = new float[m1*n1];
  
  for(int i=0;i<m1;i++){
    for(int j=0;j<n1;j++){
      row_major_matrix1[i*n1+j] = matrix1[i][j];
    }
  }

  //calling the matrix multiplication function in openblas
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1, 1, n1, 1, row_major_matrix1, n1, matrix2, 1, 0, matrix3, 1);
}

//convolution of two matrices(actually cross correlation) with multiplication
float** matrix_convolution_with_mult(float*** matrix,int c_matrix,int m1,int n1,float*** kernel,int c_kernel,int m2,int n2,float bias) {
  if(m1<m2 || n1<n2 || c_matrix!=c_kernel) {
    cerr<<"kernel bigger than matrix. Can't convolve";
    exit(1);
  }
  int m_convolveMatrix=(m1-m2+1);
  int n_convolveMatrix=(n1-n2+1);
  //converting matrix to Toeplitz form
  int n_Toeplitz=c_matrix*m2*n2;
  int m_Toeplitz=m_convolveMatrix*n_convolveMatrix; 
  float** matrix_ToeplitzForm=createMatrix(m_Toeplitz,n_Toeplitz);
  int index=0;
  for(int i=0;i<m_convolveMatrix;i++) {
    for(int j=0;j<n_convolveMatrix;j++) {
      int k=0;
      for(int chan=0;chan<c_matrix;chan++) {
        for(int i1=i;i1<i+m2;i1++) {
          for(int j1=j;j1<j+n2;j1++) {
            matrix_ToeplitzForm[index][k++]=matrix[chan][i1][j1];
          }
        }
      }
      index++;
    }
  }
  
  //converting kernel matrix to column matrix
  float* kernel_ColumnMatrix=new float[c_kernel*m2*n2];
  index=0;
  for(int chan=0;chan<c_kernel;chan++) {
    for(int i=0;i<m2;i++) {
      for(int j=0;j<n2;j++) {
        kernel_ColumnMatrix[index]=kernel[chan][i][j];
        index++;
      }
    }
  }
  float** matrix_conv; 
  matrix_conv=createMatrix(m_convolveMatrix,n_convolveMatrix);

  //making an array to store multiplication result
  float* row_major_array = new float[m_Toeplitz];
  //calling the multiplication function which use openbas
  matrix_multiplication_openblas(matrix_ToeplitzForm,m_Toeplitz,n_Toeplitz,kernel_ColumnMatrix,c_kernel*m2*n2,row_major_array);
  //making final convolution matrix from the row major array
  for(int i=0;i<m_convolveMatrix;i++){
    for(int j=0;j<n_convolveMatrix;j++){
      matrix_conv[i][j] = row_major_array[i*n_convolveMatrix+j]+bias;
    }
  }  
  delete[] row_major_array;
  

  freeSpace(matrix_ToeplitzForm,m_Toeplitz);
  delete[] kernel_ColumnMatrix;
  
  return matrix_conv;
}

float*** convolution(float*** inputMatrix, int dim1,int dim2,int dim3, string kerneltextfile, int channels, int height,int width){
  
  //dim1 is Channel_prev
  float**** convWeights=createConvWeightMatrix(channels,dim1,height,width);
  float* bias=new float[channels];
  readConvWeightsAndBias(kerneltextfile,channels,dim1,height,width,convWeights,bias);
  float*** outputMatrix=new float**[channels];

  for(int i=0;i<channels;i++) {
    outputMatrix[i]=matrix_convolution_with_mult(inputMatrix,dim1,dim2,dim3,convWeights[i],dim1,height,width,bias[i]);
  }

  return outputMatrix;
}

void writeMatrix(string filename,float*** matrix,int dim1,int dim2,int dim3) {
  ofstream fout(filename);
  fout<<"[";
  for(int i=0;i<dim1;i++) {
    fout<<"[";
    for(int j=0;j<dim2;j++) {
      fout<<"[";
      for(int k=0;k<dim3;k++) {
        fout<<matrix[i][j][k]<<" ";
      }
      fout<<"]"<<endl;
    }
    fout<<"]"<<endl;
  }
  fout<<"]";
  fout.close();
}

void test1() {
  int inputImage_size1=28;
  int inputImage_size2=28;
  int inputImage_channel=1;
  string inputFile="./mnistSampleOutput/1/1_new.txt";
  float*** inputImage=readMatrix(inputFile,inputImage_channel,inputImage_size1,inputImage_size2);
  for(int i=0;i<inputImage_size1;i++) {
    for(int j=0;j<inputImage_size2;j++) {
      inputImage[0][i][j]=1-inputImage[0][i][j]/255;
    }
  }
  //conv1
  int conv1_channels=20;
  int kernelSize1=5;
  int kernelSize2=5;
  string kerneltextfile="./LenetWeights/conv1.txt";
  float*** conv1=convolution(inputImage,inputImage_channel,inputImage_size1,inputImage_size2, kerneltextfile,conv1_channels, kernelSize1, kernelSize2);
  writeMatrix("output.txt",conv1,conv1_channels,inputImage_size1-kernelSize1+1,inputImage_size2-kernelSize2+1);
}

void test2() {
  int inputImage_size1=12;
  int inputImage_size2=12;
  int inputImage_channel=20;
  string inputFile="pool1.txt";
  float*** inputImage=readMatrix(inputFile,inputImage_channel,inputImage_size1,inputImage_size2);

  //conv1
  int conv2_channels=50;
  int kernelSize1=5;
  int kernelSize2=5;
  string kerneltextfile="./LenetWeights/conv2.txt";
  float*** conv2=convolution(inputImage,inputImage_channel,inputImage_size1,inputImage_size2, kerneltextfile,conv2_channels, kernelSize1, kernelSize2);
  writeMatrix("output2.txt",conv2,conv2_channels,inputImage_size1-kernelSize1+1,inputImage_size2-kernelSize2+1);
}


int main() {
  test2();
}

