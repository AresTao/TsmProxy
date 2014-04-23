#ifndef _HTTPHANDLER_H
#define _HTTPHANDLER_H
#include "fifo.h"
#include "httpmessage.h"

class CHttpHandler
{
    public:
        CHttpHandler();
        ~CHttpHandler();
        bool add(CHttpMessage *msg);
        virtual void post();
        CHttpMessage *getMsg();
    private:
        Fifo<CHttpMessage> *mFifo;
};

#endif
