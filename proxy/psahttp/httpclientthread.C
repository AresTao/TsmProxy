#include <iostream>
#include "httpclientthread.h"
using namespace std;
HttpClientThread::HttpClientThread(CTcpConnectionManager &client):mClient(client)
{
}

HttpClientThread::~HttpClientThread()
{

}
void HttpClientThread::onstart()
{

}
void HttpClientThread::process()
{
    while(!isShutdown())
    {
            CFdSet fdset;
            int ret = fdset.selectMilliSeconds(1000);
          
            if(ret>= 0)
            {
            cout<<"process test"<<endl;
                prethreadstart();
            }
            tosleep(1000);
            //mServer.procMsg();
            //mServer.process();
        
    }
}
