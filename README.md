
# MPPTF 

Multi Purpose Performance Testing Framework 


# MPPTF 개요

어떠한 모듈을 대상으로 Performance 를 테스트 할때는 single thread 부터 multi thread 의 형태로 테스트를 진행한다. 테스트 시에는 생성, 초기화, 런닝, 마무리 단계를 거처 성능 수치를 측정하게 되는데 이러한 일련의 반복되는 기능을 Framework 로 구성하여, 사용자는 test core 부분만 작성하도록 하는것을 목적으로 제작되었다.

본 문서는 MPPTF 의 구조와 동작원리등을 설명하며 사용자가 본 Framework 를 이용하여 간단한 샘플을 작성할 수 있도록 설명을 한다.

# MPPTF 구조

MPPTF 는 사용자 Plugin 부분과 이를 제어하는 Runner 부분으로 구성된다. 

### Plugin

Plugin 은 사용자에 의해 메뉴얼한 코드로 구성된 Shared Object 를 의미한다. 사용자는 MPPTF 에서 제공되는 ITask 클래스를 상속한 클래스를 작성하고 이 클래스를 조작할 Interface 함수를 작성하여 Shared Object 형태로 만들어야 한다.

### Runner

Runner 는 실행가능한 바이너리로 입력된 사용자 Plugin 을 dynmic loading 후 이를 실행하며 Performance 를 측정, 결과를 제공하는 역할을 한다.

# Plugin 구성 모듈

### ITask Virtual Class

아래는 Plugin 을 위해 제공되는 ITask 클래스이다. Interface 역할을 하고 있으며 실제로 하는 역할은 없다. 사용자의 개발방식에 따라 작성하지 않아도 된다.

```cpp
class ITask {
    public :
        virtual void Initialize ( int aThreadIndex, int aLoopCount )  = 0; 
        virtual void Run (int aIndex) = 0; 
        virtual void Finalize () = 0 ;
};
```

>> virtual void Initialize ( int aThreadIndex, int aLoopCount )  = 0; 

Plugin 로딩시 최초로 실행되는 함수로 초기화 역할의 코드를 작성한다. aThreadIndex 파라미터는 Runner 에서 순차적으로 정한 Thread 들의 Index 값이며 0 부터 thread_count -1 까지 입력된다. aLoopCount 는 Run 함수의 호출 수를 의미한다.

>> virtual void Run (int aIndex) = 0;

초기화 이후 테스트할 코드를 작성한다. aIndex 는 현재 실행되는 차수를 의미하며 0 부터 aLoopCount -1 까지 입력된다.

>> virtual void Finalize () = 0;

Run 함수의 aLoopCount 만큼 수행 이후 마지막으로 호출되는 함수이다. 리소스 정리코드를 작성한다.

### Interface Functions (New, Init, Run, Finalize)

Runner 에서 Plugin 을 사용할때 호출되는 실제 함수 이다. Plugin 작성시 사용자는 주어진 Interface 대로 반드시 작성해야 한다. 사용자가 ITask 를 상속한 클래스를 작성하였을 경우 기능에 맞게 객체 생성, 실행, 해제 등의 코드를 작성하며 ITask 를 상속한 클래스를 작성하지 않을 경우 그 역할을 함수에 직접 코딩하여도 무방하다. 주의 할 점은 함수내 또는 전역변수 사용시 ThreadSafe 하지 않으므로 주의가 필요하다.

아래는 SimpleSelectTask 라는 ITask 클래스를 상속한 사용자 정의 클래스를 생성하여 각 Interface 함수에 역할별로 코딩을 한것이다.

>> New()

```c
int New  (ObjectHandle *aHandle ) 
{
    SimpleSelectTask *sHandle = new SimpleSelectTask () ;
    *aHandle = sHandle; 
    return 0 ;
}
```

 Plugin 사용시 Thread 별로 최초 한번만 생성되며, 테스트에 사용할 객체들에 대한 생성코드를 작성한다. 

```c
ObjectHandle* aHandle
  Runner 에서 입력되는 Handle 포인터로써 사용자가 다른 함수들의 aHandle 에서 사용하고 싶은 객체 혹은 메모리 구조체를 할당을 위한 것이다.
```

>> Init()

```c
int Init (ObjectHandle aHandle, int aThreadNo, int aLoopCount ) 
{

    SimpleSelectTask *sHandle = (SimpleSelectTask*)aHandle; 
    sHandle->Initialize (aThreadNo, aLoopCount) ;
    return 0;

}
```

**New()** 함수와 마찬가지로 Thread 별로 **New()** 함수 호출 이후 한번만 호출 된다. 코드의 초기화 부분을 담당하며 **New()** 호출 이후 각 객체에 및 설정에 대한 초기화 코드를 작성한다. 


```c
ObjectHandle aHandle
  New() 에서 사용자가 할당한 Handle 이다.
int aThreadNo
  Runner 에서 순차적으로 정한 Thread 들의 Index 값이며 0 부터 thread_count -1 까지 입력된다.
int aLoopCount
  Run 함수의 호출 수를 의미한다.
```

>> Run()

```c
int Run  (ObjectHandle aHandle, int aLoop ) 
{
    SimpleSelectTask *sHandle = (SimpleSelectTask*)aHandle; 
    sHandle->Run(aLoop);

    return 0 ; 
}
```

Run() 함수는 테스트 코드가 실행되는 함수로 **Init()** 의 **aLoopCount** 만큼 반복되어 호출 된다. 

```c
ObjectHandle aHandle
  New() 에서 사용가 할당한 Handle
int aLoop
  현재 Run 함수의 호출 sequence 로 0 부터 시작된다.
```

>> Finalize

```c
int Finalize ( ObjectHandle aHandle)
{
    SimpleSelectTask *sHandle = (SimpleSelectTask*)aHandle; 
    sHandle->Finalize();

    return 0;

}
```

Run() 함수의 모든 수행이 끝나고 Thread 별 마지막에 한 번 호출되는 함수이다. 테스트간에 사용된 리소스 해제용 코드를 작성한다.

```c
ObjectHandle aHandle
  New() 에서 사용자가 할당한 Handle
```

위 4개의 함수는 return 타입이 int 성공시에 0 를 반환해야 하며 그 외의 값이 반환되면 Runner 에 의해 모든 테스트는 중지된다. 중지될때는 별도의 Finalize 과정을 거치지 않고 바로 종료 되기 때문에 Finalize() 코드호출을 보장 할 수 없다.

(나중에는 추가 될듯))

# MPPTF sample

상기 설명된 Plugin 설명을 토대로 Plugin 을 만들어 본다. 아래에서 설명되는 코드 및 Makefile 은 소스 체크아웃시 plugin 디렉터리에서 확인할 수 있다.

### Source Checkout

```bash
$ git clone https://github.com/sunjesoft/mpptf.git mpptf
Cloning into 'mpptf'...
remote: Counting objects: 81, done.
remote: Compressing objects: 100% (57/57), done.
remote: Total 81 (delta 35), reused 69 (delta 23), pack-reused 0
Unpacking objects: 100% (81/81), done.
```

### SimpleSelectTask 클래스 작성

아래 경로에 ITask 를 상속받는 SimpleSelectTask 클래스의 헤더 파일과 소스 파일을 작성한다.

```bash
$ vi mpptf/plugins/SimpleSelectTask/SimpleSelectTask.hpp
```

```cpp
#pragma once
#include <ITask.hpp>
#define MAX_STRING_LEN 1024

class SimpleSelectTask : public ITask {

    public :
        SimpleSelectTask () ;
        void Initialize ( int aThreadIndex, int aLoopCounter );
        void Run (int aIndex);
        void Finalize ();
        ~SimpleSelectTask () ;

    private :
        int mThreadIndex;
        int mLoopCount ;
        void setConnName ( char * aConnName);
};
```

소스는 Goldilocks EmbeddedSQL 을 이용하여 작성되었다.

```bash
$ vi plugins/SimpleSelectTask/SimpleSelectTask.gc
```

```c
#include <SimpleSelectTask.hpp>
#include <cstdio>
#include <cstring>
#include <unistd.h>
#include <cassert>


SimpleSelectTask  :: SimpleSelectTask () { }

EXEC SQL INCLUDE SQLCA;


void SimpleSelectTask :: Initialize (int aThreadIndex, int aLoopCount)
{
    EXEC SQL BEGIN DECLARE SECTION ;
    char sUserName [MAX_STRING_LEN] ;
    char sPassword [MAX_STRING_LEN] ;
    char sDSN [MAX_STRING_LEN] ;
    char sConnName [ MAX_STRING_LEN ];
    EXEC SQL END   DECLARE SECTION ;

    mThreadIndex = aThreadIndex ;
    mLoopCount = aLoopCount;


    std::strncpy ( sUserName , "test" , MAX_STRING_LEN );
    std::strncpy ( sPassword , "test" , MAX_STRING_LEN );
    std::strncpy ( sDSN , "DSN=GOLDILOCKS", MAX_STRING_LEN);

    setConnName ( sConnName ) ;

    EXEC SQL AT :sConnName CONNECT :sUserName IDENTIFIED BY :sPassword USING :sDSN;
    if ( sqlca.sqlcode ) {
        std::printf("Fail to connect : [%s]\n" , sqlca.sqlerrm.sqlerrmc );
    }
}

void SimpleSelectTask :: Run (int aIndex)
{
    struct sqlca sqlca;
    EXEC SQL BEGIN DECLARE SECTION ;
    int   sValue;
    char sConnName [ MAX_STRING_LEN ];
    EXEC SQL END   DECLARE SECTION ;

    // Stop Complain ...
    aIndex = aIndex;
    setConnName ( sConnName ) ;

    EXEC SQL AT :sConnName SELECT 1 into :sValue FROM DUAL ;
    if ( sqlca.sqlcode ) {
        std::printf("EXEC ERROR : [%s] [%s]\n" , sqlca.sqlerrm.sqlerrmc, sConnName );
        assert ( 0 );
    }

}

void SimpleSelectTask :: Finalize ()
{
    struct sqlca sqlca;

    EXEC SQL BEGIN DECLARE SECTION ;
    char sConnName [ MAX_STRING_LEN ];
    EXEC SQL END   DECLARE SECTION ;

    setConnName ( sConnName ) ;

    EXEC SQL AT :sConnName DISCONNECT ;
    if ( sqlca.sqlcode ) {
        std::printf("EXEC ERROR : [%s] [%s]\n" , sqlca.sqlerrm.sqlerrmc, sConnName );
        assert ( 0 );
    }
}


void SimpleSelectTask :: setConnName ( char * aConnName)
{
    sprintf ( aConnName, "CONN_%d", mThreadIndex);
}
````

아래와 같이 Makefile 을 작성한다.

```bash
$ vi plugins/SimpleSelectTask/Makefile
```

```makefile
CXX_FLAGS=-std=c++11 -O0 -W -Wall -fpic  -g
CXX=g++
INC= -I. -I.. -I${GOLDILOCKS_HOME}/include
all:
	gpec SimpleSelectTask.gc -o SimpleSelectTask.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} SimpleSelectTask.cc
	${CXX} -c ${CXX_FLAGS} ${INC} ${LIB} ${LPATH} SimpleSelectTask.c -o SimpleSelectSelectInterface.o
	gcc -shared -o libSimpleSelectTask.so SimpleSelectTask.o SimpleSelectSelectInterface.o -lgoldilocksesqls -L${GOLDILOCKS_HOME}/lib -lgoldilockscs-ul64

clean:
	rm -rf *.o test_timer *.so test_timer_c *.a SimpleSelectTask.cc
```

make 명령어를 통해 plugin 을 생성한다.

```bash
$ make
gpec SimpleSelectTask.gc -o SimpleSelectTask.cc

 Copyright © 2010 SUNJESOFT Inc. All rights reserved.
 Release Venus.3.1.16 revision(25210)

FileName: SimpleSelectTask.gc
Pre-compile SimpleSelectTask.gc -> SimpleSelectTask.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   SimpleSelectTask.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   SimpleSelectTask.c -o SimpleSelectSelectInterface.o
gcc -shared -o libSimpleSelectTask.so SimpleSelectTask.o SimpleSelectSelectInterface.o -lgoldilocksesqls -L/home/chlee/workspace/product/Gliese/home/lib -lgoldilockscs-ul64
```

오류 없이 make 작업이 끝났다면 Framework 를 생성한다.

```bash
$ cd mpptf
$ make

$ make
cd timer && make
make[1]: 디렉터리 '/home/chlee/workspace/mpptf/timer' 들어감
g++ -fpic -c -std=c++11 -O2 -W -Wall  -I.   -lMultiPointTimer -L. MultiPointTimer.cc
g++ -fpic -c -std=c++11 -O2 -W -Wall  -I.   -lMultiPointTimer -L. MultiPointTimerC.cc
gcc -shared -o libMultiPointTimer.so MultiPointTimerC.o  MultiPointTimer.o
ar rcs libMultiPointTimer.a MultiPointTimerC.o MultiPointTimer.o
make[1]: 디렉터리 '/home/chlee/workspace/mpptf/timer' 나감
g++ -c -std=c++11 -O2 -W -Wall -fpic  -I. -I ./timer   Runner.cc
g++ -o Runner Runner.o ./timer/libMultiPointTimer.a -ldl -lpthread
cd plugins && make
make[1]: 디렉터리 '/home/chlee/workspace/mpptf/plugins' 들어감
cd SimpleSelectTask && make all
make[2]: 디렉터리 '/home/chlee/workspace/mpptf/plugins/SimpleSelectTask' 들어감
gpec SimpleSelectTask.gc -o SimpleSelectTask.cc

 Copyright © 2010 SUNJESOFT Inc. All rights reserved.
 Release Venus.3.1.16 revision(25210)

FileName: SimpleSelectTask.gc
Pre-compile SimpleSelectTask.gc -> SimpleSelectTask.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   SimpleSelectTask.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   SimpleSelectTask.c -o SimpleSelectSelectInterface.o
gcc -shared -o libSimpleSelectTask.so SimpleSelectTask.o SimpleSelectSelectInterface.o -lgoldilocksesqls -L/home/chlee/workspace/product/Gliese/home/lib -lgoldilockscs-ul64
make[2]: 디렉터리 '/home/chlee/workspace/mpptf/plugins/SimpleSelectTask' 나감
cd SimpleInserter && make all
make[2]: 디렉터리 '/home/chlee/workspace/mpptf/plugins/SimpleInserter' 들어감
gpec SimpleInserter.gc -o SimpleInserter.cc

 Copyright © 2010 SUNJESOFT Inc. All rights reserved.
 Release Venus.3.1.16 revision(25210)

FileName: SimpleInserter.gc
Pre-compile SimpleInserter.gc -> SimpleInserter.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   SimpleInserter.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   SimpleInserter.c -o SimpleInserterC.o
gcc -shared -o libSimpleInserter.so SimpleInserter.o SimpleInserterC.o -lgoldilocksesqls -L/home/chlee/workspace/product/Gliese/home/lib -lgoldilockscs-ul64
make[2]: 디렉터리 '/home/chlee/workspace/mpptf/plugins/SimpleInserter' 나감
cd PCRF && make all
make[2]: 디렉터리 '/home/chlee/workspace/mpptf/plugins/PCRF' 들어감
gpec PCRF.gc -o PCRF.cc

 Copyright © 2010 SUNJESOFT Inc. All rights reserved.
 Release Venus.3.1.16 revision(25210)

FileName: PCRF.gc
Pre-compile PCRF.gc -> PCRF.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   PCRF.cc
g++ -c -std=c++11 -O0 -W -Wall -fpic  -g -I. -I.. -I/home/chlee/workspace/product/Gliese/home/include   PCRF.c -o PCRFC.o
gcc -shared -o libPCRF.so PCRF.o PCRFC.o -lgoldilocksesqls -L/home/chlee/workspace/product/Gliese/home/lib -lgoldilockscs-ul64
make[2]: 디렉터리 '/home/chlee/workspace/mpptf/plugins/PCRF' 나감
make[1]: 디렉터리 '/home/chlee/workspace/mpptf/plugins' 나감
```

Framework 컴파일 시에는 하위 plugins 디렉터리에 위치한 plugin 들을 함께 컴파일 한다.
다음은 Framework 실행 방법이다. SimpleSelectTask 를 실행해 본다.

```bash
$ Runner
Got long argc[1]
Usage : Runner -t [TotalThreadCount] -l [LibraryFilePath] -c [LoopCount]
Example : Runner -t 10 -l ./plugin/libSelect.so -c 10000

$ Runner -t 1 -l ./plugins/SimpleSelectTask/libSimpleSelectTask.so -c 1
Thread[    0] MinLat[  1353.79] AvgLat[  1353.79] MaxLat[  1353.79] 99 Pct[     0.00] 99.9 Pct[     0.00] Ops[   738.67]
------------------------------------------------------------------------------------------------------------------------
                                                                                                    Total Ops[   738.67]
```
