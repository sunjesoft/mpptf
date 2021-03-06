#include <SimpleSelectTask.hpp>
#include <ITask.h>


int New  (ObjectHandle *aHandle ) 
{
    SimpleSelectTask *sHandle = new SimpleSelectTask () ;
    *aHandle = sHandle; 
    return 0 ;
}

int Init (ObjectHandle aHandle, int aThreadNo, int aLoopCount ) 
{

    SimpleSelectTask *sHandle = (SimpleSelectTask*)aHandle; 
    sHandle->Initialize (aThreadNo, aLoopCount) ;
    return 0;

}

int Run  (ObjectHandle aHandle, int aLoop ) 
{
    SimpleSelectTask *sHandle = (SimpleSelectTask*)aHandle; 
    sHandle->Run(aLoop);

    return 0 ; 
}

int Finalize ( ObjectHandle aHandle)
{
    SimpleSelectTask *sHandle = (SimpleSelectTask*)aHandle; 
    sHandle->Finalize();

    return 0;

}
