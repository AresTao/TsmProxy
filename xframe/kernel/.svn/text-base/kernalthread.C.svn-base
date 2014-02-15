
#include "kernalthread.h"
#include "framectrl.h"
#include "info.h"

TKernalThread::TKernalThread(TFRAMEControl& kn)
	:mKn(kn),TGeneralThread("kernal-framectrl")
{
}

TKernalThread::~TKernalThread()
{

}

void TKernalThread::onstart()
{
	if(env.logLevel!=mKn.logLevel) env.logLevel=mKn.logLevel;
	if(env.logType!=mKn.logType) env.logType=mKn.logType;
}

void TKernalThread::process()
{
	int i;
	try
	{
		i=10;
		while(i>0)
		{
			mKn.mainLoop();
			i--;			//Loop 100 times
            //		tosleep(10); //to sleep 1 microseconds
		}
		//UniDEBUG("Kernal thread main loop 1000 times.");
		tosleep(1);
	}

	catch (exception& e)
	{
		UniERROR("Unhandled exception: %s" ,e.what());
	}
}

