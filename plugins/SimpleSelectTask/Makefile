
include ../../Makefile.$(shell uname)
CXX_FLAGS=-std=c++11 -O0 -W -Wall -fpic  -g
CXX=g++
INC= -I. -I.. -I${GOLDILOCKS_HOME}/include
all: 
	gpec SimpleSelectTask.gc -o SimpleSelectTask.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} SimpleSelectTask.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} SimpleSelectTask.c -o SimpleSelectSelectInterface.o
	gcc $(SHARED_CFLAGS) -o libSimpleSelectTask.$(SHARED_LIB_EXT) SimpleSelectTask.o SimpleSelectSelectInterface.o -lgoldilocksesqls -L${GOLDILOCKS_HOME}/lib -lgoldilockscs-ul64 $(CXX_LIBS)

clean:
	rm -rf *.o test_timer *.so test_timer_c *.a SimpleSelectTask.cc
