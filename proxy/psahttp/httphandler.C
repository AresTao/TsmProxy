#include "httphandler.h"
#include <iostream>

using namespace std;
CHttpHandler::CHttpHandler()
{
    mFifo = new Fifo<CHttpMessage>();
}

CHttpHandler::~CHttpHandler()
{

}
void CHttpHandler::post()
{

}
bool CHttpHandler::add(CHttpMessage *msg)
{
    mFifo->add(msg);
    cout<<"handler fifo"<<mFifo->size()<<endl;
    return true;
}
CHttpMessage *CHttpHandler::getMsg()
{
    CHttpMessage *msg = mFifo->getNext(1000);
    if(msg != NULL)
        return msg;
    else 
        return NULL;
}
