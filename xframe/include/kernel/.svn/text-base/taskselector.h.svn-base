/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName:    taskselector.h $
 * System:       xframe
 * SubSystem:    common
 * Author:       Li Jinglin
 * Date：        2010.4.4
 * Description:
		用于管理task，包括task的加载、注册、消息分发处理等。

****************************************************************/
#if !defined(TaskSelector_H)
#define TaskSelector_H

#include <stdio.h>

#include "comtypedef.h"
#include "unihashtable.h"
#include "clist.h"
#include "fifotimelimit.h"
#include "msgdef.h"

_CLASSDEF(TAbstractTask);
_CLASSDEF(TTaskSelector);

class TTaskSelector
{
   public:
      TTaskSelector();	//new selector的时候，要输入FrameCtrl的Task sendTo Fifo
      ~TTaskSelector();
	  void init();

	  BOOL  add(TUniNetMsg* msg);	//发送消息
      BOOL  add(std::auto_ptr<TUniNetMsg> msg);
	  UINT 	registerTask(UINT taskid, UINT instid, TAbstractTask* task);
	  void 	shutdownTask(UINT taskid);
	  void 	shutdownInst(UINT instid);
      void 	unregisterTask(UINT taskid);		//unregister只操作ts中的负荷分担表，具体的对象实例由 framemng自行管理
	  void 	unregisterInst(UINT instid);

      INT 	getActiveTaskCounter() { return activeTaskCounter;}
      INT 	getRegisteredTaskAmount() { return registeredTaskAmount;}

      void	printState();

   private:
	  BOOL  sendMsgToTask(TUniNetMsg* msg);
	  BOOL  multicastMsgToTask(TUniNetMsg* msg);
	  BOOL  broadcastMsgToTask(TUniNetMsg* msg);

      struct Item
      {
            Item(){};
            //Item(int tid, TAbstractTask* ptu) : taskid(tid), tu(ptu), shuttingDown(false) {}
            Item(UINT tid, UINT insid, TAbstractTask* ptu) : taskid(tid), instid(insid), tu(ptu), shuttingDown(false) {}
			UINT 			instid;
            UINT 			taskid;
            TAbstractTask* 	tu;
			UINT			count;
            BOOL 			shuttingDown;
            BOOL operator==(const Item& rhs) { return instid == rhs.instid; }
      };

	  struct TaskState
	  {
			TaskState(){};
			TaskState(UINT tid, CList<UINT>* ts):taskid(tid),inst(ts), instpos(0){};

			UINT 			taskid;
			UINT 			instpos;
			CList<UINT>* 	inst;
			BOOL operator==(const TaskState& rhs) { return taskid == rhs.taskid; }
	  };
		//如果taskid>0, instid=0
		//    由于task 有多个Inst ，因此该方法返回的是一个按照负荷分担选择的Inst 结果
		//    在标记某个inst 处于 shutdown 状态的时候，则不返回该实例，保证shutdown 的实例不会再处理新的对话请求
		//如果taskid>0, instid>0
		//    已经明确inst 的请求，一般是后继请求，还能选择标记为shutdown的实例，保证实例友好的结束
      TTaskSelector::Item*	getTask(UINT taskid, UINT instid);

      BOOL	exists(UINT taskid);

      //CHashTable<int, Item> mTaskList;	//任务列表，主键为TaskID，内容为Item的内容
      CHashTable<UINT, Item*> 		mInstList;	//任务列表，主键为InstanceID，内容为Item的内容
      CHashTable<UINT, TaskState*> 	mTaskList;  //按照TaskId 管理的任务列表，主键为TaskID，内容为Item的内容数组

      INT  	activeTaskCounter;		//活动的任务数量
	  INT  	activeInstCounter;
	  INT  	registeredTaskAmount;	//注册的任务数量
	  INT  	registeredInstAmount;	//注册的实例数量

      INT  	sendMsgToTaskCount;
      INT  	multicastMsgCount;
      INT 	broadcastMsgCount;
};

#endif
