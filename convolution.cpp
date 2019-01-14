#include<iostream>
#include <fstream>
#include <vector>
using namespace std;


float** matrix_multiplication(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2);
void printMatrix(float** matrix,int m,int n);
float** readMatrix(ifstream& fin,int m,int n);
void writeMatrix(ofstream& fout,float** matrix,int m,int n);
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);
float** matrix_convolution(float** matrix,int m1,int n1,float** kernel,int m2,int n2);


float** matrix_multiplication(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2) {
	if(n1!=m2) {
		throw "matrixes not compatible for multiplication";
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
void printMatrix(float** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			cout<<matrix[i][j]<<" ";
		}
	}
	cout<<"\n";
}
float** readMatrix(ifstream& fin,int m,int n) {
	float** matrix=createMatrix(m,n);
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			fin>>matrix[i][j];
		}
	}
	return matrix;
}
void writeMatrix(ofstream& fout,float** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			fout<<matrix[i][j]<<" ";
		}
	}
}

void freeSpace(float** matrix,int m) {
	for(int i=0;i<m;i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

float** createMatrix(int m,int n) {
	float** matrix=new float*[m];
	for(int i=0;i<m;i++) {
		matrix[i]=new float[n];
	}
	return matrix;
}

float** matrix_convolution(float** matrix,int m1,int n1,float** kernel,int m2,int n2) {
	if(m1<m2 || n1<n2) {
		throw "kernel bigger than matrix. Can't convolve";
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
	return matrix_conv;
}

int main() {
	int m1;
	int n1;
	ifstream fin("input.txt");
	fin>>m1>>n1;
	float** matrix=readMatrix(fin,m1,n1);
	 printMatrix(matrix,m1,n1);
	int m2,n2;
	fin>>m2>>n2;
	float** kernel=readMatrix(fin,m2,n2);
	printMatrix(kernel,m2,n2);
	fin.close();
	// cout<<"m1 "<<m1<<" n1 "<<n1<<" m2 "<<m2<<" n2 "<<n2;
	// int** matrix3=matrix_multiplication(matrix,m1,n1,kernel,m2,n2);
	float** matrix3=matrix_convolution(matrix,m1,n1,kernel,m2,n2);
	int m_convolveMatrix=(m1-m2+1);
	int n_convolveMatrix=(n1-n2+1);
	printMatrix(matrix3,m_convolveMatrix,n_convolveMatrix);
	ofstream fout("output.txt");
	writeMatrix(fout,matrix3,m_convolveMatrix,n_convolveMatrix);
	
	freeSpace(matrix,m1);
	freeSpace(kernel,m2);
	freeSpace(matrix3,m_convolveMatrix);
	fout.close();
}