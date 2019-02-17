#include <iostream>
#include<vector>
#include <fstream>
#include <string>
#include "utility.h"
#include "convolution.h"
#include "sampling.h"
#include "probability.h"
using namespace std;

void sort(vector<float> vec);

//sort the softmax probability and give top 5 classes with respective probabilities
void sort(vector<float> vec){
  float a;
  int index;
  for(int i=0;i<5;i++){
    a = 0.0;
    index = 0;
    for(int j=0;j<10;j++){
      if(vec[j]>=a){
        a = vec[j];
        index = j;
      }
    }
    cout << "class:" << index << " probability: " << a << endl;
    vec[index] = -1;
  }
}


int main (int argc, char **argv)
{     
  if(argc==7) {
    //Arguments-> ./main lenet input_file conv1 conv2 fc1 fc2
    string input_file = argv[2];
    string conv1_kernelfile = argv[3];
    string conv2_kernelfile = argv[4];
    string fc1_kernelfile = argv[5];
    string fc2_kernelfile = argv[6];
    
    //forward propogation
    float*** input_mat = createMatrix(1,28,28);
    input_mat = imageTo3dMatrix(input_file);
    float*** conv1 = createMatrix(20,24,24);
    conv1 = convolution(input_mat,1,28,28,conv1_kernelfile,20,5,5,0);
    float*** pool1 = createMatrix(20,12,12);
    pool1 = max_pool(conv1,20,24,24,2,2);
    float*** conv2 = createMatrix(50,8,8); 
    conv2 = convolution(pool1,20,12,12,conv2_kernelfile,50,5,5,0);
    float*** pool2 = createMatrix(50,4,4);
    pool2 = max_pool(conv2,50,8,8,2,2);
    float*** fc1 = createMatrix(500,1,1);
    fc1 = convolution(pool2,50,4,4,fc1_kernelfile,500,4,4,1);
    float*** fc2 = createMatrix(10,1,1);
    fc2 = convolution(fc1,500,1,1,fc2_kernelfile,10,1,1,0);

    //storing final layer output in a vector
    vector<float> output;
    for(int i=0;i<10;i++){
      output.push_back(fc2[i][0][0]);
    }  

    //taking out the probabilities
    vector<float> probabilities = softmax(output);
    //sorting the probabilities and giving top 5 classes
    sort(probabilities);

  }
  else{
    cout << "HELP:" << endl;
    cout << "lenet usage:" << endl;
    cout << "% ./main lenet input_file conv1_file conv2_file fc1_file fc2_file" << endl;
  }
}
