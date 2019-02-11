
//multipies two matrices using openblas
void matrix_multiplication_openblas(float** matrix1,int m1,int n1,float* matrix2,int m2,float* matrix3) {
 
  //making row major array from original matrices
  float* row_major_matrix1 = new float[m1*n1];
  
  for(int i=0;i<m1;i++){
    for(int j=0;j<n1;j++){
      row_major_matrix1[i*n1+j] = matrix1[i][j];
    }
  }

  //calling the matrix multiplication function in openblas
  cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m1, 1, n1, 1, row_major_matrix1, n1, matrix2, 1, 0, matrix3, 1);
}

//convolution of two matrices(actually cross correlation) with multiplication
float** matrix_convolution_with_mult(float*** matrix,int c_matrix,int m1,int n1,float*** kernel,int c_kernel,int m2,int n2,float bias) {
  if(m1<m2 || n1<n2 || c_matrix!=c_kernel) {
    cerr<<"kernel bigger than matrix. Can't convolve";
    exit(1);
  }
  int m_convolveMatrix=(m1-m2+1);
  int n_convolveMatrix=(n1-n2+1);
  //converting matrix to Toeplitz form
  int n_Toeplitz=c_matrix*m2*n2;
  int m_Toeplitz=m_convolveMatrix*n_convolveMatrix; 
  float** matrix_ToeplitzForm=createMatrix(m_Toeplitz,n_Toeplitz);
  int index=0;
  for(int i=0;i<m_convolveMatrix;i++) {
    for(int j=0;j<n_convolveMatrix;j++) {
      int k=0;
      for(int chan=0;chan<c_matrix;chan++) {
        for(int i1=i;i1<i+m2;i1++) {
          for(int j1=j;j1<j+n2;j1++) {
            matrix_ToeplitzForm[index][k++]=matrix[chan][i1][j1];
          }
        }
      }
      index++;
    }
  }
  
  //converting kernel matrix to column matrix
  float* kernel_ColumnMatrix=new float[c_kernel*m2*n2];
  index=0;
  for(int chan=0;chan<c_kernel;chan++) {
    for(int i=0;i<m2;i++) {
      for(int j=0;j<n2;j++) {
        kernel_ColumnMatrix[index]=kernel[chan][i][j];
        index++;
      }
    }
  }
  float** matrix_conv; 
  matrix_conv=createMatrix(m_convolveMatrix,n_convolveMatrix);

  //making an array to store multiplication result
  float* row_major_array = new float[m_Toeplitz];
  //calling the multiplication function which use openbas
  matrix_multiplication_openblas(matrix_ToeplitzForm,m_Toeplitz,n_Toeplitz,kernel_ColumnMatrix,c_kernel*m2*n2,row_major_array);
  //making final convolution matrix from the row major array
  for(int i=0;i<m_convolveMatrix;i++){
    for(int j=0;j<n_convolveMatrix;j++){
      matrix_conv[i][j] = row_major_array[i*n_convolveMatrix+j]+bias;
    }
  }  
  delete[] row_major_array;
  

  freeSpace(matrix_ToeplitzForm,m_Toeplitz);
  delete[] kernel_ColumnMatrix;
  
  return matrix_conv;
}


