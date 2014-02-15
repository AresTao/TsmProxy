/*****************************************************************************
 * msgdatadef_uninet.C
 * It is an implementation file of message definition.
 * 
 * Note: This file is created automatically by msg compiler tool. 
 *       Please do not modify it.
 * 
 * Created at Fri Dec 14 13:05:18 2012
.
 * 
 ******************************************************************************/
#include "msgdatadef.h"
#include "info.h"
#include "msgutil.h"
_CLASSDEF(CGFSM);


/////////////////////////////////////////////
//           for class TMsgAddress
/////////////////////////////////////////////
PTMsgPara TMsgAddress::clone()
{
	PTMsgAddress amsg = new TMsgAddress();
	amsg->logAddr                   = logAddr;
	amsg->phyAddr                   = phyAddr;
	amsg->taskInstID                = taskInstID;
	return amsg;
}
TMsgAddress& TMsgAddress::operator=(const TMsgAddress &r)
{
	logAddr                   = r.logAddr;
	phyAddr                   = r.phyAddr;
	taskInstID                = r.taskInstID;
	return *this;
}

BOOL TMsgAddress::operator == (TMsgPara& msg)
{
	COMPARE_MSG_BEGIN(TMsgAddress,msg)

	COMPARE_FORCE_INT(TMsgAddress,logAddr)
	COMPARE_FORCE_INT(TMsgAddress,phyAddr)
	COMPARE_FORCE_INT(TMsgAddress,taskInstID)

	COMPARE_END
}

INT TMsgAddress::size()
{
	INT tmpSize = 0;

	tmpSize += sizeof(INT);
	tmpSize += sizeof(INT);
	tmpSize += sizeof(INT);

	return tmpSize;
}

INT TMsgAddress::encode(CHAR* &buf)
{
	ENCODE_INT( buf , logAddr )
	ENCODE_INT( buf , phyAddr )
	ENCODE_INT( buf , taskInstID )

	return size();
}

INT TMsgAddress::decode(CHAR* &buf)
{
	DECODE_INT( logAddr, buf )
	DECODE_INT( phyAddr, buf )
	DECODE_INT( taskInstID, buf )

	return size();
}

BOOL TMsgAddress::decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm)
{
	FILL_FIELD_BEGIN

	FILL_FORCE_INT(TMsgAddress,logAddr)
	FILL_FORCE_INT(TMsgAddress,phyAddr)
	FILL_FORCE_INT(TMsgAddress,taskInstID)

	FILL_FIELD_END
}
void TMsgAddress::print(ostrstream& st)
{
	st << "TMsgAddress" << endl;
	st << getIndent() << "$logAddr    = ";
	st << logAddr << endl;
	st << getIndent() << "$phyAddr    = ";
	st << phyAddr << endl;
	st << getIndent() << "$taskInstID = ";
	st << taskInstID << endl;
}

int TMsgAddress::getFieldValue(const char** p, int& type, CStr& value)
{
	if(*p == NULL) return -1;
	CStr fieldName; int arrayIndex;
	getMsgFieldName(p, fieldName, arrayIndex);
	if(fieldName == "logAddr")
	{
		value = logAddr; type = 2; return 1;
	}
	else if(fieldName == "phyAddr")
	{
		value = phyAddr; type = 2; return 1;
	}
	else if(fieldName == "taskInstID")
	{
		value = taskInstID; type = 2; return 1;
	}
	UniERROR("TMsgAddress::getFieldValue, no field %s", fieldName.c_str());
	return -1;
}

int TMsgAddress::setFieldValue(const char** p, int& type, CStr& value)
{
	if(*p == NULL) return -1;
	CStr fieldName; int arrayIndex;
	getMsgFieldName(p, fieldName, arrayIndex);
	if(fieldName == "logAddr")
	{
		logAddr = (UINT)(value.toInt()); return 1;
	}
	else if(fieldName == "phyAddr")
	{
		phyAddr = (UINT)(value.toInt()); return 1;
	}
	else if(fieldName == "taskInstID")
	{
		taskInstID = (UINT)(value.toInt()); return 1;
	}
	UniERROR("TMsgAddress::getFieldValue, no field %s", fieldName.c_str());
	return -1;
}

