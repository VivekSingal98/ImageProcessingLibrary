#include<iostream>
#include<string>
#include "utility.h"

int main() {
  int inputImage_size1=28;
  int inputImage_size2=28;
  int inputImage_channel=1;
  string inputFile="./mnistSampleOutput/1/1_new.txt";
  float*** inputImage=readMatrix(inputFile,inputImage_channel,inputImage_size1,inputImage_size2);

  //conv1
  int conv1_channels=20;
  int kernelSize1=5;
  int kernelSize2=5;
  inputFile="./LenetWeights/conv1.txt";
  float*** conv1Kernel=readMatrix(inputFile,conv1_channels,kernelSize1,kernelSize2);
  


}