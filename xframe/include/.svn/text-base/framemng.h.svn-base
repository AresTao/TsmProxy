/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName:    framectrl.h $
 * System:       xframe
 * SubSystem:    common
 * Author:       zhangzhixiang
 * Date：        2011.4.25
 * Description:
        XframeManager主要用于task以及psa，framecontrol 的建立，以及各个线程的建立，运行等～～

****************************************************************/
#ifndef _TFRAMEMANAGER_H
#define _TFRAMEMANAGER_H

#include <vector>
#include <cstdio>
#include "msgdef.h"
#include "msg.h"
#include "fifo.h"
#include "clist.h"
#include "unihashtable.h"
#include "env.h"
#include "udpsocket.h"

using namespace std;

_CLASSDEF(TFRAMEManager);
_CLASSDEF(TFRAMEControl);
_CLASSDEF(TKernalThread);
_CLASSDEF(TTaskThread);
_CLASSDEF(TAbstractTask);
_CLASSDEF(TaskTimerQueue);
_CLASSDEF(TEnv);

/********************************************/
// this is for xclient
const int maxLen = 10000;

#pragma pack (1)
struct Command
{
    int type;  //0 present for reload,1 present for status
    int taskId;
    int instId;
    char cmd[100];
    Command():taskId(-1), instId(-1)
    {
        cmd[0] = 0;
    }
};
/********************************************/

struct TTask
{
	UINT			instID;
	UINT			taskID;
	CStr			taskName;
	TAbstractTask* 	task;
	TTaskThread*	thread;
};

//singleton形式的Xframemanager
class TFRAMEManager
{
    //外部调用接口
public:
	TFRAMEManager();
	virtual ~TFRAMEManager();
	static TFRAMEManager * instance();

	//启动之前根据配置文件初始化
	void Init(UINT appID, char* appName);
	//在系统启动之前把应用的任务注册到manager中
	void registerTask(UINT taskID, TAbstractTask* task);
	//系统启动
	void Run();

private:
    //socket for external watch tool
    CUDPSocket socket;

    TFRAMEControl *     framectrl;
    TKernalThread * kernalThread;

	UINT mMaxInst;


	int mLogType;
	int mLogLevel;

	CStr mSCIP;
	UINT	 mSCPort;
	UINT	 mSCHB;

	UINT mAppID;
	CStr mAppName;
	CStr mHostIP;

	CList<TProcRsc>						mProcList;		//配置文件中设置的系统启动列表
	CHashTable<UINT, TAbstractTask *>	mTaskObj;		//应用注册的对象实例列表<UINT taskID, TAbstractTask* task>

    CList<TTask*> 						mInstList;		//按照Inst 管理的实例列表，用于系统启动时按顺序加载和初始化
    CHashTable<UINT, CList<TTask*>*>	mTaskList;		//按照Task 管理的实例列表，可用于系统管理和控制

    Fifo<TMsg> * mFifo; 	//接受应用返回消息的队列

    static TFRAMEManager * _instance;
	UINT	mTransID;

	TaskTimerQueue* mTTQ;			//内部定时器对象
 	TTimerRsc 		mTME;		//每个任务默认配置不超过30个定时器

	BOOL   shutdown;

};

#endif

