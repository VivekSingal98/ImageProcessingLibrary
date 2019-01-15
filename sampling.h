#ifndef SAMPLING_H
#define SAMPLING_H
#include <iostream>
#include<list>
#include<iterator>
#include <fstream>
#include <string>
#include <iomanip>
#include <math.h>
#include "utility.h"

using namespace std;

//sub sampling of given matrix stored as column major list using max pool
list<float> max_pool(list<float> input, int input_rows, int kernel_rows){
  
  //storing input column major list as 2D list (also in column major format)
  int input_col = input_rows;
  list<list<float>> input_mat;
  for(int i=0;i<input_col;i++){
    list<float> col;
    for(int j=0;j<input_rows;j++){
      col.push_back(get(input,i*input_col+j));

    }
    input_mat.push_back(col);
  }
  

  int kernel_col = kernel_rows;
  list<float> output;
  
  //max pool operation
  for(int i=0;i<=input_col - kernel_col;i++){
    for(int j=0;j<=input_rows - kernel_rows;j++){
      list<float> element;
      for(int k=0;k<kernel_col;k++){
        for(int l=0;l<kernel_rows;l++){
          element.push_back(get(get(input_mat,i+k),j+l));
        }
      }
      element.sort();
      output.push_back(element.back());
    }
  }
  return output;
}

//sub sampling of given matrix stored as column major list using avg pool
list<float> avg_pool(list<float> input, int input_rows, int kernel_rows){
  
  //storing input column major list as 2D list (also in column major format)
  int input_col = input_rows;
  list<list<float>> input_mat;
  for(int i=0;i<input_col;i++){
    list<float> col;
    for(int j=0;j<input_rows;j++){
      col.push_back(get(input,i*input_col+j));

    }
    input_mat.push_back(col);
  }
  
  int kernel_col = kernel_rows;
  list<float> output;
  
  // avg pool operation
  for(int i=0;i<=input_col - kernel_col;i++){
    for(int j=0;j<=input_rows - kernel_rows;j++){
      float element = 0.0;
      for(int k=0;k<kernel_col;k++){
        for(int l=0;l<kernel_rows;l++){
          element = element + get(get(input_mat,i+k),j+l);
        }
      }
      
      output.push_back(element/(kernel_rows*kernel_col));
    }
  }
  return output;
}

#endif