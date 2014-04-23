#include <iostream>
#include "httpthread.h"
using namespace std;
HttpThread::HttpThread(CHttpServer &server):mServer(server)
{
}

HttpThread::~HttpThread()
{

}
void HttpThread::onstart()
{

}
void HttpThread::process()
{
    while(!isShutdown())
    {
            CFdSet fdset;
            mServer.prepareFdSet(fdset);
            int ret = fdset.selectMilliSeconds(1000);
          
            if(ret>= 0)
            {
            cout<<"process test"<<endl;
                prethreadstart();
                mServer.check(fdset);
            }
            tosleep(1000);
            //mServer.procMsg();
            //mServer.process();
        
    }
}
