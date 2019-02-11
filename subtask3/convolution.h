#ifndef CONVOLUTION_H
#define CONVOLUTION_H
#include <iostream>
#include <fstream>
#include <pthread.h>
#include "utility.h"
#include <cblas.h>

using namespace std;
void* single_mult(void* arg);
float** matrix_multiplication_pthread(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2);
void matrix_multiplication_openblas(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2,float* matrix3);
float** addPadding(float** matrix,int m,int n,int p);
float** matrix_convolution_with_mult(float** matrix,int m1,int n1,float** kernel,int m2,int n2,string method);
float** conv_without_mult(float** matrix, int m1, int n1, float** kernel, int m2, int n2);
float*** convolution(float*** inputMatrix, int dim1,int dim2,int dim3, string kerneltextfile, int channels, int height,int width,int relu);


//making a struct for storing the matix row and kernel matrix to be multiplied
struct mult_store_struct {
	float* matrix_row;
	float** matrix_column;
	int l1,l2;
	float* answer; 
};

//function for computing multiplication of one row with another matrix 
void* single_mult(void* arg){
	struct mult_store_struct *arg_struct = (struct mult_store_struct*) arg;
	float* sum = new float[arg_struct->l2];
	for(int i=0;i<arg_struct->l2;i++){
		sum[i] = 0;
		for(int j=0;j<arg_struct->l1;j++){
			sum[i] = sum[i] + (arg_struct->matrix_row[j])*(arg_struct->matrix_column[j][i]); 
		}
	} 
	arg_struct->answer = sum;
	pthread_exit(0);
}


//multipies two matrices using pthreads
float** matrix_multiplication_pthread(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2) {
	if(n1!=m2) {
		cerr<<"matrixes not compatible for multiplication";
		exit(1);
	}
	float** matrix3=createMatrix(m1,n2);
		

	//declaring array of struct and pthread corresponding to each row of matrix1 
	struct mult_store_struct args[m1];  
	pthread_t tids[m1];
	
	//creating pthread for multiplying each row of matrix1 with matrix2
	for(int i=0;i<m1;i++) {
		
			
			args[i].matrix_row = matrix1[i];
			args[i].matrix_column = matrix2;
			args[i].l1 = n1;
			args[i].l2 = n2;
			pthread_create(&tids[i], NULL, single_mult, &args[i]);			
		
	}

	//running the pthreads and storing result in matrix3
	for(int i=0;i<m1;i++) {
		
			pthread_join(tids[i],NULL);
			matrix3[i] = args[i].answer;
		
	}

	return matrix3;
}

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

//convolution of two matrices(actually cross correlation) without multiplication
float** conv_without_mult(float** matrix, int m1, int n1, float** kernel, int m2, int n2){
	if(m1<m2 || n1<n2) {
		cerr<<"kernel bigger than matrix. Can't convolve";
		exit(1);
	}
	int m_convolveMatrix=(m1-m2+1);
	int n_convolveMatrix=(n1-n2+1);
	float** matrix_conv=createMatrix(m_convolveMatrix,n_convolveMatrix);
	for(int i=0;i<m_convolveMatrix;i++) {
		for(int j=0;j<n_convolveMatrix;j++) {
			float elem=0;
			for(int k=0;k<m2;k++) {
				for(int l=0;l<n2;l++) {
					elem+=matrix[i+k][j+l]*kernel[k][l];
				}
			}
			matrix_conv[i][j]=elem;
		}
	}
	return matrix_conv;

} 

float*** convolution(float*** inputMatrix, int dim1,int dim2,int dim3, string kerneltextfile, int channels, int height,int width, int relu){
  
  //dim1 is Channel_prev
  float**** convWeights=createConvWeightMatrix(channels,dim1,height,width);
  float* bias=new float[channels];
  readConvWeightsAndBias(kerneltextfile,channels,dim1,height,width,convWeights,bias);
  float*** outputMatrix=new float**[channels];

  for(int i=0;i<channels;i++) {
    outputMatrix[i]=matrix_convolution_with_mult(inputMatrix,dim1,dim2,dim3,convWeights[i],dim1,height,width,bias[i]);
  }

  if(relu==1){
  	for(int i=0;i<channels;i++){
  		for(int j=0;j<dim2-height+1;j++){
  			for(int k=0;k<dim3-width+1;k++){
  				if(outputMatrix[i][j][k]<0){
  					outputMatrix[i][j][k] = 0;
  				}
  			}
  		}
  	}

  }

  return outputMatrix;
}

#endif