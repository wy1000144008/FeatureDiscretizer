# Makefile

### CPP
CC = g++
LD = g++

OPTIONS=-Ofast -march=native -std=c++11 

.SUFFIXES:
.SUFFIXES: .h .cpp .o .exe 

.cpp.exe:
	$(CC) $(OPTIONS) $*.cpp -o $*.exe 

.cpp.o: 
	$(LD) -c -o $*.o $*.cpp $(OPTIONS)

.cpp:
	$(CC) $(OPTIONS) -c $*.cpp
	$(LD) -o $* $*.o $(OPTIONS)

TIMECMD=gtime -f 'user:%U(sec) sys:%S(sec) elapsed:%E(sec) memory:%M(B)'


feature_discretizer.o:  feature_discretizer.cpp  feature_discretizer.h


fd.exe:  feature_discretizer.o
	 g++ $(OPTIONS) feature_discretizer.o -o fd.exe


all: fd.exe

clean:
	-rm -f  *~  *.o 

cleanall: clean
	make clean
	-rm -f *.exe 
	-rm -r *.dSYM;
	-rm -f log.txt log

### EOF


