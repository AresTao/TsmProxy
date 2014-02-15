/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName:    taskselector.cpp $
 * System:       xframe
 * SubSystem:    common
 * Author:       zhang zhixiang
 * Date��        2011.4.14
 * Description:
		���ڹ���task������task�ļ��ء�ע�ᡢ��Ϣ�ַ������ȡ�

****************************************************************/

#include "taskselector.h"
#include "abstracttask.h"


TTaskSelector::TTaskSelector()
{
}
TTaskSelector::~TTaskSelector()
{
	//�رյ�ʱ�򣬹�ϣ���б����taskָ����framemng�Լ��ͷţ����ﲻҪɾ��������������
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
    sendMsgToTaskCount=0; //���͵�Task����Ϣ��������; by ZZQ
    multicastMsgCount=0; //�ನ��Ϣ��������;��by ZZQ
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
				ts->instpos++;	//���ѡ��task ������ø��ɷֵ���ʽ��instpos ��¼�����µ�inst ѡ��
				if(ts->instpos>=ts->inst->count()) ts->instpos=0;
				if(ts->inst->get(ts->instpos,instid)) 
				{
					item=NULL;
					if(mInstList.get(instid, item)) 
						if(!item->shuttingDown) return item;	//���ɷֵ���ʱ�򣬱��Ϊshutdown ��ʵ���Ͳ�ѡ����
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
	if(!exists(msg->tAddr.logAddr)) //Taskû��ע��
	{
		return FALSE;
	}
	
	Item * item=NULL; 
	//��Ϣ��Я�������ʵ��ID������ζ����Ϣ�Զ�֪������Ŀ�꣬����ϢӦ�ò��Ǵ����Ի���Ϣ������shutdown��Ӱ��
	//��Ϣ��δЯ�������ʵ��ID������Ҫ���ɷֵ�
	item=getTask(msg->tAddr.logAddr, msg->tAddr.taskInstID);
	if(item)
	{
		item->tu->add(msg);  //�ҵ�Task,ֻ�����Task��add �������ɡ���
		item->count++;
       	sendMsgToTaskCount++;
		return TRUE;
	}

 	return FALSE;
}

//�ಥ
BOOL TTaskSelector::multicastMsgToTask(TUniNetMsg* msg)
{
	if(msg==NULL) return FALSE;
	if(!exists(msg->tAddr.logAddr)) //Taskû��ע��
	{
		delete msg;
		return FALSE;
	}
	Item * item=NULL; 		//Task���ر�
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

//�㲥
BOOL TTaskSelector::broadcastMsgToTask(TUniNetMsg* msg)
{
	if(msg==NULL) return FALSE;

	TMsg* newMsg;
	Item * item=NULL; //Task���ر�
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