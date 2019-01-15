#ifndef UTIL_H
#define UTIL_H

#include <iostream>
#include<list>
#include <string>
#include <iomanip>
#include<fstream>

using namespace std;

void printMatrix(float** matrix,int m,int n);
float** readMatrix(string filename,int m,int n);
void writeMatrix(string filename,float** matrix,int m,int n);
void freeSpace(float** matrix,int m);
float** createMatrix(int m,int n);

//print matrix to console for debigging purpose
void printMatrix(float** matrix,int m,int n) {
	for(int i=0;i<m;i++) {
		for(int j=0;j<n;j++) {
			cout<<matrix[i][j]<<" ";
		}
	}
	cout<<"\n";
}

//read in column major form
float** readMatrix(string filename,int m,int n) {
	ifstream fin(filename);
	float** matrix=createMatrix(m,n);
	for(int j=0;j<n;j++) {
		for(int i=0;i<m;i++) {
			fin>>matrix[i][j];
		}
	}
	fin.close();
	return matrix;
}

//write in column major form
void writeMatrix(string filename,float** matrix,int m,int n) {
	ofstream fout(filename);
	for(int j=0;j<n;j++) {
		for(int i=0;i<m;i++) {
			fout<<matrix[i][j]<<" ";
		}
	}
	fout.close();
}

//removes dynamically allocated space
void freeSpace(float** matrix,int m) {
	for(int i=0;i<m;i++) {
		delete[] matrix[i];
	}
	delete[] matrix;
}

//allocates dynamic space
float** createMatrix(int m,int n) {
	float** matrix=new float*[m];
	for(int i=0;i<m;i++) {
		matrix[i]=new float[n];
	}
	return matrix;
}

/**********/
float get(list<float> _list, int _i){
    list<float>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}

list<float> get(list<list<float> > _list, int _i){
    list<list<float> >::iterator it = _list.begin();
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



#endif