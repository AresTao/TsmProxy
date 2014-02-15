#if !defined(__TESTRECVTASK_HXX)
#define __TESTRECVTASK_HXX 

#include "comserv/comtypedef.h"
#include "task/abstracttask.h"
#include <map>

_CLASSDEF(TestRecvTask);
class TestRecvTask : public TAbstractTask
{
public:
	TestRecvTask();
	virtual ~TestRecvTask();

	TAbstractTask* clone(){ return (TAbstractTask*)( new TestRecvTask());}
	BOOL onInit(TiXmlElement*	extend);

	void procMsg(std::auto_ptr<TUniNetMsg> msg);
	void onTimeOut(TTimerKey timerKey, void* para);

	int mCount;
};

#endif
