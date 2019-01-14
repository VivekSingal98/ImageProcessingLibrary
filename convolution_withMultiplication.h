float** matrix_multiplication(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2);
float** matrix_convolution(float** matrix,int m1,int n1,float** kernel,int m2,int n2);
float** addPadding(float** matrix,int m,int n,int p);
void errorArguments();

void errorArguments() {
	cerr<<"Wrong arguments"<<endl;
	cerr<<"./main convolve_mult padding inputFile1.txt sizeOfMatrix1 inputFile2.txt sizeOfMatrix2 outputFile.txt"<<endl;
}
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
	freeSpace(matrix_ToeplitzForm,m_Toeplitz);
	freeSpace(kernel_ColumnMatrix,m2*n2);
	freeSpace(matrix_conv_column,m_Toeplitz);
	return matrix_conv;
}