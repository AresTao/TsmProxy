#if !defined(__TESTSENDTASK_HXX)
#define __TESTSENDTASK_HXX 

#include "comserv/comtypedef.h"
#include "task/abstracttask.h"
#include <map>

_CLASSDEF(TestSendTask);
class TestSendTask : public TAbstractTask
{
public:
	TestSendTask();
	virtual ~TestSendTask();

	TAbstractTask* clone(){ return (TAbstractTask*)( new TestSendTask());}
	BOOL onInit(TiXmlElement*	extend);

	void procMsg(std::auto_ptr<TUniNetMsg> msg);
	void onTimeOut(TTimerKey timerKey, void* para);
	int mCount;
	int mSum;
	TTimerKey mTK1;
	TTimerKey mTK2;
};

#endif
