main: utility.h func.h main.cpp
	g++ -o main main.cpp

func: utility.h func.h
	g++ -o func func.h

utility: utility.h
	g++ -o utility utility.h


