CXX = g++
CXXFLAGS = -Wall -g
LIBS= -L/usr/local/opt/openblas/lib -lopenblas -lpthread
INCLUDE=-I/usr/local/opt/openblas/include
main: lenet.o utility.o convolution.o sampling.o probability.o
	$(CXX) $(CXXFLAGS) $(INCLUDE) $(LIBS) -o main lenet.o utility.o convolution.o sampling.o probability.o
lenet.o: lenet.cpp utility.h convolution.h sampling.h probability.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c lenet.cpp
utility.o: utility.cpp utility.h
	$(CXX) $(CXXFLAGS) -c utility.cpp
convolution.o: convolution.cpp convolution.h utility.h
	$(CXX) $(CXXFLAGS) $(INCLUDE) -c convolution.cpp
sampling.o: sampling.cpp sampling.h utility.h
	$(CXX) $(CXXFLAGS) -c sampling.cpp
probability.o: probability.cpp probability.h utility.h
	$(CXX) $(CXXFLAGS) -c probability.cpp
clean: 
	-rm -f main *.o core *.core
