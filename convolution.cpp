#include<iostream>
#include <fstream>
#include <vector>
using namespace std;

int** matrix_multiplication(int** matrix1,int m1,int n1,int** matrix2,int m2,int n2) {
	if(n1!=m2) {
		throw "matrixes not compatible for multiplication";
	}
	int** matrix3=new int*[m1];
	for(int i=0;i<m1;i++) {
		matrix3[i]=new int[n2];
		for(int j=0;j<n2;j++) {
			matrix3[i][j]=0;
			for(int k=0;k<n1;k++) {
				matrix3[i][j]+=matrix1[i][k]*matrix2[k][j];
			}
		}
	} 
	return matrix3;
}
void printMatrix(int** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			cout<<matrix[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"\n";
}
int** readMatrix(ifstream& fin,int m,int n) {
	int** matrix=new int*[m];
	for(int i=0;i<m;i++) {
		matrix[i]=new int[n];
		for(int j=0;j<n;j++) {
			fin>>matrix[i][j];
		}
	}
	return matrix;
}
void writeMatrix(ofstream& fout,int** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			fout<<matrix[i][j]<<" ";
		}
		fout<<"\n";
	}
}
void freeSpace(int** matrix,int m) {
	for(int i=0;i<m;i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}
int main() {
	int m1;
	int n1;
	ifstream fin("input.txt");
	fin>>m1>>n1;
	int** matrix=readMatrix(fin,m1,n1);
	// printMatrix(matrix,m1,n1);
	int m2,n2;
	fin>>m2>>n2;
	int** kernel=readMatrix(fin,m2,n2);
	// printMatrix(kernel,m2,n2);
	fin.close();
	int** matrix3=matrix_multiplication(matrix,m1,n1,kernel,m2,n2);
	printMatrix(matrix3,m1,n2);
	ofstream fout("output.txt");
	writeMatrix(fout,matrix3,m1,n2);
	
	freeSpace(matrix,m1);
	freeSpace(kernel,m2);
	freeSpace(matrix3,m1);
	fout.close();
}