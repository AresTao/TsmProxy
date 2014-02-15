#include "abstracttask.h"
#include "framectrl.h"
#include<time.h>

TFRAMEControl::TFRAMEControl( Fifo<TMsg>&	framemngFifo ):mPostFifo(framemngFifo),shutdown(TRUE),loopCount(0)
{
    mFifo = new Fifo<TMsg>();
	mTaskSelector = new TTaskSelector();
}

TFRAMEControl::~TFRAMEControl()
{
	if(mFifo!=NULL)
	{
		delete mFifo;
	}
}

TTaskSelector* TFRAMEControl::getTaskSelector()
{
	return mTaskSelector;
}

BOOL TFRAMEControl::mainLoop()
{
	loopCount++;
//	std::cout << mFifo->size() << endl;
	//用智能指针会将消息释放掉
	TMsg * msg= mFifo->getNext(1000);	//在消息队列最长等待时间=1000ms
	if(msg)
	{
		TUniNetMsg* pUniNetMsg = NULL;
		pUniNetMsg=dynamic_cast<TUniNetMsg*>(msg);
		if(pUniNetMsg)
		{
			if(mTaskSelector->add(pUniNetMsg)) uninetmsgCountS++;
			else
			{
				uninetmsgCountF++;
				delete pUniNetMsg;
			}
			return TRUE;
		}
		TEventMsg* pEventMsg = NULL;
		pEventMsg=dynamic_cast<TEventMsg*>(msg);
		if(pEventMsg)
		{
			switch(pEventMsg->eventID)
			{
				case KERNAL_APP_INIT:
					return onInit(pEventMsg->transID);
					break;
				case KERNAL_APP_HEATBEAT:
					return onHeartBeat(pEventMsg->transID);
					break;
				case KERNAL_APP_RELOAD:
					return onReload(pEventMsg->transID);
					break;
				case KERNAL_APP_SHUTDOWN:
					return onShutdown(pEventMsg->transID);
					break;
				case KERNAL_APP_STATUES:
					return onStatues(pEventMsg->transID, pEventMsg->eventInfo);
				default:
					break;
			}
			delete pEventMsg;
			pEventMsg=NULL;
			return TRUE;
		}
		delete msg;
		msg=NULL;
	}
    return TRUE;
}



BOOL TFRAMEControl::add(TMsg* pMsg)
{
	mFifo->add(pMsg);
	return TRUE;
}

BOOL TFRAMEControl::postTo(TMsg* pMsg)
{
	if(pMsg)
	{
		mPostFifo.add(pMsg);
		return TRUE;
	}
	else return FALSE;
}

BOOL TFRAMEControl::post(TMsg* pMsg)
{
	if(pMsg)
	{
		pMsg->sender.taskID=__FRAMECONTROL_TASKID;
		pMsg->sender.taskType=objtype_Kernal;
		pMsg->sender.instID=0;
		mPostFifo.add(pMsg);
		return TRUE;
	}
	else return FALSE;
}

void TFRAMEControl::printMsgs(TUniNetMsg* Msg)
{
    if(Msg!=NULL)
    {
        UniINFO("kernal recieved Msg~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
        UniINFO("%d---->%d\n",Msg->oAddr.logAddr,Msg->tAddr.logAddr);
    }
}


BOOL	TFRAMEControl::onInit(UINT transid)
{
	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_INIT;
		msg->transID=transid;
		msg->status=1;	//1=active
		return post(msg);
	}

	shutdown=FALSE;		//收到init 消息后，kernal 模块启动
	return TRUE;
}

BOOL	TFRAMEControl::onHeartBeat(UINT transid)
{
	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_HEATBEAT;
		msg->transID=transid;
		msg->status=1;	//1=active
		return post(msg);
	}
	else return FALSE;
}

BOOL	TFRAMEControl::onReload(UINT transid)
{
	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_RELOAD;
		msg->transID=transid;
		msg->status=1;	//1=sucess
		return post(msg);
	}return TRUE;

}

BOOL	TFRAMEControl::onShutdown(UINT transid)
{

	shutdown=TRUE;		//收到shutdown 消息后，kernal 模块关闭

	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_SHUTDOWN;
		msg->transID=transid;
		msg->status=1;	//1=active
		return post(msg);
	}

	return TRUE;

}

BOOL	TFRAMEControl::onStatues(UINT transid, CStr& eventinfo)
{
	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_STATUES;
		msg->transID=transid;
		msg->status=1;	//1=get statues
		//msg->eventInfo is empty
		return post(msg);
	}
	return TRUE;
}


