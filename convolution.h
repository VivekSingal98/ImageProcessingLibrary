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
float** matrix_multiplication_openblas(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2,float* matrix3);
float** addPadding(float** matrix,int m,int n,int p);
float** matrix_convolution_with_mult(float** matrix,int m1,int n1,float** kernel,int m2,int n2,string method);
float** conv_without_mult(float** matrix, int m1, int n1, float** kernel, int m2, int n2);

//making a struct for storing the matix row and column to be multiplied
struct mult_store_struct {
	float* matrix_row;
	float* matrix_column;
	int length;
	float answer; 
};

//function for computing multiplication of one row and one column 
void* single_mult(void* arg){
	struct mult_store_struct *arg_struct = (struct mult_store_struct*) arg;
	float sum = 0;
	for(int i=0;i<arg_struct->length;i++){
		sum = sum + (arg_struct->matrix_row[i])*(arg_struct->matrix_column[i]); 
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
		
	//transposing matrix2
	float** column_mat = createMatrix(n2,m2);
	for(int i=0;i<n2;i++){
		for(int j=0;j<m2;j++){
			column_mat[i][j] = matrix2[j][i];
		}	 	
	}

	//declaring array of struct and pthread corresponding to each row and column multiplication 
	struct mult_store_struct args[m1*n2];  
	pthread_t tids[m1*n2];
	
	//creating pthread for multiplying each row of matrix1 with each column of matrix2
	for(int i=0;i<m1;i++) {
		for(int j=0;j<n2;j++) {
			
			args[i*n2+j].matrix_row = matrix1[i];
			args[i*n2+j].matrix_column = column_mat[j];
			args[i*n2+j].length = n1;
			pthread_create(&tids[i*n2+j], NULL, single_mult, &args[i*n2+j]);			
		}
	}

	//running the pthreads and storing result in matrix3
	for(int i=0;i<m1;i++) {
		for(int j=0;j<n2;j++) {
			pthread_join(tids[i*n2+j],NULL);
			matrix3[i][j] = args[i*n2+j].answer;
		}
	}

	return matrix3;
}

//multipies two matrices using openblas
float** matrix_multiplication_openblas(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2,float* matrix3) {
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
	cblas_sgemm(CblasColMajor, CblasNoTrans, CblasNoTrans, m1, n2, n1, 1, col_major_matrix1, m1, col_major_matrix2, m1, 0, matrix3, m1);

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
float** matrix_convolution_with_mult(float** matrix,int m1,int n1,float** kernel,int m2,int n2,string method) {
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
	if(method=="pthread"){
		//calling the multiplication function which use pthread
		float** matrix_conv_column = matrix_multiplication_pthread(matrix_ToeplitzForm,m_Toeplitz,n_Toeplitz,kernel_ColumnMatrix,m2*n2,1);
		
		//making final convolution matrix from the column matrix
		index=0;
		for(int i=0;i<m_convolveMatrix;i++) {
			for(int j=0;j<n_convolveMatrix;j++) {
				matrix_conv[i][j]=matrix_conv_column[index++][0];
			}
		}
		freeSpace(matrix_conv_column,m_Toeplitz);
	}
	else if(method=="openblas"){
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
	}

	freeSpace(matrix_ToeplitzForm,m_Toeplitz);
	freeSpace(kernel_ColumnMatrix,m2*n2);
	
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

#endif