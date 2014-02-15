#include "testselftask.h"
#include "testrecvtask.h"
#include "testsendtask.h"
#include "framemng.h"

char *appName = "KernalTest";

int main()
{
    threadEnvCreate();
    InitLogInfo(File, Info, 100000, "log/KernalTest_");
    
    TFRAMEManager manager;
    TestRecvTask*   recvtask    = new TestRecvTask();
    TestSendTask*   sendtask    = new TestSendTask();
    //TestSelfTask*   selftask    = new TestSelfTask();

    //manager.registerTask(1002, selftask);
    manager.registerTask(1001, recvtask);
    manager.registerTask(101,  sendtask);

    manager.Init(1, appName);
    manager.Run();

    //if(selftask!=NULL) delete selftask;
    if(recvtask!=NULL) delete recvtask;
    if(sendtask!=NULL)  delete sendtask;

    return 0;
}


