#include "framemng.h"
#include "testtask.h"
#include "msgdef.h"

char *appName = "XframeTest";

int main(){

    threadEnvCreate();
    InitLogInfo(Cout, Info,100, "");
    TFRAMEManager manager;
    TestTask* task1 = new TestTask();
    TestTask* task2 = new TestTask();
    manager.registerTask(2, task1);
    manager.registerTask(3, task2);
    manager.Init(0, appName);

    TUniNetMsg * testmsg = new TUniNetMsg();
    testmsg->tAddr.logAddr = 3;
    testmsg->tAddr.phyAddr = 1;
    testmsg->tAddr.taskInstID=2;
    testmsg->oAddr.logAddr = 2;
    testmsg->oAddr.phyAddr = 1;
    testmsg->oAddr.taskInstID=1;
    task2->add(testmsg);

    manager.Run();
    delete task1;
    delete task2;
    return 0;
}
