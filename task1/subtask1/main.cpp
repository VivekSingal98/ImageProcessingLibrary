#include <iostream>
#include<vector>
#include <fstream>
#include <string>
#include <math.h>
#include "utility.h"
#include "convolution.h"
#include "sampling.h"
#include "activation.h"
#include "probability.h"
using namespace std;
int main (int argc, char **argv)
{     
  string check = argv[1];
  //check for convolution with or without multiplication operation
  if(argc==8 && (check=="convolution_mult"||check=="convolution")){
    //Arguments-> ./main convolution/convolution_mult padding input_file input_rows kernel_file kernel_rows output_file
    int pad=stoi(argv[2]);
    string inputFile1=argv[3];
    int sizeMatrix1=stoi(argv[4]);
    string inputFile2=argv[5];
    int sizeMatrix2=stoi(argv[6]);
    string outputFileName=argv[7];

    //reading matrix1
    int m1=sizeMatrix1;
    int n1=sizeMatrix1;
    float** matrix1=readMatrix(inputFile1,m1,n1);

    //adding padding
    float** matrix1_pad=addPadding(matrix1,m1,n1,pad);
    freeSpace(matrix1,m1); //freeing matrix1 space as its no longer needed
    m1=m1+2*pad;
    n1=n1+2*pad;
    
    //reading matrix2
    int m2=sizeMatrix2;
    int n2=sizeMatrix2;
    float** matrix2=readMatrix(inputFile2,m2,n2);

    float** matrix3;
    if(check=="convolution_mult") {
      //convolving two matrices with multiplication by using Toeplitz form
      matrix3=matrix_convolution_with_mult(matrix1_pad,m1,n1,matrix2,m2,n2);
    }
    else {
      matrix3=conv_without_mult(matrix1_pad,m1,n1,matrix2,m2,n2);
    }
    int m_convolveMatrix=(m1-m2+1);
    int n_convolveMatrix=(n1-n2+1);
    
    //writing to output file
    writeMatrix(outputFileName,matrix3,m_convolveMatrix,n_convolveMatrix);
    
    freeSpace(matrix1_pad,m1);
    freeSpace(matrix2,m2);
    freeSpace(matrix3,m_convolveMatrix);
  }

  //check for sampling operation
  else if(argc==6 && (check=="maxpool"||check=="avgpool")){
    //usage-> ./main maxpool/avgpool input_file input_rows kernel_rows output_file
    string pooling_type = argv[1];    
    string inputFile = argv[2];
    int inputSize = stoi(argv[3]);
    int kernelSize = stoi(argv[4]);
    string outputFileName = argv[5];

    int m1=inputSize;
    int n1=inputSize;
    int m2=kernelSize;
    int n2=kernelSize;

    //reading input matrix
    float** matrix=readMatrix(inputFile,m1,n1);

    float** matrix_afterPool;
    if(pooling_type=="maxpool") {
      //convolving two matrices with multiplication
      matrix_afterPool=maxpool(matrix,m1,n1,m2,n2);
    }
    else {
      matrix_afterPool=avgpool(matrix,m1,n1,m2,n2);
    }
    int m_afterPool=m1/m2;
    int n_afterPool=n1/n2;
  
    //writing to output file
    writeMatrix(outputFileName,matrix_afterPool,m_afterPool,n_afterPool);
    
    freeSpace(matrix,m1);
    freeSpace(matrix_afterPool,m_afterPool);
  }

  //check for activation operation 
  else if(argc==6 && (check=="relu" || check=="tanh")){
    //usage -> ./main relu/tanh inputFile1.txt matrix_m matrix_n outputFile.txt
    string func=argv[1];
    string inputFile=argv[2];
    int m=stoi(argv[3]);
    int n=stoi(argv[4]);
    string outputFile=argv[5];

    //reading matrix
    float** matrix=readMatrix(inputFile,m,n);

    if(func=="relu") {
      relu(matrix,m,n);
    }
    else {
      tanh(matrix,m,n);
    }
    //writing matrix
    writeMatrix(outputFile,matrix,m,n);
    freeSpace(matrix,m); 
  }

  //check for probabilities operation
  else if(argc==4 && (check=="sigmoid" || check=="softmax")){
    //usage-> ./main sigmoid/softmax input_file output_file
    
    string probability_type = argv[1];    
    string inputFile = argv[2];
    string outputFile = argv[3];

    //storing the input matrix as vector
    vector<float> input = getVectorFromFile(inputFile);

    //compution sigmoid/softmax
    vector<float> output;
    if(probability_type=="sigmoid"){
      output = sigmoid(input);
    }
    else{
      output = softmax(input);  
    }

    //writing to file
    writeVectorToFile(outputFile,output);
  }

  //help if argument matches no specified format
  else{
    cout << "HELP:" << endl;
    cout << "convolution usage:" << endl;
    cout << "% ./main convolution/convolution_mult padding input_file input_rows kernel_file kernel_rows output_file" << endl;
    cout << "maxpool or avgpool usage:" << endl;
    cout << "% ./main maxpool/avgpool input_file input_rows kernel_rows output_file" << endl;
    cout << "relu or tanh usage:" << endl;
    cout << "% ./main activation relu/tanh inputFile1.txt matrix_m matrix_n outputFile.txt" << endl;
    cout << "sigmoid or softmax usage:" << endl;
    cout << "% ./main sigmoid/softmax input_file output_file" << endl;

  }

  return 0;

}