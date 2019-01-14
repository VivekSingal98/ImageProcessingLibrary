void printMatrix(float** matrix,int m,int n);
float** readMatrix(ifstream& fin,int m,int n);
void writeMatrix(ofstream& fout,float** matrix,int m,int n);
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);

//print matrix to console for debigging purpose
void printMatrix(float** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			cout<<matrix[i][j]<<" ";
		}
	}
	cout<<"\n";
}

//read in column major form
float** readMatrix(ifstream& fin,int m,int n) {
	float** matrix=createMatrix(m,n);
	for(int j=0;j<n;j++) {
		for(int i=0;i<m;i++) {
			fin>>matrix[i][j];
		}
	}
	
	return matrix;
}

//write in column major form
void writeMatrix(ofstream& fout,float** matrix,int m,int n) {
	for(int j=0;j<n;j++) {
		for(int i=0;i<m;i++) {
			fout<<matrix[i][j]<<" ";
		}
	}
}

//removes dynamically allocated space
void freeSpace(float** matrix,int m) {
	for(int i=0;i<m;i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

//allocates dynamic space
float** createMatrix(int m,int n) {
	float** matrix=new float*[m];
	for(int i=0;i<m;i++) {
		matrix[i]=new float[n];
	}
	return matrix;
}