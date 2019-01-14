CXX = g++

main: main.cpp convolution_withMultiplication.h util.h activation.h
	$(CXX) -o main main.cpp