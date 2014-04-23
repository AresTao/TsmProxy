/*
 *
 * http 消息接收task
 * 作者：wzt 
 * 时间：20140411
 * */
#ifndef __HTTPSTACKPSA_H
#define __HTTPSTACKPSA_H

#include "abstracttask.h"
#include "httpserver.h"
#include "httpthread.h"
#include "httphandler.h"
//更新时间戳ID，配置在etc/Proxy_config.xml中
const int UPDATE = 2;

_CLASSDEF(RECEIVER);
class Receiver : public TAbstractTask, public CHttpHandler{
    public:
        Receiver();
        virtual ~Receiver();

        virtual TAbstractTask* clone()
        {
            return new Receiver();
        } 
        virtual BOOL onInit(TiXmlElement* extend);
        virtual void procMsg(std::auto_ptr<TUniNetMsg> msg);
        virtual void onTimeOut(TTimerKey timerKey, void* para);
       // bool add(CHttpMessage *msg);
        virtual void post();
        
    private:
        wstring StringToWString(const string &str);
        void  WStringToString(const wstring &wstr,string &str);
        CHttpServer * mServer;
        HttpThread * mThread;
        int mTimer;
        int mRegID;
        int mPubID;
        int mNotID;
        int mSubID;
};
#endif
