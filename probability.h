#ifndef PROBABILITY_H
#define PROBABILITY_H
#include <iostream>
#include<vector>
#include <math.h>
#include "utility.h"

using namespace std;
vector<float> sigmoid(vector<float> input);
vector<float> softmax(vector<float> input);

vector<float> sigmoid(vector<float> input){
  vector<float> output;
  for (int i =0,size=input.size(); i <size; i++){
    float exponential = exp(-input[i]);
    output.push_back(1/(1+exponential));
  }
  return output;
}

vector<float> softmax(vector<float> input){
  vector<float> output;
  double sum=0;
  for (int i =0,size=input.size(); i <size; i++){
    sum = sum + exp(input[i]);
  }

  for (int i =0,size=input.size(); i <size; i++){
    output.push_back(float(exp(input[i])/sum));
  }

  return output;
}
#endif