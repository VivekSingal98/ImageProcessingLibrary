#ifndef PROBABILITY_H
#define PROBABILITY_H
#include <iostream>
#include<list>
#include<iterator>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include "utility.h"

using namespace std;

//compute sigmoid probabilities
list<float> sigmoid(list<float> input){
  list<float> output;
  //sigmoid operation
  for (auto it = input.begin(); it != input.end(); ++it){
    float exponential = exp(*it);
    output.push_back(exponential/(1.0+exponential));
    
  }
  return output;
}

//compute softmax probabilities
list<float> softmax(list<float> input){
  list<float> output;
  //softmax operation
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