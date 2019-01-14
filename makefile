CXX = g++

convolution: convolution.cpp convolution_withMultiplication.h util.h
	$(CXX) -o convolution convolution.cpp