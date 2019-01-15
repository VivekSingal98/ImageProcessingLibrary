#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include <iostream>
#include<list>
#include<iterator>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include "utility.h"

using namespace std;

//convolution of 2 matrices stored as column major in list 
list<float> convolution(list<float> input, int input_rows, int padding, list<float> kernel, int kernel_rows){
  
  //storing input column major list as 2D list (also in column major format) 
  int input_col = input_rows;
  list<list<float>> input_mat;
  for(int i=0;i<input_col;i++){
    list<float> col;
    for(int j=0;j<input_rows;j++){
      col.push_back(get(input,i*input_col+j));

    }
    //taking care of padding
    for(int j=0;j<padding;j++){
      col.push_front(0.0);
      col.push_back(0.0);
    }
    input_mat.push_back(col);
  }
  //taking care of padding
  for(int i=0;i<padding;i++){
    list<float> col,col1;
    for(int j=0;j<input_rows+2*padding;j++){
      col.push_back(0.0);
      col1.push_back(0.0);
    }
    input_mat.push_front(col);
    input_mat.push_back(col1);

  }

	//storing kernel column major list as 2D list (also in column major format)  
  int kernel_col = kernel_rows;
  list<list<float>> kernel_mat;
  for(int i=0;i<kernel_col;i++){
    list<float> col;
    for(int j=0;j<kernel_rows;j++){
      col.push_back(get(kernel,i*kernel_col+j));

    }
    kernel_mat.push_back(col);
  }

  //convolution operation
  list<float> output;
  int input_rows1 = input_rows+2*padding;
  int input_col1 = input_rows1;
  for(int i=0;i<=input_col1 - kernel_col;i++){
    for(int j=0;j<=input_rows1 - kernel_rows;j++){
      float element = 0.0;
      for(int k=0;k<kernel_col;k++){
        for(int l=0;l<kernel_rows;l++){
          element = element + get(get(input_mat,i+k),j+l)*get(get(kernel_mat,k),l);
        }
      }
      output.push_back(element);
    }
  }
  return output;
} 


//multipies two matrices
float** matrix_multiplication(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2) {
	if(n1!=m2) {
		cerr<<"matrixes not compatible for multiplication";
		exit(0);
	}
	float** matrix3=createMatrix(m1,n2);
	for(int i=0;i<m1;i++) {
		for(int j=0;j<n2;j++) {
			matrix3[i][j]=0;
			for(int k=0;k<n1;k++) {
				matrix3[i][j]+=matrix1[i][k]*matrix2[k][j];
			}
		}
	} 
	return matrix3;
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

//convolution of two matrices(actually cross correlation)
float** matrix_convolution(float** matrix,int m1,int n1,float** kernel,int m2,int n2) {
	if(m1<m2 || n1<n2) {
		cerr<<"kernel bigger than matrix. Can't convolve";
		exit(0);
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

	float** matrix_conv_column=matrix_multiplication(matrix_ToeplitzForm,m_Toeplitz,n_Toeplitz,kernel_ColumnMatrix,m2*n2,1);
	//making final convolution matrix from the column matrix
	float** matrix_conv=createMatrix(m_convolveMatrix,n_convolveMatrix);
	index=0;
	for(int i=0;i<m_convolveMatrix;i++) {
		for(int j=0;j<n_convolveMatrix;j++) {
			matrix_conv[i][j]=matrix_conv_column[index++][0];
		}
	}
	freeSpace(matrix_ToeplitzForm,m_Toeplitz);
	freeSpace(kernel_ColumnMatrix,m2*n2);
	freeSpace(matrix_conv_column,m_Toeplitz);
	return matrix_conv;
}

#endif