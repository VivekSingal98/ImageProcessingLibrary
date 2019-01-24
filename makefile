main: main.cpp utility.h convolution.h sampling.h probability.h activation.h
	g++ -o main main.cpp -I/include/ -L/lib/ -lopenblas -lpthread

