#include "RegisterTask.h"
#include "cmd5.h"
#include "func.h"

#include <string.h>
#include <iostream>
#include <cstdlib>


using namespace std;

RegisterTask::RegisterTask()
{
	mTimerUpdate = setTimer(TIMER_DB_UPDATE);
}

RegisterTask::~RegisterTask()
{
        
}
BOOL RegisterTask::reloadTaskEnv(CStr& cmd, TiXmlElement* & extend)
{
	CStr scmd[5];
	cmd.split(scmd,5);
	if(!strcmp(scmd[0].c_str(),"reload"))
	{
                onInit(extend);
	}
        return true;
}

BOOL RegisterTask::onInit(TiXmlElement* extend)
{
	TiXmlHandle handle(extend);
	TiXmlElement* task = NULL;
	task = handle.FirstChild("task").Element();
	if(task)
	{
/*
		TiXmlHandle reghandle(task);
		TiXmlElement * binds = NULL;
		binds = psahandle.FirstChild("binds").Element();
		if(binds)
		{
			TiXmlHandle bindhandle(binds);
			TiXmlElement*   bind=NULL;
			bind=bindhandle.FirstChild("bind").Element();
			if(bind)
			{}
		}*/
                cout<<"init regtask"<<endl;
	}
	return TRUE;
}

void RegisterTask::procMsg(std::auto_ptr<TUniNetMsg> msg)
{
	TUniNetMsg* unimsg=msg.get();
	if(!unimsg) return;
	if(!unimsg->hasMsgBody()) return;
        cout<<"test regtask procMsg"<<endl;
	THttpMsgReq *httpbody = NULL;
	httpbody = dynamic_cast<THttpMsgReq *>(unimsg->msgBody);
	CHttpMessage *hmsg = httpbody->hMsg;
        cout<<"register task "<<hmsg->linkId<<endl;
       /* CHttpMessage *resp = new CHttpMessage(hmsg->linkId);
        resp->content = "test response";
        resp->setReq(false);
        TUniNetMsg* reqmsg = new TUniNetMsg();
        THttpMsgReq *reqmsgbody = new THttpMsgReq;
        if(reqmsg)
        {
            reqmsg->msgBody = reqmsgbody;
            reqmsg->setMsgBody();

            reqmsgbody->hMsg = resp;
            reqmsg->tAddr.logAddr = 101;
            sendMsg(reqmsg);
        }*/
        unimsg->tAddr.logAddr = 102;
        sendMsg(msg); 
	return;

}


void RegisterTask::onTimeOut(TTimerKey timerKey, void* para)
{
        if(timerKey == mTimerUpdate)
        {
                mTimerUpdate = setTimer(TIMER_DB_UPDATE);
        } 
}        
