/*****************************************************************************
 * msgdatadef_sip.C
 * It is an implementation file of message definition.
 * 
 * Note: This file is created automatically by msg compiler tool. 
 *       Please do not modify it.
 * 
 * Created at Thu May  5 11:39:28 2011
.
 * 
 ******************************************************************************/
#include "msgdef_sip.h"
#include "info.h"
#include "msgutil.h"


/////////////////////////////////////////////
//           for class TSipCtrlMsg
/////////////////////////////////////////////
PTCtrlMsg TSipCtrlMsg::clone()
{
	PTSipCtrlMsg amsg = new TSipCtrlMsg();
	amsg->optionSet                 = optionSet;

	amsg->orgAddr                   = orgAddr;

	amsg->transid                   = transid;
	amsg->method                    = method;
	amsg->status                    = status;
	amsg->dialogid                  = dialogid;
	return amsg;
}
TSipCtrlMsg& TSipCtrlMsg::operator=(const TSipCtrlMsg &r)
{
	transid                   = r.transid;
	method                    = r.method;
	status                    = r.status;
	dialogid                  = r.dialogid;
	return *this;
}

BOOL TSipCtrlMsg::operator == (TMsgPara& msg)
{
	COMPARE_MSG_BEGIN(TSipCtrlMsg,msg)

	COMPARE_FORCE_VCHAR(TSipCtrlMsg,transid)
	COMPARE_FORCE_INT(TSipCtrlMsg,method)
	COMPARE_FORCE_INT(TSipCtrlMsg,status)
	COMPARE_FORCE_VCHAR(TSipCtrlMsg,dialogid)

	COMPARE_END
}

INT TSipCtrlMsg::size()
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

INT TSipCtrlMsg::encode(CHAR* &buf)
{
	ENCODE_INT( buf , optionSet )

	if( optionSet & orgAddr_flag )   orgAddr.encode(buf);

	transid.encode(buf);
	ENCODE_ENUM( buf , method )
	ENCODE_INT( buf , status )
	dialogid.encode(buf);

	return size();
}

INT TSipCtrlMsg::decode(CHAR* &buf)
{
	DECODE_INT( optionSet , buf )

	if( optionSet & orgAddr_flag )   orgAddr.decode(buf);

	transid.decode(buf);
	DECODE_ENUM( method, buf )
	DECODE_INT( status, buf )
	dialogid.decode(buf);

	return size();
}

BOOL TSipCtrlMsg::decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm)
{
	FILL_FIELD_BEGIN

	FILL_FORCE_VCHAR(TSipCtrlMsg,transid)
	FILL_FORCE_ENUM(TSipCtrlMsg,SIPMETHODTYPE,method)
	FILL_FORCE_INT(TSipCtrlMsg,status)
	FILL_FORCE_VCHAR(TSipCtrlMsg,dialogid)

	FILL_FIELD_END
}
void TSipCtrlMsg::print(ostrstream& st)
{
	st << "TSipCtrlMsg" << endl;
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

int TSipCtrlMsg::getFieldValue(const char** p, int& type, CStr& value)
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
	UniERROR("TSipCtrlMsg::getFieldValue, no field %s, valid fields: %s", fieldName.c_str(), validPara());
	return -1;
}

int TSipCtrlMsg::setFieldValue(const char** p, int& type, CStr& value)
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
	UniERROR("TSipCtrlMsg::getFieldValue, no field %s, valid fields: %s", fieldName.c_str(), validPara());
	return -1;
}


/////////////////////////////////////////////
//           for class TSipMsg
/////////////////////////////////////////////
TSipMsg::TSipMsg(const TSipMsg& r) {
        //optionSet = r.optionSet;
        sipMsg = new resip::SipMessage();
        *sipMsg = *r.sipMsg;
}

PTMsgBody TSipMsg::clone()
{
	PTSipMsg amsg = new TSipMsg(*this);
	return amsg;
}
TSipMsg& TSipMsg::operator=(const TSipMsg &r)
{
	if (this == &r) return *this;
	resip::SipMessage* pSipMsg = sipMsg;
	sipMsg = new resip::SipMessage(*r.sipMsg);
	delete pSipMsg;
	return *this;
}

BOOL TSipMsg::operator == (TMsgPara& msg)
{
	COMPARE_MSG_BEGIN(TSipMsg,msg)
	COMPARE_FORCE_INT(TSipMsg,sipMsg)
	COMPARE_END
}

INT TSipMsg::size()
{
	INT tmpSize = 0;

	//tmpSize += sizeof(UINT); //ºÃÏñÃ»ÓÐoptionset
	tmpSize += sizeof(sipMsg);

	return tmpSize;
}

INT TSipMsg::encode(CHAR* &buf)
{
	//ENCODE_INT( buf , optionSet )

	return size();
}

INT TSipMsg::decode(CHAR* &buf)
{
	//DECODE_INT( optionSet , buf )

	return size();
}

BOOL TSipMsg::decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm)
{
	FILL_FIELD_BEGIN

//	FILL_FORCE_ENUM(TSipMsg,SIPURITYPE,type)

	FILL_FIELD_END
}

void TSipMsg::print(ostrstream& st)
{
	st << "TSipMsg" << endl;
	//CHAR temp[30];
	//sprintf(temp,"0x%x", optionSet);
	//st << getIndent() << "optionSet  = " << temp << endl;
	st << "sip message info=" << sipMsg->brief() << endl;
}


