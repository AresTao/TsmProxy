/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName:    generalthread.h $
 * System:       xframe
 * SubSystem:    common
 * Author:       Li Jinglin
 * Date：        2010.4.4
 * Description:
		公共线程
		所有的应用线程都应该从这一公共线程基础类派生，因为这一线程
	完成了线程环境变量的初始化，完成了线程基本信息的设置。

****************************************************************/

#if !defined(_GENERAL_THREAD_H_)
#define _GENERAL_THREAD_H_

#include "threadif.h"
#include "generalobj.h"
#include "env.h"
#include <stdlib.h>

class TGeneralThread : public ThreadIf
{
public:
	TGeneralThread(const char * tname="TGeneralThread", int tLogType = File, int tLogLevel = Info);
	~TGeneralThread();

    	virtual void process() { tosleep(1000);};
	void prethreadstart();
	virtual void onstart() {
		//used to reload infomation before runnning
		};

public:
	UINT 	mThreadPID;
	CStr	mThreadName;
	GeneralThreadEnv env;
};

#endif

