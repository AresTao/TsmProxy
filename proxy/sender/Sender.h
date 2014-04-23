/*
 *
 * http 消息接收task
 * 作者：wzt 
 * 时间：20140411
 * */
#ifndef __HTTPSENDER_H
#define __HTTPSENDER_H

#include "abstracttask.h"
#include "httpserver.h"
#include "httpclient.h"
//#include "httpthread.h"
//#include "httphandler.h"
//更新时间戳ID，配置在etc/Proxy_config.xml中
const int UPDATETIMER = 2;

_CLASSDEF(SENDER);
class Sender : public TAbstractTask{
    public:
        Sender();
        virtual ~Sender();

        virtual TAbstractTask* clone()
        {
            return new Sender();
        } 
        virtual BOOL onInit(TiXmlElement* extend);
        virtual void procMsg(std::auto_ptr<TUniNetMsg> msg);
        virtual void onTimeOut(TTimerKey timerKey, void* para);
       // bool add(CHttpMessage *msg);
//        virtual void post();
         
    private:
        CTcpConnectionManager * mClient;
        void WStringToString(const wstring &wstr,string &str);
        //HttpThread * mThread;
        int mReceiver;
        int mTimer;
        int mRegLinkID;
};
#endif
