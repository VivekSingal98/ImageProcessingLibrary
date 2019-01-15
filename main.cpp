#include <iostream>
#include<list>
#include<iterator>
#include <fstream>
#include <string>
#include <iomanip>
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
  //check for convolution operation
  if(argc==8 && check=="convolution"){
    int padding = stoi(argv[2]);
    string inputfile = argv[3];
    int input_rows = stoi(argv[4]);
    string kernelfile = argv[5];
    int kernel_rows = stoi(argv[6]);
    string outputfile = argv[7];

    //read line from input file
    string line;
    readline_from_file(inputfile,&line);
    string line1;
    readline_from_file(kernelfile,&line1);

    //storing the input and kernel matrix as list
    list<float> input = string_to_list(line);
    list<float> kernel = string_to_list(line1);
    
    //computing the convolution operation
    list<float> output = conv(input,input_rows,padding,kernel,kernel_rows);

    //storing result convolution as string
    string result = "";
    for(auto v : output){
      result.append(to_string(v));
      result.append(" ");
    }
  
    //write result to ouput file
    writeline_to_file(outputfile,result);



  }

  //check for convolution using multiplication operation
  else if(argc==8 && check=="convolution_mult"){
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

  //check for sampling operation
  else if(argc==6){
    string pooling_type = argv[1];    
    string inputfile = argv[2];
    int input_rows = stoi(argv[3]);
    int kernel_rows = stoi(argv[4]);
    string outputfile = argv[5];

    //read line from input file
    string line;
    readline_from_file(inputfile,&line);

    //storing the input matrix as list
    list<float> input = string_to_list(line);
    
    //computing the maxpool/avgpool 
    list<float> output;
    if(pooling_type=="maxpool"){
      output = max_pool(input,input_rows,kernel_rows);
    }
    if(pooling_type=="avgpool"){
      output = avg_pool(input,input_rows,kernel_rows);  
    }

    //storing result sampling as string
    string result = "";
    for(auto v : output){
      result.append(to_string(v));
      result.append(" ");
    }
    
    //write result to output file
    writeline_to_file(outputfile,result);    

  }

  //check for activation operation 
  else if(argc==7 && (check=="activation")){
    
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

  //check for probabilities operation
  else if(argc==4 && (check=="sigmoid" || check=="softmax")){
    
    string probability_type = argv[1];    
    string inputfile = argv[2];
    string outputfile = argv[3];

    //read line from input file
    string line;
    readline_from_file(inputfile,&line);

    //storing the input matrix as list
    std::list<float> input = string_to_list(line);

    //compution sigmoid/softmax
    list<float> output;
    if(probability_type=="sigmoid"){
      output = sigmoid(input);
    }
    if(probability_type=="softmax"){
      output = softmax(input);  
    }

    //storing result sampling as string
    string result = "";
    for(auto v : output){
      result.append(to_string(v));
      result.append(" ");
    }
    
    //write result to output file
    writeline_to_file(outputfile,result);
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
    cout << "% ./main maxpool/avgpool input_file output_file" << endl;

  }

  return 0;

}