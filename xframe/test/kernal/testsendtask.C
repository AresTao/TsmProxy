

#include "testsendtask.h"

TestSendTask::TestSendTask():TAbstractTask()
{
	mCount=1000;
	mSum=0;
}

TestSendTask::~TestSendTask()
{
}
 
BOOL TestSendTask::onInit(TiXmlElement*	extend)
{
	UniINFO("Send Task %d, Inst %d, on Init, count=%d/T2...", getTaskId(), getInstId(), mCount);
	mTK1=setTimer(1);
	mTK2=setTimer(2);
	return TRUE;
}


void TestSendTask::procMsg(std::auto_ptr<TUniNetMsg> msg)
{

}



void TestSendTask::onTimeOut(TTimerKey timerKey, void* para)
{
	if(timerKey==mTK2)
	{
		mTK2=setTimer(2);
	//	UniINFO("Send Task %d, Inst %d, Send msg start... time %d", getTaskId(), getInstId(), time(0));
		int count=mCount;
		while(count)
		{	
			TUniNetMsg* unimsg=new TUniNetMsg();
			unimsg->tAddr.logAddr=1001;
			unimsg->dialogType=DIALOG_BEGIN;
			unimsg->msgName=100;
	
			sendMsg(unimsg);
			count--;
			mSum++;
		}
	}
	else if(timerKey==mTK1)
	{
		UniINFO("Send Task %d, Inst %d, Send msg count %d.", getTaskId(), getInstId(), mSum);
		mTK1=setTimer(1);
	}
}

