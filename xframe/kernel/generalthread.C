#include "generalthread.h"
#include <stdio.h>

TGeneralThread::TGeneralThread(const char * tname, int tLogType, int tLogLevel)
	:ThreadIf(tname)
{
    env.logLevel = tLogLevel;
    env.logType = tLogType;
}

TGeneralThread::~TGeneralThread()
{
}

void TGeneralThread::prethreadstart()
{
	onstart();	//subclass init info
	mThreadPID=getPID();
	mThreadName=getName();
    env.threadName = mThreadName.c_str();
	threadEnvSet((void *)&env);	//设置线程全局的env变量，把env设置进去
}



