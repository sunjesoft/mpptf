

include ../Makefile.$(shell uname)
#CXX_FLAGS=-std=c++11 -g -O0 -W -Wall
CXX_FLAGS=-std=c++11 -O2 -W -Wall 
INC= -I.  
LIB=-lMultiPointTimer
LPATH= -L.
all: 
	g++ -fpic -c ${CXX_FLAGS} ${INC} MultiPointTimer.cc
	g++ -fpic -c ${CXX_FLAGS} ${INC} MultiPointTimerC.cc

	gcc $(SHARED_CFLAGS) -o libMultiPointTimer.$(SHARED_LIB_EXT) MultiPointTimerC.o  MultiPointTimer.o $(CXX_LIBS)
	ar rcs libMultiPointTimer.a MultiPointTimerC.o MultiPointTimer.o


test : 
	g++ ${INC} test.cc -o test_timer ${LPATH} ${LIB}
	gcc ${INC} test.c -o test_timer_c ${LPATH} ${LIB} -lstdc++


clean:
	rm -rf *.o test_timer *.so test_timer_c *.a
