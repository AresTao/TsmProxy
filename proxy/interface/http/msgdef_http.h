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

#if !defined __MSGDEF_HTTPMSG_H
#define __MSGDEF_HTTPMSG_H

#include "msgconstdef_http.h"
#include "msgdef.h"

#include "message.hpp"
 
using namespace std;

_CLASSDEF(CGFSM);
_CLASSDEF(THttpCtrlMsg);
_CLASSDEF(THttpMsg); 


// TMsgType
const int HTTP_MESSAGE_TYPE = 31;

// THTTPMsgReq
const int HTTP_MESSAGE_REQ = 3100;

// THTTPMsgResp
const int HTTP_MESSAGE_RSP = 3101;

class MSGDEF_HTTP_H_IDName
{
public:
	static const char* n(int id, const char* defaultName)
	{
		switch(id)
		{
			case HTTP_MESSAGE_REQ: return "HTTP_MESSAGE_REQ";
			case HTTP_MESSAGE_RSP: return "HTTP_MESSAGE_RSP";
			default: return defaultName;
		};
	};
};


class THttpCtrlMsg:public TCtrlMsg
{
	public:
		CStr           transid;
		HTTPMETHODTYPE  method;
		INT            status;
		CStr           dialogid;
	//	const char* validPara() { return "transid,method,status,dialogid"; }

		THttpCtrlMsg(){};

		CHAR*          getMsgName(){ return (char*)"THttpCtrlMsg";};
		THttpCtrlMsg    &operator=(const THttpCtrlMsg &r);
		PTCtrlMsg      clone();
		BOOL           operator == (TMsgPara&);

		INT            size();
		INT            encode(CHAR* &buf);
		INT            decode(CHAR* &buf);
		BOOL           decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm);

		void           print(ostrstream& st);
		const CHAR*    getMsgNameById(int id){ return MSGDEF_HTTP_H_IDName::n(id, "THttpCtrlMsg");};
		int            getFieldValue(const char** p, int& type, CStr& value);
		int            setFieldValue(const char** p, int& type, CStr& value);
};


class THttpMsg:public TMsgBody
{
	public:

  		http::server::Message* httpMsg;
  		THttpMsg() {httpMsg=NULL; }
  		THttpMsg(const THttpMsg&);

  		virtual ~THttpMsg(){ if(httpMsg!=NULL) delete httpMsg; httpMsg == NULL;}; ////这个析构函数必须是虚的，才能保证在析构的时候能够调用子类的析构函

                CHAR*          getMsgName(){ return "THttpMsg";};
                THttpMsg        &operator=(const THttpMsg &r);
                PTMsgBody      clone();
                BOOL           operator == (TMsgPara&);

                INT            size();
                INT            encode(CHAR* &buf);
                INT            decode(CHAR* &buf);
                BOOL           decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm);

                void           print(ostrstream& st);

				//释放sipMsg 指针，TUniNetMsg 释放的时候不会删除封装的 SipMessage 指针
			//	resip::SipMessage*   release() { resip::SipMessage* msg=sipMsg; sipMsg=NULL; return msg; }
};

#endif
