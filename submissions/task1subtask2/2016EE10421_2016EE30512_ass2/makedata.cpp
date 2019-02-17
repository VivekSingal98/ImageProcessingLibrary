#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

int main() {
  vector<string> methods{"mkl","openblas","pthread","iteration"};
  vector<int> sizes{10,50,100,500,1000};
  int noofvalues=100;

  for(int k=0;k<sizes.size();k++) {

    int m=noofvalues;
    int n=methods.size(); // mkl,openblas,pthread,iteration
    float** matrix=new float*[m];
    for(int i=0;i<m;i++) {
        matrix[i]=new float[n];
    }
    for(int j=0;j<n;j++) {
      ifstream fin("./timefiles/"+methods[j]+to_string(sizes[k])+".txt");
      for(int i=0;i<m;i++) {
        fin>>matrix[i][j];
      }
      fin.close();
    }
    ofstream fout("./timefiles/combined"+to_string(sizes[k])+".txt");
    fout<<"#";
    for(int i=0;i<n;i++) {
      fout<<methods[i]<<"\t";
    }
    fout<<"\n";
    for(int i=0;i<m;i++) {
      for(int j=0;j<n;j++) {
        fout<<matrix[i][j]<<"\t\t";
      }
      fout<<"\n";
    }
    fout.close();
  }

}