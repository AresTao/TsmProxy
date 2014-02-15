/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName：      abstracttask.h
 * System:       xframe
 * SubSystem：     Kernel_Task
 * Author:       Li Jinglin
 * Date：        2010.4.4
 * Version：      1.0
 * Description：
      注意，TAbstractTask是提供给所有应用实现的上层Application的接口。
	  TAbstractTask提供独立的process方法供线程调用，使用time limit fifo接收消息。

********************************************************************/

#ifndef _TABSTRACTTASK_H
#define _TABSTRACTTASK_H

#include <strstream>

#include "comtypedef.h"
#include "comtypedef_vchar.h"
#include "fifo.h"
#include "framectrl.h"
#include "generalobj.h"
#include "msgdef.h"
#include "msg.h"
#include "fifosorted.h"
#include "env.h"
#include "info.h"
#include "timerqueue.h"

using namespace std;

_CLASSDEF(TFRAMEControl);
_CLASSDEF(TAbstractTask);
class TAbstractTask : public TGeneralObject
{
public:
	TAbstractTask();
	virtual ~TAbstractTask();


	//===============================================================================================
	//继承的应用可重载一下方法，以实现应用逻辑
	//================================================================================================
	//系统初始化之后加载，输入配置文件中应用扩展配置信息的指针，应用自行读取并保存到任务中
	virtual BOOL onInit(TiXmlElement*	extend) { return TRUE;}
	//Task 任务完成加载，处理消息之前
	virtual BOOL preTaskStart() {return TRUE;}
	//Task 任务关闭之前
	virtual BOOL preTaskShutdown() { return FALSE;}
	//重新加载配置，输入配置文件中应用扩展配置信息的指针，应用自行读取并保存到任务中
	//  cmd:	输入的命令行；extend: 输入的配置文件指针)
	virtual BOOL reloadTaskEnv(CStr& cmd, TiXmlElement* & extend) { return TRUE;}
	//获取Task当前处理状态
	//  cmd:   输入的命令行；st: 输出的状态
	virtual BOOL getStatues(CStr& cmd, CStr& st) {return TRUE;}

	//子类必须派生一个克隆函数，用于生成对象副本
	virtual TAbstractTask* clone()=0;
	//示例：TAbstractTask* newTask=new TXXTask(mHostId, mTaskId, mFrameCtrl);

	//=================================================================================================
	//任务消息处理函数
	//    包括消息处理函数和消息发送函数
	//procMsg
	//	消息处理函数。子类需要重载之，以实现消息处理功能。
	//输入参数是接收到的msg
	//
	//使用procMsg(TUniNetMsg* msg) 要注意，传入的msg 不能作为消息内容，再发送出去，因为这个指针会被本地释放
	//     如果需要使用转发，则重载procMsg(std::auto_ptr<TMsg>& msg) 是更合适的选择，在转发消息的时候，调用msg.release() 释放自动指针对消息指针的控制权
	virtual void procMsg(TUniNetMsg* msg);
//	{
//        UniINFO("Task recieved Msg\n");
//        UniINFO("%d---->%d\n",msg->oAddr.logAddr,msg->tAddr.logAddr);
//	}
	virtual void procMsg(std::auto_ptr<TUniNetMsg> msg);
//	{
//		TUniNetMsg* pUniNetMsg = msg.get();
//		if(pUniNetMsg) procMsg(pUniNetMsg);
//	}

	//sendMsg
	// 	发出一条消息。
	//输入参数
	//	应用要构造TMsg msg对象。
	// 	msg对象将会被内核自动销毁，上层应用需要保证每次发出的
	// 	msg是一个新的对象，且不能主动销毁之。
	//使用sendMsg(TUniNetMsg* msg) 要注意，msg一定是要new出来的新指针，不要直接用procMsg 传入的msg 指针，这个传入的指针会自动释放的
	//    如果使用sendMsg(std::auto_ptr<TMsg> msg) ，则不必担心，因为自动指针复制会自行移交指针控制权
	void  sendMsg(TUniNetMsg* msg);
	void  sendMsg(std::auto_ptr<TUniNetMsg> msg);


	//定时器操作函数
	//
	//setTimer
	//	设置一个定时器。此方法直接使用。子类无需重载。
	//输入参数:
	//    timerId		为定时器id，指示通过配置文件配置的定时器信息。
	//    timerDelay	为超时时间，单位为毫秒。不携带delay，则从配置文件中自动获取时延。
	//	para			用户自定义参数，如果没有，无需传入此参数
	//返回值:
	//	timerKey		定时器的唯一标识，后继的操作需通过该定时器标识执行。
	TTimerKey setTimer(TTimeMark timerId, TTimeDelay timerDelay, void* para=NULL);
	TTimerKey setTimer(TTimeMark timerId, void* para=NULL);

	//delTimer
	//	根据timerKey删除一个定时器。
	BOOL delTimer(TTimerKey timerKey);

	//onTimeOut
	//	 定时器超时处理函数。
	//输入参数数:
	//    timerKey		定时器标识，setTimer的时候返回的标识
	//	para			用户自定义参数
	virtual void onTimeOut(TTimerKey timerKey, void* para);


   //////////////////////////////////
   ///     成员变量，直接访问.
   //////////////////////////////////
   UINT getTaskId(){return mTaskId;}
   UINT getHostId(){return mHostId;}
   UINT getInstId() { return mInstId; }
   CDB* getDB(int i) {return mDB[i];}


public:

	/////////////////////////////////////////
	//	内部函数，不能使用
	/////////////////////////////////////////

	//由framemng模块调用，线程启动前的对象初始化
	BOOL init(UINT instid, UINT hostid, UINT taskid, TFRAMEControl* framectrl);

	//由线程管理模块调用，线程的主循环
	void  process();

	//由FrameCtrl 模块调用，向任务发送消息
	//应用可重写这个接口，以实现向特定位置
	virtual void  add(TMsg* msg) { mTaskRxFifo->add(msg); }

protected:
	// 特定事件处理。
	void onStart(UINT transid);
	void onShutdown(UINT transid);
	void onReload(UINT transid, CStr& st);
	void onHeartBeat(UINT transid);
	void onStatues(UINT transid, CStr& st);

    Fifo<TMsg>* 	mTaskRxFifo; 	//发送到Task的消息队列缓冲

private:

	BOOL  post(TMsg* msg);

	UINT  mTaskId;
    UINT  mHostId;
    UINT  mInstId;

    UINT  mMsgProcCount;	//外部消息计数
    UINT  mTimerProcCount;	//定时器消息计数
    UINT  mInnerProcCount;	//内部消息计数

    TaskTimerQueue* mTTQ;			//内部定时器对象
	TDBRsc			mDB;		//每个任务默认配置不超过5 个数据库连接，可通过mDB[i] 的方式来访问，i 是配置文件中配置的数据库序号id
 	TTimerRsc 		mTME;		//每个任务默认配置不超过30个定时器

	TFRAMEControl* 	mFrameCtrl;	//控制器对象
};

#endif
