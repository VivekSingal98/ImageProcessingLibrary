CXX = g++

main: main.cpp convolution.h util.h activation.h probability.h sampling.h
	$(CXX) -o main main.cpp