#include "Receiver.h"
#include "framemng.h"
#include "Sender.h"
//#include "sipstackpsa.h"
//#include "testtask.h"
#include "RegisterTask.h"
#include <string.h>
char *appName = "Proxy";

int main()
{
    threadEnvCreate();
    InitLogInfo(File, Info, 500, "log/Proxy_");

    TFRAMEManager* manager = NULL;
    
    Receiver * recv = NULL;
//    SipStackPsa*   psa     = NULL;
    //TProxyTask*    proxy   = NULL;
    RegisterTask*  regtask = NULL;
    Sender* send = NULL;

    manager=    new TFRAMEManager();
    //psa=        new SipStackPsa();
    //proxy=      new TProxyTask();
    regtask=	new RegisterTask();
    recv = new Receiver();
    send = new Sender();
    if(manager==NULL|| recv==NULL )
    {
	UniERROR("Init CallServer Error!");
	if(manager) delete manager;
	if(regtask) delete regtask;
//	if(proxy) delete proxy;
	if(recv) delete recv;
        if(send) delete send;
	exit(0);
    }

    manager->registerTask(1001, regtask);
    manager->registerTask(102, send);
    manager->registerTask(101, recv);
    //manager.registerTask(9999, test);

    manager->Init(1, appName);
    manager->Run();

    //if(test!=NULL) delete test;
    if(recv!=NULL)  delete recv;
    if(regtask!=NULL)  delete regtask;
    if(send!=NULL)  delete send;
    //if(proxy!=NULL) delete proxy;

    return 0;
   /* recv = new Receiver();

    if(manager == NULL|| recv == NULL) 
    {
        if(manager) delete manager;
        if(recv) delete recv;
        exit(0);   
    }    
    manager->registerTask(101, recv);
    manager->Init(1, appName);
    manager->Run();

    if(recv != NULL) delete recv;

    return 0;*/
}
