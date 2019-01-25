CC=g++
CFlags=-c
MKLFLAG = -I ~/intel/mkl/include -L~/intel/mkl/lib/intel64 -Wl,--no-as-needed ~/intel/mkl/lib/intel64/libmkl_scalapack_lp64.a ~/intel/mkl/lib/intel64/libmkl_intel_lp64.a ~/intel/mkl/lib/intel64/libmkl_gnu_thread.a ~/intel/mkl/lib/intel64/libmkl_core.a ~/intel/mkl/lib/intel64/libmkl_blacs_intelmpi_lp64.a -lgomp -lpthread -lm -ldl
BLASFLAG = -I /include/ -L/lib/ -lopenblas -lpthread
main: main.cpp
	$(CC) main.cpp -m64 $(BLASFLAG) $(MKLFLAG)

clean: 
	rm -rf *o a.out
