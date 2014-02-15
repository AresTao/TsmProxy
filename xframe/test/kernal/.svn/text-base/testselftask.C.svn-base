

#include "testselftask.h"

TestSelfTask::TestSelfTask():TAbstractTask()
{
	mCount=1000;
	mSum=0;
	mRecvCount=0;
}

TestSelfTask::~TestSelfTask()
{
}
 
BOOL TestSelfTask::onInit(TiXmlElement*	extend)
{
	UniINFO("Send Task %d, Inst %d, on Init, count=%d/T2...", getTaskId(), getInstId(), mCount);
	mTK1=setTimer(1);
	mTK2=setTimer(2);
	return TRUE;
}


void TestSelfTask::procMsg(std::auto_ptr<TUniNetMsg> msg)
{
        TUniNetMsg* unimsg=msg.get();
        if(!unimsg) return;

        mRecvCount++;
}



void TestSelfTask::onTimeOut(TTimerKey timerKey, void* para)
{
	if(timerKey==mTK2)
	{
		mTK2=setTimer(2);
	//	UniINFO("Send Task %d, Inst %d, Send msg start... time %d", getTaskId(), getInstId(), time(0));
		int count=mCount;
		while(count)
		{	
			TUniNetMsg* unimsg=new TUniNetMsg();
		UniINFO("Send Task %d, Inst %d, Send msg count %d.", getTaskId(), getInstId(), mSum);
		UniINFO("Send Task %d, Inst %d, Recv msg count %d.", getTaskId(), getInstId(), mRecvCount);
			unimsg->tAddr.logAddr=1002;
			unimsg->dialogType=DIALOG_BEGIN;
			unimsg->msgName=100;
	
			add(unimsg);
			count--;
			mSum++;
		}
	}
	else if(timerKey==mTK1)
	{
		UniINFO("Send Task %d, Inst %d, Send msg count %d.", getTaskId(), getInstId(), mSum);
		UniINFO("Send Task %d, Inst %d, Recv msg count %d.", getTaskId(), getInstId(), mRecvCount);
		mTK1=setTimer(1);
	}
}

