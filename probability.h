#ifndef PROBABILITY_H
#define PROBABILITY_H

#include <iostream>
#include<list>
#include<iterator>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include "util.h"

using namespace std;

void errorArgumentsProb() {
  cerr<<"Wrong arguments. Follow below pattern for probability"<<endl;
  cerr<<"./main maxpool/avgpool input_file output_file"<<endl;
}

list<float> sigmoid(list<float> input){
  list<float> output;
  for (auto it = input.begin(); it != input.end(); ++it){
    float exponential = exp(*it);
    output.push_back(exponential/(1.0+exponential));
    
  }
  return output;
}

list<float> softmax(list<float> input){
  list<float> output;
  float sum;
  for (auto it = input.begin(); it != input.end(); ++it){
    sum = sum + exp(*it);
  }
  for (auto it = input.begin(); it != input.end(); ++it){
    output.push_back(exp(*it)/sum);
  }

  return output;
}

#endif