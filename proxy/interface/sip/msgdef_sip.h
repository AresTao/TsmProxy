/****************************************************************
 * Copyright (c)2005, by Beijing TeleStar Network Technology Company Ltd.(MT2) 
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code
 
 * FileName:    msgdef.h 
 * Version:        1.0
 * Author:      Li Jinglin
 * Date:        2011.2.10 
 * Description:
 * Last Modified:
****************************************************************/

#if !defined __MSGDEF_SIPMSG_H
#define __MSGDEF_SIPMSG_H

#include "msgconstdef_sip.h"
#include "msgdef.h"
#include "resip/stack/SipMessage.hxx"

using namespace std;

_CLASSDEF(CGFSM);
_CLASSDEF(TSipCtrlMsg);
_CLASSDEF(TSipMsg); 


// TMsgType
const int SIP_MESSAGE_TYPE = 21;

// TSipMsgReq
const int SIP_MESSAGE_REQ = 2100;

// TSipMsgResp
const int SIP_MESSAGE_RSP = 2101;

class MSGDEF_SIP_H_IDName
{
public:
	static const char* n(int id, const char* defaultName)
	{
		switch(id)
		{
			case SIP_MESSAGE_REQ: return "SIP_MESSAGE_REQ";
			case SIP_MESSAGE_RSP: return "SIP_MESSAGE_RSP";
			default: return defaultName;
		};
	};
};


class TSipCtrlMsg:public TCtrlMsg
{
	public:
		CStr           transid;
		SIPMETHODTYPE  method;
		INT            status;
		CStr           dialogid;
		const char* validPara() { return "transid,method,status,dialogid"; }

		TSipCtrlMsg(){};

		CHAR*          getMsgName(){ return (char*)"TSipCtrlMsg";};
		TSipCtrlMsg    &operator=(const TSipCtrlMsg &r);
		PTCtrlMsg      clone();
		BOOL           operator == (TMsgPara&);

		INT            size();
		INT            encode(CHAR* &buf);
		INT            decode(CHAR* &buf);
		BOOL           decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm);

		void           print(ostrstream& st);
		const CHAR*    getMsgNameById(int id){ return MSGDEF_SIP_H_IDName::n(id, "TSipCtrlMsg");};
		int            getFieldValue(const char** p, int& type, CStr& value);
		int            setFieldValue(const char** p, int& type, CStr& value);
};


class TSipMsg:public TMsgBody
{
	public:

  		resip::SipMessage* sipMsg;
  		TSipMsg() {sipMsg=NULL; }
  		TSipMsg(const TSipMsg&);

  		virtual ~TSipMsg(){ if(sipMsg!=NULL) delete sipMsg; sipMsg == NULL;}; ////这个析构函数必须是虚的，才能保证在析构的时候能够调用子类的析构函

                CHAR*          getMsgName(){ return "TSipMsg";};
                TSipMsg        &operator=(const TSipMsg &r);
                PTMsgBody      clone();
                BOOL           operator == (TMsgPara&);

                INT            size();
                INT            encode(CHAR* &buf);
                INT            decode(CHAR* &buf);
                BOOL           decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm);

                void           print(ostrstream& st);

				//释放sipMsg 指针，TUniNetMsg 释放的时候不会删除封装的 SipMessage 指针
				resip::SipMessage*   release() { resip::SipMessage* msg=sipMsg; sipMsg=NULL; return msg; }
};

#endif
