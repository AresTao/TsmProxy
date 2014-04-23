#ifndef _REGISTER_TASK_H
#define _REGISTER_TASK_H

#include <vector>

#include "comtypedef.h"
#include "info.h"
#include "abstracttask.h"
#include "cexpirebuffer.h"
#include "msgdef_http.h"


//最大注册用户数
const INT MAXREGNUM = 1000000;
// nonce超时时间戳
const int NONCEOVERTIME = 172800;

const int TIMER_DB_UPDATE=3;

const int DIALOGOVERTIME = 900;

const int TIMER_UPDATETODB = 1800;

//_CLASSDEF(CRegCallService);
class RegisterTask:public TAbstractTask
{
	public:
		RegisterTask();
		virtual	~RegisterTask();

		virtual TAbstractTask* clone(){
		    return (TAbstractTask*) ( new RegisterTask());
		}
		//重载TAbstractTask的处理消息方法
                virtual void procMsg(std::auto_ptr<TUniNetMsg> msg);

                //重载TAbstractTask的初始化方法
		virtual BOOL onInit(TiXmlElement* extend);
                //定时器
                virtual void onTimeOut(TTimerKey timerKey, void* para);
                //reload
                virtual BOOL reloadTaskEnv(CStr& cmd, TiXmlElement* & extend);


		int DefaultExpire;

		int mMaxCallNum;
		int mMaxRegUserNum;
		int mMaxActiveUserNum;

		int mDlgNeedHB;
		int mDlgExpireTime;

//		CRegCallService CS;

	private:
//		CExpireBuffer<CStr, CSIPRegDialogInfo> mRegDialogMap;   //存储dialog的对象容器，以用户名为key
		int	mCurrenttime;  //当前时间
		CStr mPassword;   //服务器密码
                int mTimerUpdate;
};
#endif


