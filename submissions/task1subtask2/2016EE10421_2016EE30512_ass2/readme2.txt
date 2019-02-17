to create gnuplot we need data for that theses files must be executed

mkltime-
g++ -o mkltime mkltime.cpp -m64 -I ~/intel/mkl/include -L~/intel/mkl/lib/intel64 -Wl,--no-as-needed ~/intel/mkl/lib/intel64/libmkl_scalapack_lp64.a ~/intel/mkl/lib/intel64/libmkl_intel_lp64.a ~/intel/mkl/lib/intel64/libmkl_gnu_thread.a ~/intel/mkl/lib/intel64/libmkl_core.a ~/intel/mkl/lib/intel64/libmkl_blacs_intelmpi_lp64.a -lgomp -lpthread -lm -ldl

openblastime-
g++ -o openblastime openblastime.cpp -I /include/ -L/lib/ -lopenblas -lpthread

pthreadtime-
g++ -o pthreadtime pthreadtime.cpp

simplemultiplication-
g++ -o simplemultiplication simplemultiplication.cpp

makedata-
g++ -o makedata makedata.cpp

now data will be saved in timefiles folder. it will have time taken by different methods on different sizes in microseconds

for gnuplot. type first gnuplot in ternminal then type
load 'plot.p'

different png plots will be plotted
