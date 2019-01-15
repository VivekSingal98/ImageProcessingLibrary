#ifndef UTILITY_H
#define UTILITY_H
#include <iostream>
#include<list>
#include <iomanip>
#include<fstream>

using namespace std;


float get(list<float> _list, int _i){
    list<float>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

list<float> get(list<list<float>> _list, int _i){
    list<list<float>>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

list<float> string_to_list(string line){
    istringstream iss(line);
    list<float> input;
    for(string line; iss >> line; )
        input.push_back(stof(line));
    return input; 
}

void readline_from_file(string inputfile,string *line){
    ifstream infile; 
    infile.open(inputfile); 
    getline(infile,*line);
    infile.close();
}

void writeline_to_file(string outputfile, string result){
    ofstream outfile;
    outfile.open(outputfile);
    outfile << result << endl;
    outfile.close(); 
}


void printMatrix(float** matrix,int m,int n) {
    for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
            cout<<matrix[i][j]<<" ";
        }
    }
    cout<<"\n";
}

float** createMatrix(int m,int n) {
    float** matrix=new float*[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new float[n];
    }
    return matrix;
}

float** readMatrix(ifstream& fin,int m,int n) {
    float** matrix = createMatrix(m,n);
    for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
            fin>>matrix[j][i];
        }
    }
    return matrix;
}
void writeMatrix(ofstream& fout,float** matrix,int m,int n) {
    for(int i=0;i<m;i++) {
        for(int j=0;j<n;j++) {
            fout<<matrix[j][i]<<" ";
        }
    }
}

void freeSpace(float** matrix,int m) {
    for(int i=0;i<m;i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


#endif