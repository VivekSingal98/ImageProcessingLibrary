#include<iostream>
#include <cstdlib>
#include<vector>
#include <chrono>
#include<fstream>
#include <pthread.h>

using namespace std;
using namespace std::chrono;
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);
void writeMatrix(string filename,float* matrix,int m);
void* single_mult(void* arg);
float** matrix_multiplication_pthread(float** matrix1,int m1,int n1,float** matrix2,int m2,int n2);


void freeSpace(float** matrix,int m) {
    for(int i=0;i<m;i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

//creates a random m*n matrix
float** createMatrix(int m,int n) {
    float** matrix=new float*[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new float[n];
    }
    for(int i=0;i<m;i++) {
      for(int j=0;j<n;j++) {
        float r = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100));
        matrix[i][j]=r;
      }
    }
    return matrix;
}

//write array to a file
void writeArray(string filename,float* matrix,int m) {
  ofstream fout(filename);
  for(int i=0;i<m;i++) {
    fout<<matrix[i];
    fout<<"\n";
  }
  fout.close();
}

//making a struct for storing the matix row and kernel matrix to be multiplied
struct mult_store_struct {
  float* matrix_row;
  float** matrix_column;
  int l1,l2;
  float* answer; 
};

//function for computing multiplication of one row with another matrix 
void* single_mult(void* arg){
  struct mult_store_struct *arg_struct = (struct mult_store_struct*) arg;
  float* sum = new float[arg_struct->l2];
  for(int i=0;i<arg_struct->l2;i++){
    sum[i] = 0;
    for(int j=0;j<arg_struct->l1;j++){
      sum[i] = sum[i] + (arg_struct->matrix_row[j])*(arg_struct->matrix_column[j][i]); 
    }
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
    

  //declaring array of struct and pthread corresponding to each row of matrix1 
  struct mult_store_struct args[m1];  
  pthread_t tids[m1];
  
  //creating pthread for multiplying each row of matrix1 with matrix2
  for(int i=0;i<m1;i++) {
      args[i].matrix_row = matrix1[i];
      args[i].matrix_column = matrix2;
      args[i].l1 = n1;
      args[i].l2 = n2;
      pthread_create(&tids[i], NULL, single_mult, &args[i]);      
    
  }

  //running the pthreads and storing result in matrix3
  for(int i=0;i<m1;i++) {
      pthread_join(tids[i],NULL);
      matrix3[i] = args[i].answer;
  }

  return matrix3;
}


int main() {

  int noofvalues=100;  //will test time taken by an operation 100 times
  int kernel_m=200;    //kernel size is fixed to be 200*200 
  int kernel_n=200;
  vector<int> sizes{10,50,100,500,1000};  //matrix size is 10*200,50*200,100*200,500*200,1000*200
  
  float** kernel=createMatrix(kernel_m,kernel_n);  //kernel of 200*200 matrix with random numbers generated
  float* timeMatrix=new float[noofvalues]; //time taken will be stored in file
  
  for(int k=0;k<sizes.size();k++) {

    for(int i=0;i<noofvalues;i++) {

      float** matrix=createMatrix(sizes[k],kernel_m);
      high_resolution_clock::time_point t1 = high_resolution_clock::now();
      float** result=matrix_multiplication_pthread(matrix,sizes[k],kernel_m,kernel,kernel_m,kernel_n);
      high_resolution_clock::time_point t2 = high_resolution_clock::now();
      auto time_span = duration_cast<microseconds>( t2 - t1 );
      timeMatrix[i]=(float)time_span.count();
      freeSpace(matrix,sizes[k]);
    }

    //writing to file
    string concat="pthread"+to_string(sizes[k])+".txt";
    writeArray("./timefiles/"+concat,timeMatrix,noofvalues);
  }
  freeSpace(kernel,kernel_m);
  delete[] timeMatrix;
}

