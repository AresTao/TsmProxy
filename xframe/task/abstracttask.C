
#include "abstracttask.h"
#include "info.h"
#include "comconst.h"
#include "framectrl.h"

TAbstractTask::TAbstractTask():TGeneralObject()
{
    mTaskRxFifo = new Fifo<TMsg>();
	if(mTaskRxFifo==NULL)
	{
		UniERROR("ERROR!!!! Init Task error! taskRxFifo=NULL");
	}
	mTTQ = new TaskTimerQueue(*mTaskRxFifo);
	if(mTTQ==NULL)
	{
		UniERROR("ERROR!!!! Init Task error! taskTimerQueue=NULL");
	}

	mMsgProcCount=0;
	mTimerProcCount=0;
	mInnerProcCount=0;
}

TAbstractTask::~TAbstractTask()
{
	if(mTaskRxFifo!=NULL) 	delete mTaskRxFifo;
	if(mTTQ!=NULL) 			delete mTTQ;
}

BOOL TAbstractTask::init(UINT instid, UINT hostid, UINT taskid, TFRAMEControl* framectrl)
{
	UniINFO(" Init Task: \n");
    	UniINFO("   taskId(logAddr): %d", taskid);
    	UniINFO("   hostId(phyAddr): %d", hostid);
    	UniINFO("   instId: %d", instid);
	mInstId=instid;
	mHostId=hostid;
	mTaskId=taskid;
	mFrameCtrl = framectrl;

	TTaskEnv env(mTaskId);
	env.load();

	CStr task_env;
	env.list(task_env);
	UniINFO(" Task env: %s", task_env.c_str());

	logType=env.getLogType();
	logLevel=env.getLogLevel();
	env.getTaskInfo(objectType, objectName);
	mTME.clear();
	env.getTimerRsc(mTME);
	mDB.clear();
	if(env.getDBRsc(mDB))
		mDB.conn();
	return onInit(env.extend);
}


void TAbstractTask::process()
{
    //TODO:检查定时器的逻辑
	unsigned int nextstep = mTTQ->process();
	if(nextstep>1000) nextstep=1000;	//定时器返回的下一个定时器时间时间间隔，最长不超过1秒

	std::auto_ptr<TMsg> msg=std::auto_ptr<TMsg>(mTaskRxFifo->getNext(nextstep));	//在消息队列最长等待时间是定时器下一个超时时间间隔
	if(msg.get())
	{
		TUniNetMsg* pUniNetMsg = NULL;
		pUniNetMsg=dynamic_cast<TUniNetMsg*>(msg.get());
		if(pUniNetMsg)
		{
			//procMsg(pUniNetMsg);
			msg.release();
			procMsg(std::auto_ptr<TUniNetMsg>(pUniNetMsg));

			mMsgProcCount++;
			return;
		}
		TTimeOutMsg* pTimeOutMsg = NULL;
		pTimeOutMsg=dynamic_cast<TTimeOutMsg*>(msg.get());
		if(pTimeOutMsg)
		{
			onTimeOut(pTimeOutMsg->timerKey, pTimeOutMsg->timerPara);
			//注意不要删掉该消息，该消息会自动删掉，因为用了 auto_ptr。

			mTimerProcCount++;
			return;
		}
		TEventMsg* pEventMsg = NULL;
		pEventMsg=dynamic_cast<TEventMsg*>(msg.get());
		if(pEventMsg)
		{
			switch(pEventMsg->eventID)
			{
				case KERNAL_APP_INIT:
					onStart(pEventMsg->transID);
					break;
				case KERNAL_APP_HEATBEAT:
					onHeartBeat(pEventMsg->transID);
					break;
				case KERNAL_APP_RELOAD:
					onReload(pEventMsg->transID, pEventMsg->eventInfo);
					break;
				case KERNAL_APP_SHUTDOWN:
					onShutdown(pEventMsg->transID);
					break;
				case KERNAL_APP_STATUES:
					onStatues(pEventMsg->transID, pEventMsg->eventInfo);
				default:
					break;
			}
			mInnerProcCount++;
			return;
		}

	}
}


void TAbstractTask::sendMsg(TUniNetMsg* msg)
{
    //将消息放入队列，由kernel进行处理
	msg->sender.taskType=objectType;
	msg->sender.taskID=getTaskId();
	msg->sender.instID=getInstId();

	msg->oAddr.logAddr=getTaskId();
	msg->oAddr.phyAddr=getHostId();
	msg->oAddr.taskInstID=getInstId();

	std::ostrstream st;
	msg->print(st);
	UniDEBUG("Content of the message:%s",st.str());
	st.freeze(0);
	mFrameCtrl->add(msg);
}


void  TAbstractTask::sendMsg(std::auto_ptr<TUniNetMsg> msg)
{
	TUniNetMsg* pUniNetMsg = msg.release();	//释放自动指针，发送消息对象
	if(pUniNetMsg) sendMsg(pUniNetMsg);
}

TTimerKey TAbstractTask::setTimer(TTimeMark timerId, TTimeDelay timerDelay, void* para)
{
	return mTTQ->add(timerId, timerDelay, para);
}

TTimerKey TAbstractTask::setTimer(TTimeMark timerId, void* para)
{
	if(timerId>=0 && timerId<__TASK_TIMER_NUM)
		return setTimer(mTME[timerId].timerId, mTME[timerId].timerDelay, para);
	else
		return 0;
}

void TAbstractTask::onTimeOut(TTimerKey timerKey, void* para)
{
    UniDEBUG("Task %d Time out !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n",mTaskId);

	//不要删除定时器，超时之后，定时器自动删除了
	//用户也不要自行删除para，定时器消息析构的时候会自动删除
}

BOOL TAbstractTask::delTimer( TTimerKey timerKey)
{
	mTTQ->remove(timerKey);	//用户自定义对象会自动删除
    return TRUE;
}

BOOL  TAbstractTask::post(TMsg* msg)
{
	msg->sender.taskType=objectType;
	msg->sender.taskID=getTaskId();
	msg->sender.instID=getInstId();
	return mFrameCtrl->postTo(msg);
}

void TAbstractTask::onStart(UINT transid)
{
	BOOL ret=preTaskStart();

	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_INIT;
		msg->transID=transid;
		if(ret)	msg->status=1;	//1=active
		else msg->status=0;		//0=inactive
	    post(msg);
	}
}

void TAbstractTask::onShutdown(UINT transid)
{

	BOOL ret=preTaskShutdown();

	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_SHUTDOWN;
		msg->transID=transid;
		if(ret)	msg->status=1;	//1=shutdown
		else msg->status=0;		//0=still active

		post(msg);
	}

}

void TAbstractTask::onReload(UINT transid, CStr& cmd)
{
// set logLevel DEBUG     设置信息输出级别为调试
// set logLevel default	  设置信息输出级别为配置文件默认
// reload all			  重新加载所有配置
// reload log             重新加载日志输出级别
// reload db              重新加载数据库配置
// reload timer           重新加载定时器配置
	TTaskEnv env(mTaskId);
	GeneralThreadEnv* pEnv=NULL;
	pEnv=(GeneralThreadEnv*)threadEnvGet();	//更新线程设置

	CStr scmd[5];
	cmd.split(scmd,5);
	if(scmd[0]=="set")
	{
		if(scmd[1]=="logLevel")
		{
			if(scmd[2]=="DEBUG")
			{
				if(pEnv)
				{
					pEnv->logLevel = Debug;
    			}
			}
			else if(scmd[2]=="default")
			{
				if(pEnv)
				{
					pEnv->logLevel = logLevel;
    			}
			}
		}
	}
	else if(scmd[0]=="reload")
	{
		env.load();
		if(scmd[1]=="all" || scmd[1]=="log")
		{
			logType=env.getLogType();
			logLevel=env.getLogLevel();
			if(pEnv)
			{
				pEnv->logLevel = logLevel;
    			pEnv->logType = logType;
			}
		}
		if(scmd[1]=="all" || scmd[1]=="db")
		{
			mDB.clear();
			if(env.getDBRsc(mDB))
				mDB.conn();
		}
		if(scmd[1]=="all" || scmd[1]=="timer")
		{
			mTME.clear();
			env.getTimerRsc(mTME);
		}
	}

	BOOL ret=reloadTaskEnv(cmd, env.extend);

	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_RELOAD;
		msg->transID=transid;
		if(ret) msg->status=1;	//1=reload success
		else msg->status=0; 	//0=reload error
		post(msg);
	}
	return;
}

void TAbstractTask::onHeartBeat(UINT transid)
{
	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_HEATBEAT;
		msg->transID=transid;
		msg->status=1;	//1=active

		post(msg);
	}
	return;
}

void TAbstractTask::onStatues(UINT transid, CStr& cmd)
{
	CStr sts;

	sts<<"TaskID=";
	sts<<mTaskId;
	sts<<", HostID=";
	sts<<mHostId;
	sts<<", InstID=";
	sts<<mInstId;
	sts<<":\r\n";
	sts<<" Proc UniMsg Count=";
	sts<<mMsgProcCount;
	sts<<", TimerOut Count=";
	sts<<mTimerProcCount;
	sts<<", InnerMsg Count=";
	sts<<mInnerProcCount;
	sts<<"\r\n";

	getStatues(cmd, sts);

	TEventMsg* msg=new TEventMsg();
	if(msg)
	{
		msg->eventID=KERNAL_APP_STATUES;
		msg->transID=transid;
		msg->status=1;	//1=get statues
		msg->eventInfo=sts;
		post(msg);
	}
	return;

}

void TAbstractTask::procMsg(TUniNetMsg* msg)
{
    UniDEBUG("Task recieved Msg\n");
    UniDEBUG("%d---->%d\n",msg->oAddr.logAddr,msg->tAddr.logAddr);
}

void TAbstractTask::procMsg(std::auto_ptr<TUniNetMsg> msg)
{
    TUniNetMsg* pUniNetMsg = msg.get();
    if(pUniNetMsg) procMsg(pUniNetMsg);
}


