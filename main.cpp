#include<iostream>
#include <fstream>
#include<string>
#include<cmath>
using namespace std;
#include "util.h"
#include "convolution_withMultiplication.h"
#include "activation.h"

int main(int argc, char** argv) {
	string type=argv[1];
	if(type=="convolution_mult") {
		//Arg pattern-> ./main convolution_mult padding inputFile1.txt sizeOfMatrix1 inputFile2.txt sizeOfMatrix2 outputFile.txt
		if(argc-2!=6) {
			errorArgumentsConvMult();
			exit(0);
		}
		//taking arguments
		int pad=stoi(argv[2]);
		string inputFile1=argv[3];
		int sizeMatrix1=stoi(argv[4]);
		string inputFile2=argv[5];
		int sizeMatrix2=stoi(argv[6]);
		string outputFileName=argv[7];

		//reading matrix1
		int m1=sizeMatrix1;
		int n1=sizeMatrix1;
		ifstream fin(inputFile1);
		float** matrix1=readMatrix(fin,m1,n1);
		// printMatrix(matrix,m1,n1);
		fin.close();

		//adding padding
		float** matrix1_pad=addPadding(matrix1,m1,n1,pad);
		freeSpace(matrix1,m1); //freeing matrix1 space as its no longer needed
		m1=m1+2*pad;
		n1=n1+2*pad;
		
		//reading matrix2
		fin.open(inputFile2);
		int m2=sizeMatrix2;
		int n2=sizeMatrix2;
		float** matrix2=readMatrix(fin,m2,n2);
		// printMatrix(matrix2,m2,n2);
		fin.close();
		
		//convolving two matrices with multiplication
		float** matrix3=matrix_convolution(matrix1_pad,m1,n1,matrix2,m2,n2);
		int m_convolveMatrix=(m1-m2+1);
		int n_convolveMatrix=(n1-n2+1);
		// printMatrix(matrix3,m_convolveMatrix,n_convolveMatrix);
		
		//writing to output file
		ofstream fout(outputFileName);
		writeMatrix(fout,matrix3,m_convolveMatrix,n_convolveMatrix);
		
		freeSpace(matrix1_pad,m1);
		freeSpace(matrix2,m2);
		freeSpace(matrix3,m_convolveMatrix);
		fout.close();
	}
	else if(type=="activation") {
		//Arg pattern-> ./main activation relu/tanh inputFile1.txt matrix_m matrix_n outputFile.txt
		if(argc!=7) {
			errorArgumentsActivation();
		}
		string func=argv[2];
		string inputFile=argv[3];
		int m=stoi(argv[4]);
		int n=stoi(argv[5]);
		string outputFile=argv[6];

		//reading matrix
		ifstream fin(inputFile);
		float** matrix=readMatrix(fin,m,n);
		// printMatrix(matrix,m,n);
		if(func=="relu") {
			relu(matrix,m,n);
		}
		else {
			tanh(matrix,m,n);
		}
		fin.close();
		//writing matrix
		ofstream fout(outputFile);
		writeMatrix(fout,matrix,m,n);
		fout.close();
		freeSpace(matrix,m);
	}

}