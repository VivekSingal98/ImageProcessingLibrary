convolution usage: convolution is without multiplication
% ./main convolution convolution padding input_file input_rows kernel_file kernel_rows output_file

convolution usage: convolution_mult is with multiplication
% ./main convolution_mult method padding input_file input_rows kernel_file kernel_rows output_file

maxpool or avgpool usage:
% ./main maxpool/avgpool input_file input_rows kernel_rows output_file

relu or tanh usage:
% ./main relu/tanh inputFile1.txt matrix_m matrix_n outputFile.txt

sigmoid or softmax usage: High number in input file can lead to infinity value due to exponential operation
% ./main sigmoid/softmax input_file output_file
