#include "utility.h"

//for debugging purpose
void printMatrix(float** matrix,int m,int n) {
    for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
            cout<<matrix[i][j]<<" ";
        }
    }
    cout<<"\n";
}

float** createMatrix(int m,int n) {
    float** matrix=new float*[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new float[n];
    }
    return matrix;
}

float*** createMatrix(int m,int n,int l) {
    float*** matrix=new float**[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new float*[n];
        for(int j=0;j<n;j++) {
          matrix[i][j]=new float[l];
        }
    }
    return matrix;
}

//read in column major form
float** readMatrix(string filename,int m,int n) {
  ifstream fin(filename);
  float** matrix=createMatrix(m,n);
  for(int j=0;j<n;j++) {
    for(int i=0;i<m;i++) {
      fin>>matrix[i][j];
    }
  }
  fin.close();
  return matrix;
}

float*** readMatrix(string filename,int m,int n,int l) {
  ifstream fin(filename);
  float*** matrix=createMatrix(m,n,l);
  for(int i=0;i<m;i++) {
    for(int j=0;j<n;j++) {
      for(int k=0;k<l;k++) {
        fin>>matrix[i][j][k];
      }
    }
  }
  fin.close();
  return matrix;
}

//write in column major form
void writeMatrix(string filename,float** matrix,int m,int n) {
  ofstream fout(filename);
  for(int j=0;j<n;j++) {
    for(int i=0;i<m;i++) {
      fout<<matrix[i][j]<<" ";
    }
  }
  fout.close();
}

//for debugging purposes
void writeMatrix(string filename,float*** matrix,int dim1,int dim2,int dim3) {
  ofstream fout(filename);
  fout<<"[";
  for(int i=0;i<dim1;i++) {
    fout<<"[";
    for(int j=0;j<dim2;j++) {
      fout<<"[";
      for(int k=0;k<dim3;k++) {
        fout<<matrix[i][j][k]<<" ";
      }
      fout<<"]"<<endl;
    }
    fout<<"]"<<endl;
  }
  fout<<"]";
  fout.close();
}

//read image from data.txt
float*** imageTo3dMatrix(string input_file){
  //first store all the values in a vector after removing '[' and ']'
  vector<float> input;
  ifstream fin(input_file);
  string a;
  while(fin>>a) {
    string temp = "";
    for(int i=0;i<a.length();i++){
      if(a[i]!='[' && a[i]!=']'){
          temp = temp + a[i];
      }
    }
    if(temp.length()>0){
      input.push_back(stof(temp));
    }  
  }
  fin.close();
  //transfer the values to a 3d array
  float*** input_mat = createMatrix(1,28,28);
  for(int i=0;i<28;i++){
    for(int j=0;j<28;j++){
      input_mat[0][i][j] = input[i*28+j];
    }
  }
  return input_mat;
}


//frees space created for dynamic arrays
void freeSpace(float** matrix,int m) {
    for(int i=0;i<m;i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}
