/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName:    taskselector.cpp $
 * System:       xframe
 * SubSystem:    common
 * Author:       zhang zhixiang
 * Date：        2011.4.14
 * Description:
		用于管理task，包括task的加载、注册、消息分发处理等。

****************************************************************/

#include "taskselector.h"
#include "abstracttask.h"


TTaskSelector::TTaskSelector()
{
}
TTaskSelector::~TTaskSelector()
{
	//关闭的时候，哈希表中保存的task指针由framemng自己释放，这里不要删除，否则会出问题
	mInstList.clear();

	TaskState* ts=NULL;
	mTaskList.reset();
	while(mTaskList.getNext(ts))
		if(ts->inst) ts->inst->clear();
	mTaskList.clear();
}

void TTaskSelector::init()
{
	mTaskList.clear();
	activeTaskCounter=0;
    registeredTaskAmount=0;
	registeredInstAmount=0;
    sendMsgToTaskCount=0; //发送到Task的消息个数？用途 by ZZQ
    multicastMsgCount=0; //多波消息数量？用途？by ZZQ
    broadcastMsgCount =0;
}


BOOL TTaskSelector::registerTask(UINT taskid, UINT instid, TAbstractTask* task)
{
	if(taskid>0 && instid>0)
	{
		Item* item=new Item(taskid, instid, task);
		
		if(!mTaskList.containsKey(taskid))
		{
			CList<UINT>* il=new CList<UINT>();
			il->push_back(instid);
			TaskState* ts=new TaskState(taskid, il);
			
			mTaskList.put(taskid, ts);
			registeredTaskAmount++;
		}
		else
		{
			TaskState* ts=NULL;
			if(mTaskList.get(taskid,ts))
			{
				if(ts!=NULL) ts->inst->push_back(instid);
				else return FALSE;
			}
			else
				return FALSE;
		}

		mInstList.put(instid,item);

		registeredInstAmount++;
		return TRUE;
	}
	return FALSE;
}

void TTaskSelector::shutdownInst(UINT instid)
{
	Item* item=NULL;
	if(mInstList.get(instid,item))
	{
		if(item!=NULL)
            item->shuttingDown=true;
	}
}


void TTaskSelector::shutdownTask(UINT taskid)
{
	TaskState* ts=NULL;
	Item* item=NULL;
	UINT instid=0;
	if(mTaskList.get(taskid,ts))
	{
		if(ts!=NULL)
		{
            if(ts->inst->front(instid))
            {
                while(instid)
                {
                    shutdownInst(instid);
                    if(!ts->inst->next(instid)) instid=0;
                }
            }
        }
	}
}

void TTaskSelector::unregisterInst(UINT instid)
{
	UINT taskid=0;
	Item* item=NULL;
	TaskState* ts=NULL;

	if(mInstList.get(instid,item))
	{
		if(item!=NULL)
		{
			taskid=item->taskid;
			if(mTaskList.get(taskid, ts))
			{
				if(ts!=NULL) ts->inst->remove(instid);
			}
			mInstList.remove(instid);
			registeredInstAmount--;
		}
	}
}

void TTaskSelector::unregisterTask(UINT taskid)
{
	UINT instid=0;
	Item* item=NULL;
	TaskState* ts=NULL;

	if(mTaskList.get(taskid, ts))
	{
		if(ts!=NULL)
		{
			if(ts->inst->front(instid))
			while(instid)
			{
				mInstList.remove(instid);
				registeredInstAmount--;
				if(!ts->inst->next(instid)) instid=0;
			}

			mTaskList.remove(taskid);
			delete ts;
		}
		registeredTaskAmount--;
	}
}

TTaskSelector::Item* TTaskSelector:: getTask(UINT taskid, UINT instid)
{
	TaskState* ts=NULL;
	Item * item=NULL;
	if(taskid>0	&& instid>0 && mInstList.get(instid,item))
        return item;
	if(taskid>0 && instid==0 && mTaskList.get(taskid,ts))
	{
		if(ts)
		{
			for(int i=0;i<ts->inst->count();i++)
			{
				ts->instpos++;	//如果选择task ，则采用负荷分担方式，instpos 记录了最新的inst 选择
				if(ts->instpos>=ts->inst->count()) ts->instpos=0;
				if(ts->inst->get(ts->instpos,instid)) 
				{
					item=NULL;
					if(mInstList.get(instid, item)) 
						if(!item->shuttingDown) return item;	//负荷分担的时候，标记为shutdown 的实例就不选择了
				}
			}
		}
	}
	return NULL;
}

BOOL TTaskSelector::exists(UINT taskid)
{
	return mTaskList.containsKey(taskid);
}


BOOL TTaskSelector::add(TUniNetMsg* msg)
{
	switch(msg->msgType)
    {
    case DIALOG_BROADCAST:
        return broadcastMsgToTask(msg);
        break;
    case DIALOG_MULTICAST:
		return multicastMsgToTask(msg);
        break;
    default:
        return sendMsgToTask(msg);
    }
    return TRUE;
}


BOOL TTaskSelector:: add(std::auto_ptr<TUniNetMsg> msg)
{
    TUniNetMsg* pUniNetMsg = msg.release();
    if(pUniNetMsg) return add(pUniNetMsg);
    return FALSE;
}

BOOL TTaskSelector::sendMsgToTask(TUniNetMsg* msg)
{
	if(msg==NULL) return FALSE;
	if(!exists(msg->tAddr.logAddr)) //Task没被注册
	{
		return FALSE;
	}
	
	Item * item=NULL; 
	//消息中携带具体的实例ID，则意味着消息对端知道具体目标，该消息应该不是创建对话消息，不受shutdown的影响
	//消息中未携带具体的实例ID，则需要负荷分担
	item=getTask(msg->tAddr.logAddr, msg->tAddr.taskInstID);
	if(item)
	{
		item->tu->add(msg);  //找到Task,只需调用Task的add 方法即可。。
		item->count++;
       	sendMsgToTaskCount++;
		return TRUE;
	}

 	return FALSE;
}

//多播
BOOL TTaskSelector::multicastMsgToTask(TUniNetMsg* msg)
{
	if(msg==NULL) return FALSE;
	if(!exists(msg->tAddr.logAddr)) //Task没被注册
	{
		delete msg;
		return FALSE;
	}
	Item * item=NULL; 		//Task被关闭
	TaskState* ts=NULL;
	UINT	instid;
	if(mTaskList.get(msg->tAddr.logAddr, ts))
	{
		ts->inst->reset();
		TMsg* newMsg;
		while(ts->inst->next(instid))
		{
			if(instid>0 && mInstList.get(instid, item))
			{
				newMsg=NULL;
                                newMsg=msg->clone();
				//item->tu->add(msg);
                                if(newMsg)
                                {
                                        item->tu->add(newMsg);
					item->count++;
        			}
			}
		}
        	multicastMsgCount++;
	}
        delete msg;
        return TRUE;
}

//广播
BOOL TTaskSelector::broadcastMsgToTask(TUniNetMsg* msg)
{
	if(msg==NULL) return FALSE;

	TMsg* newMsg;
	Item * item=NULL; //Task被关闭
	mInstList.reset();
    	while(mInstList.getNext(item))
    	{
        	if(item)
        	{
			newMsg=NULL;
                        newMsg=msg->clone();
                        if(newMsg)
                        {
                                //item->tu->add(msg);
                                item->tu->add(newMsg);
                                item->count++;

                        }
		}                                
        }
        broadcastMsgCount++;
	delete msg;
	return TRUE;
}
