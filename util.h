void printMatrix(float** matrix,int m,int n);
float** readMatrix(ifstream& fin,int m,int n);
void writeMatrix(ofstream& fout,float** matrix,int m,int n);
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);

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
			fin>>matrix[j][i];
		}
	}
	return matrix;
}
void writeMatrix(ofstream& fout,float** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			fout<<matrix[j][i]<<" ";
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