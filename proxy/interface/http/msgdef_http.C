/*****************************************************************************
 * msgdatadef_http.C
 * It is an implementation file of message definition.
 * 
 * Note: This file is created automatically by msg compiler tool. 
 *       Please do not modify it.
 * 
 * Created at Thu May  5 11:39:28 2011
.
 * 
 ******************************************************************************/
#include "msgdef_http.h"
#include "info.h"
#include "msgutil.h"


/////////////////////////////////////////////
//           for class THttpCtrlMsg
/////////////////////////////////////////////
PTCtrlMsg THttpCtrlMsg::clone()
{
	PTHttpCtrlMsg amsg = new THttpCtrlMsg();
	amsg->optionSet                 = optionSet;

	amsg->orgAddr                   = orgAddr;

	amsg->transid                   = transid;
	amsg->method                    = method;
	amsg->status                    = status;
	amsg->dialogid                  = dialogid;
	return amsg;
}
THttpCtrlMsg& THttpCtrlMsg::operator=(const THttpCtrlMsg &r)
{
	transid                   = r.transid;
	method                    = r.method;
	status                    = r.status;
	dialogid                  = r.dialogid;
	return *this;
}

BOOL THttpCtrlMsg::operator == (TMsgPara& msg)
{
	COMPARE_MSG_BEGIN(THttpCtrlMsg,msg)

	COMPARE_FORCE_VCHAR(THttpCtrlMsg,transid)
	COMPARE_FORCE_INT(THttpCtrlMsg,method)
	COMPARE_FORCE_INT(THttpCtrlMsg,status)
	COMPARE_FORCE_VCHAR(THttpCtrlMsg,dialogid)

	COMPARE_END
}

INT THttpCtrlMsg::size()
{
	INT tmpSize = 0;

	tmpSize += sizeof(UINT); //for optionSet

	if( optionSet & orgAddr_flag )	tmpSize += orgAddr.size();

	tmpSize += transid.size();
	tmpSize += sizeof(INT);
	tmpSize += sizeof(INT);
	tmpSize += dialogid.size();

	return tmpSize;
}

INT THttpCtrlMsg::encode(CHAR* &buf)
{
	ENCODE_INT( buf , optionSet )

	if( optionSet & orgAddr_flag )   orgAddr.encode(buf);

	transid.encode(buf);
	ENCODE_ENUM( buf , method )
	ENCODE_INT( buf , status )
	dialogid.encode(buf);

	return size();
}

INT THttpCtrlMsg::decode(CHAR* &buf)
{
	DECODE_INT( optionSet , buf )

	if( optionSet & orgAddr_flag )   orgAddr.decode(buf);

	transid.decode(buf);
	DECODE_ENUM( method, buf )
	DECODE_INT( status, buf )
	dialogid.decode(buf);

	return size();
}

BOOL THttpCtrlMsg::decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm)
{
	FILL_FIELD_BEGIN

	FILL_FORCE_VCHAR(THttpCtrlMsg,transid)
	FILL_FORCE_ENUM(THttpCtrlMsg,HTTPMETHODTYPE,method)
	FILL_FORCE_INT(THttpCtrlMsg,status)
	FILL_FORCE_VCHAR(THttpCtrlMsg,dialogid)

	FILL_FIELD_END
}
void THttpCtrlMsg::print(ostrstream& st)
{
	st << "THttpCtrlMsg" << endl;
	CHAR temp[30];
	sprintf(temp,"0x%x", optionSet);
	st << getIndent() << "optionSet  = " << temp << endl;
	st << getIndent() << "orgAddr    = ";
	if( optionSet & orgAddr_flag )
	{
		orgAddr.print(st);
	}
	else
		st << "(not present)" << endl;
	st << getIndent() << "$transid   = ";
	st << "\"" << transid.GetVarCharContentPoint() << "\"" << endl;
	st << getIndent() << "$method    = ";
	st << method << endl;
	st << getIndent() << "$status    = ";
	st << status << endl;
	st << getIndent() << "$dialogid  = ";
	st << "\"" << dialogid.GetVarCharContentPoint() << "\"" << endl;
}

int THttpCtrlMsg::getFieldValue(const char** p, int& type, CStr& value)
{
	if(*p == NULL) return -1;
	CStr fieldName; int arrayIndex;
	getMsgFieldName(p, fieldName, arrayIndex);
	if((optionSet & orgAddr_flag) && fieldName == "orgAddr") return orgAddr.getFieldValue(p, type, value);
	if(fieldName == "transid")
	{
		value = transid.GetVarCharContentPoint(); type = 4; return 1;
	}
	else if(fieldName == "method")
	{
		value = method; type = 2; return 1;
	}
	else if(fieldName == "status")
	{
		value = status; type = 2; return 1;
	}
	else if(fieldName == "dialogid")
	{
		value = dialogid.GetVarCharContentPoint(); type = 4; return 1;
	}
	UniERROR("THttpCtrlMsg::getFieldValue, no field %s, valid fields: %s", fieldName.c_str(), validPara());
	return -1;
}

int THttpCtrlMsg::setFieldValue(const char** p, int& type, CStr& value)
{
	if(*p == NULL) return -1;
	CStr fieldName; int arrayIndex;
	getMsgFieldName(p, fieldName, arrayIndex);
	if((optionSet & orgAddr_flag) && fieldName == "orgAddr") return orgAddr.setFieldValue(p, type, value);
	if(fieldName == "transid")
	{
		transid = value.c_str(); return 1;
	}
	else if(fieldName == "method")
	{
		method = (SIPMETHODTYPE)(value.toInt()); return 1;
	}
	else if(fieldName == "status")
	{
		status = (INT)(value.toInt()); return 1;
	}
	else if(fieldName == "dialogid")
	{
		dialogid = value.c_str(); return 1;
	}
	UniERROR("THttpCtrlMsg::getFieldValue, no field %s, valid fields: %s", fieldName.c_str(), validPara());
	return -1;
}


/////////////////////////////////////////////
//           for class THttpMsg
/////////////////////////////////////////////
THttpMsg::THttpMsg(const THttpMsg& r) {
        //optionSet = r.optionSet;
        httpMsg = new http::server::Message();
        *httpMsg = *r.httpMsg;
}

PTMsgBody THttpMsg::clone()
{
	PTHttpMsg amsg = new THttpMsg(*this);
	return amsg;
}
THttpMsg& THttpMsg::operator=(const THttpMsg &r)
{
	if (this == &r) return *this;
	http::server::Message* pHttpMsg = httpMsg;
	httpMsg = new http::server::Message(*r.httpMsg);
	delete pHttpMsg;
	return *this;
}

BOOL THttpMsg::operator == (TMsgPara& msg)
{
	COMPARE_MSG_BEGIN(THttpMsg,msg)
	COMPARE_FORCE_INT(THttpMsg,httpMsg)
	COMPARE_END
}

INT THttpMsg::size()
{
	INT tmpSize = 0;

	//tmpSize += sizeof(UINT); //ºÃÏñÃ»ÓÐoptionset
	tmpSize += sizeof(httpMsg);

	return tmpSize;
}

INT THttpMsg::encode(CHAR* &buf)
{
	//ENCODE_INT( buf , optionSet )

	return size();
}

INT THttpMsg::decode(CHAR* &buf)
{
	//DECODE_INT( optionSet , buf )

	return size();
}

BOOL THttpMsg::decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm)
{
	FILL_FIELD_BEGIN

//	FILL_FORCE_ENUM(TSipMsg,SIPURITYPE,type)

	FILL_FIELD_END
}

void THttpMsg::print(ostrstream& st)
{
	st << "THttpMsg" << endl;
	//CHAR temp[30];
	//sprintf(temp,"0x%x", optionSet);
	//st << getIndent() << "optionSet  = " << temp << endl;
	st << "message info="  << endl;
}


