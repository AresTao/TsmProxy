#include "info.h"
#include "taskthread.h"

TTaskThread::TTaskThread(TAbstractTask& task, const char* tname)
	:mTask(task),TGeneralThread(tname)
{
}

TTaskThread::~TTaskThread()
{

}

void
TTaskThread::onstart()
{
	if(env.logLevel!=mTask.logLevel) env.logLevel=mTask.logLevel;
	if(env.logType!=mTask.logType) env.logType=mTask.logType;
}

void
TTaskThread::process()
{
	int i;
	try
	{
		i=10;
		while(i)
		{
            //UniDEBUG("task %d Thread Loops", mTask.getTaskId());
			mTask.process();
			i--;
            //		tosleep(10); //to sleep 1 microseconds
		}
		//UniDEBUG("task %d Thread Loops 1000 times.", mTask.getTaskId());
		tosleep(1);
	}

	catch (exception& e)
	{
		UniERROR("Unhandled exception: %s" ,e.what());
	}
}

