/*****************************************************************************
 * msgdatadef_uninet.h
 * It is a message declaration head file.
 * 
 * Note: This file is created automatically by msg compiler tool. 
 *       Please do not modify it.
 * 
 * Created at Fri Dec 14 13:05:18 2012
.
 * 
 ******************************************************************************/
#ifndef __MSGDATADEF_UNINET_H_
#define __MSGDATADEF_UNINET_H_

#include "comtypedef.h"
#include "comtypedef_vchar.h"
#include "comtypedef_fchar.h"
#include "tinyxml.h"
#include "xmlmsgconvertor.h"
#include "msgutil.h"

_CLASSDEF(CGFSM);


#include <time.h>
#include <strstream>
using namespace std;


class MSGDATADEF_UNINET_H_IDName
{
public:
	static const char* n(int id, const char* defaultName)
	{
		switch(id)
		{
			default: return defaultName;
		};
	};
};


_CLASSDEF(TMsgAddress)
class TMsgAddress:public TMsgPara
{
	public:
		UINT           logAddr;
		UINT           phyAddr;
		UINT           taskInstID;

		inline         TMsgAddress();
		CHAR*          getMsgName(){ return (char*)"TMsgAddress";};
		TMsgAddress    &operator=(const TMsgAddress &r);
		PTMsgPara      clone();
		BOOL           operator == (TMsgPara&);

		INT            size();
		INT            encode(CHAR* &buf);
		INT            decode(CHAR* &buf);
		BOOL           decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm);

		void           print(ostrstream& st);
		const CHAR*    getMsgNameById(int id){ return MSGDATADEF_UNINET_H_IDName::n(id, "TMsgAddress");};
		int            getFieldValue(const char** p, int& type, CStr& value);
		int            setFieldValue(const char** p, int& type, CStr& value);
};


//////////////////////////////////////////////////////
//         the implementations of inline function      
/////////////////////////////////////////////////////////
inline TMsgAddress::TMsgAddress()
{
	logAddr                   = 0;
	phyAddr                   = 0;
	taskInstID                = 0;
}


#endif
 
