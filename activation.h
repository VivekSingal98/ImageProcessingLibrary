#ifndef ACTIVATION_H
#define ACTIVATION_H

#include<iostream>
#include<cmath>

using namespace std;

void relu(float** matrix,int m,int n);
void tanh(float** matrix,int m,int n);
void errorArgumentsActivation();

//help if wrong input argument written
void errorArgumentsActivation() {
	cerr<<"Wrong arguments. Follow below pattern for non linear activation"<<endl;
	cerr<<"./main activation relu/tanh inputFile1.txt matrix_m matrix_n outputFile.txt"<<endl;
}

//relu function
void relu(float** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			if(matrix[i][j]<(float)0) {
				matrix[i][j]=0;
			}
		}
	}	
}

//tanh function
void tanh(float** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			matrix[i][j]=tanh(matrix[i][j]);
		}
	}
}

#endif