
include ../../Makefile.$(shell uname)
CXX_FLAGS=-std=c++11 -O0 -W -Wall -fpic  -g
CXX=g++
INC= -I. -I.. -I${GOLDILOCKS_HOME}/include
all: 
	gpec PCRF.gc -o PCRF.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} PCRF.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} PCRF.c -o PCRFC.o
	gcc $(SHARED_CFLAGS) -o libPCRF.$(SHARED_LIB_EXT) PCRF.o PCRFC.o -lgoldilocksesqls -L${GOLDILOCKS_HOME}/lib -lgoldilockscs-ul64 $(CXX_LIBS)

clean:
	rm -rf *.o  *.so  *.a PCRF.cc
