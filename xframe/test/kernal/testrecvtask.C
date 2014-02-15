

#include "testrecvtask.h"

TestRecvTask::TestRecvTask():TAbstractTask()
{
	mCount=0;
}

TestRecvTask::~TestRecvTask()
{
}
 
BOOL TestRecvTask::onInit(TiXmlElement*	extend)
{
//	UniINFO("Recv Task %d, Inst %d, on Init", getTaskId(), getInstId());
	setTimer(1);
	return TRUE;
}


void TestRecvTask::procMsg(std::auto_ptr<TUniNetMsg> msg)
{

	TUniNetMsg* unimsg=msg.get();
	if(!unimsg) return;
	
	mCount++;
	UniDEBUG("Recv Task %d, Inst %d, Received a msg from: %d", getTaskId(), getInstId(), unimsg->oAddr.logAddr);	
}



void TestRecvTask::onTimeOut(TTimerKey timerKey, void* para)
{
	UniINFO("Recv Task %d, Inst %d, Recv msg count:%d", getTaskId(), getInstId(), mCount);
	UniINFO("MsgQueue deep:%d ",mTaskRxFifo->size());
	setTimer(1);
}

