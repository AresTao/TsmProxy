/*****************************************************************************
 * msg.h
 * It is a message declaration head file.
 *
 * Note: This file is created automatically by msg compiler tool.
 *       Please do not modify it.
 *
 * Created at Thu May  5 12:00:51 2011
.
 *
 ******************************************************************************/
#ifndef __MSG_H_
#define __MSG_H_

#include "defs.h"
#include "comtypedef.h"
#include "comtypedef_vchar.h"
#include "comtypedef_fchar.h"
#include "tinyxml.h"
#include "xmlmsgconvertor.h"

#include "msgutil.h"
#include "msgnamedef.h"

#include <time.h>
#include <strstream>
using namespace std;

_CLASSDEF(CGFSM);
_CLASSDEF(TMsg);
_CLASSDEF(TTimeOutMsg);
_CLASSDEF(TEventMsg);

class TMsg
{
	public:
		STuple 			sender;

		inline         TMsg();
		virtual	~TMsg() {};

		virtual CHAR*   getMsgName() {return "TMsg";};
		virtual PTMsg   clone();
		virtual BOOL    operator == (TMsg&);
        virtual TMsg& operator=(const TMsg &r);
		virtual INT     size();

		virtual void    print(ostrstream& st);
};



class TTimeOutMsg:public TMsg
{
	public:
		TTimeMark      timerMark;
		TTimerKey      timerKey;
		TTime          timerDelay;
		void*		   timerPara;

		inline         TTimeOutMsg();
		inline	   	   TTimeOutMsg(TTimeMark _timerMark, TTimerKey _timerKey, TTime _timerDelay, void* timerPara=NULL);
		virtual inline		   ~TTimeOutMsg();

		CHAR*          getMsgName(){ return "TTimeOutMsg";};
		TTimeOutMsg    &operator=(const TTimeOutMsg &r);
		PTMsg      	   clone();
		BOOL           operator == (TTimeOutMsg&);

		INT            size();

		void           print(ostrstream& st);

};



class TEventMsg:public TMsg
{
	public:
		UINT           eventID;		//事件ID
		UINT		   transID;		//事务ID
		INT			   status;		//事件状态，一般在返回中提供
		CStr		   eventInfo;	//事件信息
		inline         TEventMsg();
		virtual 	~TEventMsg() {};

		CHAR*          getMsgName(){ return "TEventMsg";};
		TEventMsg     &operator=(const TEventMsg&r);
		PTMsg			clone();
		BOOL           operator == (TEventMsg&);

		INT            size();

		void           print(ostrstream& st);
};


//////////////////////////////////////////////////////
//         the implementations of inline function
/////////////////////////////////////////////////////////
inline TMsg::TMsg()
{
	sender.taskType= objtype_Invalid;
	sender.taskID  = 0;
	sender.instID  = 0;
}


inline TTimeOutMsg::TTimeOutMsg()
{
	timerMark                 = 0;
	timerDelay                = 0;
	timerKey                  = 0;
	timerPara				  = NULL;

}

inline TTimeOutMsg::TTimeOutMsg(TTimeMark _timerMark, TTimerKey _timerKey, TTime _timerDelay, void* _timerPara)
	:timerMark(_timerMark),timerDelay(_timerDelay),timerKey(_timerKey),timerPara(_timerPara)
{

}

inline TTimeOutMsg::~TTimeOutMsg()
{
	if(timerPara!=NULL) delete timerPara;
}

inline TEventMsg::TEventMsg()
{
	eventID                   = 0;
	transID					  = 0;
	status					  = 0;
}


#endif

