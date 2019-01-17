#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include<vector>
#include<fstream>

using namespace std;

vector<float> getVectorFromFile(string inputFile);
void writeVectorToFile(string outputFile, vector<float> vect);
void printMatrix(float** matrix,int m,int n);
float** readMatrix(string filename,int m,int n);
void writeMatrix(string filename,float** matrix,int m,int n);
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);


vector<float> getVectorFromFile(string inputFile){
    vector<float> input;
    ifstream fin(inputFile);
    float a;
    while(fin>>a) {
      input.push_back(a);  
    }
    fin.close();
    return input; 
}

void writeVectorToFile(string outputFile, vector<float> vect){
    ofstream fout(outputFile);
    for(int i =0,size=vect.size(); i <size; i++) {
      fout<<vect[i]<<" ";
    }
    fout.close();
}

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

//frees space created for dynamic arrays
void freeSpace(float** matrix,int m) {
    for(int i=0;i<m;i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


#endif