
include ../../Makefile.$(shell uname)

CXX_FLAGS=-std=c++11 -O0 -W -Wall -fpic  -g
CXX=g++
INC= -I. -I.. -I${GOLDILOCKS_HOME}/include
all: 
	gpec SimpleInserter.gc -o SimpleInserter.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} SimpleInserter.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} SimpleInserter.c -o SimpleInserterC.o
	gcc $(SHARED_CFLAGS) -o libSimpleInserter.$(SHARED_LIB_EXT) SimpleInserter.o SimpleInserterC.o -lgoldilocksesqls -L${GOLDILOCKS_HOME}/lib -lgoldilockscs-ul64 $(CXX_LIBS)

clean:
	rm -rf *.o  *.so  *.a SimpleInserter.cc
