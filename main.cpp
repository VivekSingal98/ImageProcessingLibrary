#include<iostream>
#include <fstream>
#include<string>
#include<cmath>

#include "util.h"
#include "convolution.h"
#include "activation.h"
#include "probability.h"
#include "sampling.h"

using namespace std;

int main(int argc, char** argv) {
	string type=argv[1];
	if(type=="convolution_mult") {
		//Arg pattern-> ./main convolution_mult padding inputFile1.txt sizeOfMatrix1 inputFile2.txt sizeOfMatrix2 outputFile.txt
		if(argc-2!=6) {
			errorArgumentsConv();
			exit(0);
		}
		//taking arguments
		int pad=stoi(argv[2]);
		string inputFile1=argv[3];
		int sizeMatrix1=stoi(argv[4]);
		string inputFile2=argv[5];
		int sizeMatrix2=stoi(argv[6]);
		string outputFileName=argv[7];

		//reading matrix1
		int m1=sizeMatrix1;
		int n1=sizeMatrix1;
		float** matrix1=readMatrix(inputFile1,m1,n1);
		// printMatrix(matrix,m1,n1);
		

		//adding padding
		float** matrix1_pad=addPadding(matrix1,m1,n1,pad);
		freeSpace(matrix1,m1); //freeing matrix1 space as its no longer needed
		m1=m1+2*pad;
		n1=n1+2*pad;
		
		//reading matrix2
		int m2=sizeMatrix2;
		int n2=sizeMatrix2;
		float** matrix2=readMatrix(inputFile2,m2,n2);
		// printMatrix(matrix2,m2,n2);
		
		//convolving two matrices with multiplication
		float** matrix3=matrix_convolution(matrix1_pad,m1,n1,matrix2,m2,n2);
		int m_convolveMatrix=(m1-m2+1);
		int n_convolveMatrix=(n1-n2+1);
		// printMatrix(matrix3,m_convolveMatrix,n_convolveMatrix);
		
		//writing to output file
		writeMatrix(outputFileName,matrix3,m_convolveMatrix,n_convolveMatrix);
		
		freeSpace(matrix1_pad,m1);
		freeSpace(matrix2,m2);
		freeSpace(matrix3,m_convolveMatrix);
		
	}
	else if(type=="convolution") {
		if(argc!=8) {
			errorArgumentsConv();
		}

	    int padding = stoi(argv[2]);
	    string inputfile = argv[3];
	    int input_rows = stoi(argv[4]);
	    string kernelfile = argv[5];
	    int kernel_rows = stoi(argv[6]);
	    string outputfile = argv[7];

	    string line;
	    readline_from_file(inputfile,&line);

	    string line1;
	    readline_from_file(kernelfile,&line1);

	    list<float> input = string_to_list(line);

	    list<float> kernel = string_to_list(line1);
	    
	    list<float> output = convolution(input,input_rows,padding,kernel,kernel_rows);

	    string result = "";
	    for(auto v : output){
	      result.append(to_string(v));
	      result.append(" ");
	    }
	  
	    writeline_to_file(outputfile,result);

	}
	else if(type=="activation") {
		//Arg pattern-> ./main activation relu/tanh inputFile1.txt matrix_m matrix_n outputFile.txt
		if(argc!=7) {
			errorArgumentsActivation();
		}
		string func=argv[2];
		string inputFile=argv[3];
		int m=stoi(argv[4]);
		int n=stoi(argv[5]);
		string outputFile=argv[6];

		//reading matrix
		float** matrix=readMatrix(inputFile,m,n);
		// printMatrix(matrix,m,n);
		if(func=="relu") {
			relu(matrix,m,n);
		}
		else {
			tanh(matrix,m,n);
		}
		
		//writing matrix
		writeMatrix(outputFile,matrix,m,n);
		freeSpace(matrix,m);
	}
	else if(type=="maxpool" || type=="avgpool") {
		//Arg ./main maxpool/avgpool input_file input_rows kernel_rows output_file
		if(argc!=6) {
			errorArgumentsSampling();
		}
	    string pooling_type = argv[1];    
	    string inputfile = argv[2];
	    int input_rows = stoi(argv[3]);
	    int kernel_rows = stoi(argv[4]);
	    string outputfile = argv[5];

	    string line;
	    readline_from_file(inputfile,&line);


	    list<float> input = string_to_list(line);
	    list<float> output;
	    if(pooling_type=="maxpool"){
	      output = max_pool(input,input_rows,kernel_rows);
	    }
	    if(pooling_type=="avgpool"){
	      output = avg_pool(input,input_rows,kernel_rows);  
	    }

	    string result = "";
	    for(auto v : output){
	      result.append(to_string(v));
	      result.append(" ");
	    }
	    
	    writeline_to_file(outputfile,result);    
	}
	else if(type=="sigmoid" || type=="softmax") {
		if(argc!=4) {
			errorArgumentsProb();
		}
	    string probability_type = argv[1];    
	    string inputfile = argv[2];
	    string outputfile = argv[3];

	    string line;
	    readline_from_file(inputfile,&line);


	    std::list<float> input = string_to_list(line);

	    list<float> output;
	    if(probability_type=="sigmoid"){
	      output = sigmoid(input);
	    }
	    if(probability_type=="softmax"){
	      output = softmax(input);  
	    }

	    string result = "";
	    for(auto v : output){
	      result.append(to_string(v));
	      result.append(" ");
	    }
	    

	    writeline_to_file(outputfile,result);
	}

	else {
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

}