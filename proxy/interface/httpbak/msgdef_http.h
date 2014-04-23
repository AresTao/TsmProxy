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

#include "msgdef.h"
#include "httpmessage.h" 
using namespace std;

_CLASSDEF(CGFSM);
_CLASSDEF(THttpCtrlMsg);
_CLASSDEF(THttpMsgReq);

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

//for server receive http request
class THttpMsgReq : public TMsgBody
{
public:
   /* CStr method;
    CStr version;
    CStr path;
    CStr query;
//    CDict header;
    CStr content;*/
    CHttpMessage *hMsg;
    THttpMsgReq(){};
    THttpMsgReq(const THttpMsgReq &r){ hMsg = new CHttpMessage();
        *hMsg = *r.hMsg;    
};
    virtual ~THttpMsgReq(){}; ////这个析构函数必须是虚的，才能保证在析构的时候能够调用子类的析构函

    CHAR*          getMsgName(){ return "THttpMsgReq";};
    THttpMsgReq    &operator=(const THttpMsgReq &r){if(this == &r)
        return *this;
        CHttpMessage *msg = hMsg;
        hMsg = new CHttpMessage(*r.hMsg);
        delete msg;
        return *this;
};
    PTMsgBody      clone(){ };
    BOOL   operator == (TMsgPara&){};

    INT            size(){INT tmpSize = 0;      tmpSize += sizeof(hMsg);
        return tmpSize;};
    INT            encode(CHAR* &buf){};
    INT            decode(CHAR* &buf){};
    BOOL           decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm){};

    void           print(ostrstream& st){ };
};
/*
class THttpMsgResp : public TMsgBody
{
public:
    CStr code;
  //  CDict header;
    CStr content;
};

// for client send http request
class THttpMsgReqOut : public TMsgBody
{
public:
    CStr url;
    //CDict header;
    CStr content;
    CStr download;
};

*/
#endif
