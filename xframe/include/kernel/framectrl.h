/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName:    framectrl.h $
 * System:       xframe
 * SubSystem:    common
 * Author:       Li Jinglin
 * Date：        2010.4.4
 * Description:
        在多线程模式下，FrameControl 只完成 Team / Task创建，Feam / Psa创建，定时器管理，消息转发，
		当前时间获取等功能，不进行各模块线程的管理。
		FrameControl 需维护各个Task的msgBuffer，并向各个task提供frameCtrl的msgBuffer。
		各个Task有几个线程处理，由外部的线程管理模块完成。

		framectrl在多线程模式下，是一个线程，轮训多个msgqueue好还是多个线程，每个线程阻塞在msgqueue好？
		可能得需要测试才能决定。

****************************************************************/
#ifndef _CFRAMECONTROL_H
#define _CFRAMECONTROL_H

#include <stdio.h>
#include "generalobj.h"
#include "comtypedef.h"
#include "msgdef.h"
#include "msg.h"
#include "fifo.h"
#include "taskselector.h"

#define __FRAMECONTROL_TASKID 1

_CLASSDEF(TGeneralObject);
_CLASSDEF(TFRAMEControl);
_CLASSDEF(AbstractTask);
_CLASSDEF(TTaskSelector);

class TFRAMEControl:public TGeneralObject
{
public:
    TFRAMEControl(Fifo<TMsg>&	framemngFifo);
    ~TFRAMEControl();
    //TFRAMEControl& operator=(TFRAMEControl&);
	void    init(int logT, int logL) { logType=logT; logLevel=logL;};

    BOOL	add(TMsg* msg);	//外部发消息到邋FrameCtrl
	BOOL	post(TMsg* msg);	//向FrameMng发消息
	BOOL	postTo(TMsg* msg);	//各个Task向FrameMng发消息

    TTaskSelector* getTaskSelector();//{return mTaskSelector;}
    void	printStat(){};
    void	printMsgs(TUniNetMsg * Msg);

public:
	//系统内部调用接口
	BOOL	mainLoop();

private:
    UINT	loopCount;
    UINT	uninetmsgCountS;
    UINT	uninetmsgCountF;
	BOOL	shutdown;

	BOOL	onInit(UINT transid);
	BOOL	onHeartBeat(UINT transid);
	BOOL	onReload(UINT transid);
	BOOL	onShutdown(UINT transid);
	BOOL 	onStatues(UINT transid, CStr& eventinfo);

	TTaskSelector*  		mTaskSelector;
	Fifo<TMsg>*			mFifo; 		//收消息队列缓冲
	Fifo<TMsg>&			mPostFifo;	//发送Event消息和邋OAM 消息的时候，发送到该fifo，该引用由framemng 初始化，用于向framemng 反馈消息

};

#endif

