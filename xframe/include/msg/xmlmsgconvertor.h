/*********************************************************************

 * Copyright (c)2005-2008, by Beijing TeleStar Network Technology Company Ltd.(MT2)
 * All rights reserved.

 * FileName：       xmlmsgconvertor.h
 * System：         UniFrame 
 * SubSystem：      Common
 * Author：         Long Xiangming
 * Date：           2006.03.02
 * Version：        1.0
 * Description：
     XML消息到C++消息转换器函数原型。各模块需实现之，以便能进行集成测试。 
     
 *
 * Last Modified:
    
    2006-4-4 使用tinyXML重构消息解码宏
    
    2006-4-10 将控制消息头和消息体的解码宏从每个消息库的.C中统一到本文件中

**********************************************************************/

#ifndef _MYCONVERTER_H
#define _MYCONVERTER_H
#include <stdio.h>
#include "tinyxml.h"
#include "func.h"


//解析控制消息头
#ifndef GEN_CTRLHDR
#define GEN_CTRLHDR(CLASSNAME)\
{\
	TiXmlHandle temphandle=xmlParser.FirstChildElement("ctrlMsgHdr");\
	if(temphandle.Node())\
	{\
		TiXmlHandle ctrlhandle=temphandle.FirstChildElement(#CLASSNAME);\
		if(ctrlhandle.Node())\
		{\
			msg->ctrlMsgHdr=new CLASSNAME;\
			msg->setCtrlMsgHdr();\
			if(msg->ctrlMsgHdr->decodeFromXML(ctrlhandle,fsm)<=0)\
			{\
				 UniERROR("Error %s\n",#CLASSNAME);\
				 return FALSE;\
			}\
		}\
		else\
		{\
			UniERROR("Error GEN_CTRLHDR %s\n",#CLASSNAME);\
			return FALSE;\
		}\
	}\
}	
#endif


//解析消息体，root在TUniNetMsg
#ifndef GEN_MSGBODY
#define GEN_MSGBODY(CLASSNAME)\
{\
		TiXmlHandle msgBodyHandle=bodyhandle.FirstChildElement(#CLASSNAME);\
		if(!msgBodyHandle.Node())\
		{\
				UniERROR("Error Msg ,no msgBody %S\n",#CLASSNAME);\
				return FALSE;\
		}\
		msg->msgBody=new CLASSNAME;\
		msg->setMsgBody();\
		if(msg->msgBody->decodeFromXML(msgBodyHandle,fsm)<=0)\
		{\
				UniERROR("Error decode MsgBody %s\n",#CLASSNAME);\
				return FALSE;\
		}\
}
#endif


#ifndef FILL_FIELD_BEGIN
#define FILL_FIELD_BEGIN\
		CHAR strTemp[512];
#endif

#ifndef FILL_FIELD_END
#define FILL_FIELD_END\
	return TRUE;
#endif

#ifndef FILL_OPTION_CHAR
#define FILL_OPTION_CHAR(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
				variableName = atoi(tempElement->GetText());\
	}
#endif

#ifndef FILL_FORCE_CHAR
#define FILL_FORCE_CHAR(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#variableName);\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
		{\
					variableName = atoi(tempElement->GetText());\
		}\
		else\
		{\
				UniERROR("Error in FILL_FORCE_CHAR(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
   else\
	{\
		UniERROR("Error in FILL_FORCE_CHAR(): The field is forcible but does not exist. msgName:%s, field:%s\n",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif

#ifndef FILL_OPTION_BYTE
#define FILL_OPTION_BYTE(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
				variableName = atoi(tempElement->GetText());\
	}
#endif

#ifndef FILL_FORCE_BYTE
#define FILL_FORCE_BYTE(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#variableName);\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
		{\
					variableName = atoi(tempElement->GetText());\
		}\
		else\
		{\
				UniERROR("Error in FILL_FORCE_BYTE(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
   else\
	{\
		UniERROR("Error in FILL_FORCE_BYTE(): The field is forcible but does not exist. msgName:%s, field:%s\n",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif


#ifndef FILL_OPTION_INT
#define FILL_OPTION_INT(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
				variableName = atoi(tempElement->GetText());\
	}
#endif

#ifndef FILL_FORCE_INT
#define FILL_FORCE_INT(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#variableName);\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
		{\
					variableName = atoi(tempElement->GetText());\
		}\
		else\
		{\
			UniERROR("Error in FILL_FORCE_INT(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
			return FALSE;\
		}\
	}\
   else\
	{\
		UniERROR("Error in FILL_FORCE_INT(): The field is forcible but does not exist. msgName:%s, field:%s\n",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif

#ifndef FILL_OPTION_LONG
#define FILL_OPTION_LONG(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
				variableName = atol(tempElement->GetText());\
	}
#endif

#ifndef FILL_FORCE_LONG
#define FILL_FORCE_LONG(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#variableName);\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
		{\
					variableName = atol(tempElement->GetText());\
		}\
		else\
		{\
				UniERROR("Error in FILL_FORCE_LONG(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
   else\
	{\
		UniERROR("Error in FILL_FORCE_LONG(): The field is forcible but does not exist. msgName:%s, field:%s\n",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif


#ifndef FILL_OPTION_BOOL
#define FILL_OPTION_BOOL(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
		{\
			if(strcmp(tempElement->GetText(),"true")==0 || strcmp(tempElement->GetText(),"TRUE")==0)\
					variableName = TRUE;\
			else if(strcmp(tempElement->GetText(),"false")==0 || strcmp(tempElement->GetText(),"FALSE")==0 )\
					variableName = FALSE;\
			else\
				variableName = atoi(tempElement->GetText());\
		}\
	}
#endif

#ifndef FILL_FORCE_BOOL
#define FILL_FORCE_BOOL(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#variableName);\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
		{\
			if(strcmp(tempElement->GetText(),"true")==0 || strcmp(tempElement->GetText(),"TRUE")==0)\
					variableName = TRUE;\
			else if(strcmp(tempElement->GetText(),"false")==0 || strcmp(tempElement->GetText(),"FALSE")==0)\
					variableName = FALSE;\
			else\
				variableName = atoi(tempElement->GetText());\
		}\
		else\
		{\
				UniERROR("Error in FILL_FORCE_BOOL(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
   else\
	{\
		UniERROR("Error in FILL_FORCE_BOOL(): The field is forcible but does not exist. msgName:%s, field:%s\n",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif


#ifndef FILL_OPTION_FLOAT
#define FILL_OPTION_FLOAT(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName = atof(tempElement->GetText());\
	}
#endif

#ifndef FILL_FORCE_FLOAT
#define FILL_FORCE_FLOAT(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName = atof(tempElement->GetText());\
		else\
		{\
				UniERROR("Error in FILL_FORCE_FLOAT(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
  else\
	{\
		UniERROR("Error in FILL_FORCE_FLOAT(): The field is forcible but does not exist. ClassName:%s, field:%s",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif

#ifndef FILL_OPTION_ENUM
#define FILL_OPTION_ENUM(thisClassName,ENUMTYPE,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName =ENUMTYPE(atoi(tempElement->GetText()));\
	}
#endif

#ifndef FILL_FORCE_ENUM
#define FILL_FORCE_ENUM(thisClassName,ENUMTYPE,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName =ENUMTYPE(atoi(tempElement->GetText()));\
		else\
		{\
				UniERROR("Error in FILL_FORCE_ENUM(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
  else\
	{\
		UniERROR("Error in FILL_FORCE_ENUM(): The field is forcible but does not exist. ClassName:%s, field:%s",#thisClassName,#variableName);\
		return FALSE;\
		\
	}
#endif


#ifndef FILL_FORCE_VCHAR
#define FILL_FORCE_VCHAR(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName.SetVarCharContent(tempElement->GetText(),strlen(tempElement->GetText()));\
	}\
  else \
	{\
		UniERROR("Error in FILL_FORCE_VCHAR(): The field is forcible but does not exist. ClassName:%s, field:%s",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif

#ifndef FILL_OPTION_VCHAR
#define FILL_OPTION_VCHAR(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName.SetVarCharContent(tempElement->GetText(),strlen(tempElement->GetText()));\
	}
#endif

#ifndef FILL_FORCE_FCHAR
#define FILL_FORCE_FCHAR(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName.set(tempElement->GetText());\
	}\
  else \
	{\
		UniERROR("Error in FILL_FORCE_FCHAR(): The field is forcible but does not exist. ClassName:%s, field:%s",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif

#ifndef FILL_OPTION_FCHAR
#define FILL_OPTION_FCHAR(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
			variableName.set(tempElement->GetText());\
	}
#endif


#ifndef FILL_FORCE_NEST
#define FILL_FORCE_NEST(superClassName,nestClassName,nestVariableName)\
	if(!xmlParser.FirstChildElement(#nestVariableName).Node()) \
	{\
		UniERROR("Error in FILL_FORCE_NEST(): The nest-field is not optional but does not exist. ClassName:%s, field:%s",#superClassName,#nestVariableName);\
		return FALSE;\
	}\
	else\
	{\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#nestVariableName);\
		tempHandle=tempHandle.FirstChildElement(#nestClassName);\
		if(tempHandle.Node())\
		{\
			if((nestVariableName.decodeFromXML(tempHandle,fsm))==FALSE)\
			{\
				UniERROR("Error in FILL_FORCE_NEST(): calling the nest-class function %s::decodeFromXML(xmlParser,fsm) faild. ClassName:%s, field:%s",#nestClassName,#superClassName,#nestVariableName);\
			}\
		}\
		else\
		{\
			UniERROR("Error in FILL_FORCE_NEST(): no such element %s exist in xml. ClassName:%s, field:%s\n",#nestVariableName,#superClassName,#nestVariableName);\
			return FALSE;\
		}\
	}
#endif

#ifndef FILL_OPTION_NEST
#define FILL_OPTION_NEST(superClassName,nestClassName,nestVariableName)\
	if(xmlParser.FirstChildElement(#nestVariableName).Node())\
	{\
		optionSet |= superClassName::nestVariableName##_flag;\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#nestVariableName);\
		tempHandle=tempHandle.FirstChildElement(#nestClassName);\
		if(tempHandle.Node())\
		{\
			if(nestVariableName.decodeFromXML(tempHandle,fsm)==FALSE)\
			{\
				UniERROR("Error in FILL_OPTION_NEST(): calling the nest-class function %s::decodeFromXML(xmlParser,fsm) faild. ClassName:%s, field:%s\n",#nestClassName,#superClassName,#nestVariableName);\
			}\
		}\
	}
#endif




#ifndef FILL_FORCE_ARRAY
#define FILL_FORCE_ARRAY(thisClassName,variableName,repeatNum)\
if(repeatNum==0)\
	;\
else if(xmlParser.FirstChild(#variableName).Node())\
{\
	for(int i=0;i<repeatNum;i++)\
	{\
		TiXmlHandle tempHandle=xmlParser.ChildElement(#variableName,i);\
		if(!tempHandle.Node()) \
		{\
			UniERROR("Number doesn't match--ClassName:%s;VariableName:%s\n",#thisClassName,#variableName);\
			break;\
		}\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=atoi(tempElement->GetText());\
		else\
		{\
				UniERROR("Error in FILL_FORCE_ARRAY(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
}\
else\
{\
		UniERROR("Error in FILL_FORCE_ARRAY(): The array-field is not optional but does not exist.\n");\
		return FALSE;\
}
#endif

#ifndef FILL_OPTION_ARRAY
#define FILL_OPTION_ARRAY(thisClassName,variableName,repeatNum)\
if((repeatNum>0)&&xmlParser.FirstChild(#variableName).Node())\
{\
	optionSet |= thisClassName::variableName##_flag;\
	for(int i=0;i<repeatNum;i++)\
	{\
		TiXmlHandle tempHandle=xmlParser.ChildElement(#variableName,i);\
		if(!tempHandle.Node()) \
		{\
			UniERROR("Number doesn't match--ClassName:%s;VariableName:%s\n",#thisClassName,#variableName);\
			break;\
		}\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=atoi(tempElement->GetText());\
	}\
}
#endif

//--begin of Adding by LongXiangming. 2007.04.20
#ifndef FILL_FORCE_ENUM_ARRAY
#define FILL_FORCE_ENUM_ARRAY(thisClassName,ENUMTYPE,variableName,repeatNum)\
if(repeatNum==0)\
	;\
else if(xmlParser.FirstChild(#variableName).Node())\
{\
	for(int i=0;i<repeatNum;i++)\
	{\
		TiXmlHandle tempHandle=xmlParser.ChildElement(#variableName,i);\
		if(!tempHandle.Node()) \
		{\
			UniERROR("Number doesn't match--ClassName:%s;VariableName:%s\n",#thisClassName,#variableName);\
			break;\
		}\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=ENUMTYPE(atoi(tempElement->GetText()));\
		else\
		{\
				UniERROR("Error in FILL_FORCE_ARRAY(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
}\
else\
{\
		UniERROR("Error in FILL_FORCE_ARRAY(): The array-field is not optional but does not exist.\n");\
		return FALSE;\
}
#endif

#ifndef FILL_OPTION_ENUM_ARRAY
#define FILL_OPTION_ENUM_ARRAY(thisClassName,ENUMTYPE,variableName,repeatNum)\
if((repeatNum>0)&&xmlParser.FirstChild(#variableName).Node())\
{\
	optionSet |= thisClassName::variableName##_flag;\
	for(int i=0;i<repeatNum;i++)\
	{\
		TiXmlHandle tempHandle=xmlParser.ChildElement(#variableName,i);\
		if(!tempHandle.Node()) \
		{\
			UniERROR("Number doesn't match--ClassName:%s;VariableName:%s\n",#thisClassName,#variableName);\
			break;\
		}\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=ENUMTYPE(atoi(tempElement->GetText()));\
	}\
}
#endif
//--end of Adding by LongXiangming. 2007.04.20

#ifndef FILL_FORCE_BYTE_ARRAY
#define FILL_FORCE_BYTE_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_FORCE_ARRAY(thisClassName,variableName,repeatNum)
#endif
 

#ifndef FILL_OPTION_BYTE_ARRAY
#define FILL_OPTION_BYTE_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_OPTION_ARRAY(thisClassName,variableName,repeatNum)
#endif

#ifndef FILL_FORCE_CHAR_ARRAY
#define FILL_FORCE_CHAR_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_FORCE_ARRAY(thisClassName,variableName,repeatNum)
#endif
 

#ifndef FILL_OPTION_CHAR_ARRAY
#define FILL_OPTION_CHAR_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_OPTION_ARRAY(thisClassName,variableName,repeatNum)
#endif


#ifndef FILL_FORCE_INT_ARRAY
#define FILL_FORCE_INT_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_FORCE_ARRAY(thisClassName,variableName,repeatNum)
#endif
 

#ifndef FILL_OPTION_INT_ARRAY
#define FILL_OPTION_INT_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_OPTION_ARRAY(thisClassName,variableName,repeatNum)
#endif

#ifndef FILL_FORCE_LONG_ARRAY
#define FILL_FORCE_LONG_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_FORCE_ARRAY(thisClassName,variableName,repeatNum)
#endif
 

#ifndef FILL_OPTION_LONG_ARRAY
#define FILL_OPTION_LONG_ARRAY(thisClassName,variableName,repeatNum)\
	FILL_OPTION_ARRAY(thisClassName,variableName,repeatNum)
#endif


#ifndef FILL_FLOAT_FORCE_ARRAY
#define FILL_FLOAT_FORCE_ARRAY(thisClassName,variableName,repeatNum)\
if(repeatNum==0)\
	;\
else if(xmlParser.FirstChild(#variableName).Node())\
{\
	for(int i=0;i<repeatNum;i++)\
	{\
		TiXmlHandle tempHandle=xmlParser.ChildElement(#variableName,i);\
		if(!tempHandle.Node()) \
		{\
			UniERROR("Number doesn't match--ClassName:%s;VariableName:%s\n",#thisClassName,#variableName);\
			break;\
		}\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=atof(tempElement->GetText());\
		else\
		{\
				UniERROR("Error in FILL_FORCE_FLOAT_ARRAY(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
}\
else\
{\
		UniERROR("Error in FILL_FORCE_FLOAT_ARRAY(): The array-field is not optional but does not exist.\n");\
		return FALSE;\
}
#endif

#ifndef FILL_OPTION_FLOAT_ARRAY
#define FILL_OPTION_FLOAT_ARRAY(thisClassName,variableName,repeatNum)\
if((repeatNum>0)&&xmlParser.FirstChild(#variableName).Node())\
{\
	optionSet |= thisClassName::variableName##_flag;\
	for(int i=0;i<repeatNum;i++)\
	{\
		TiXmlHandle tempHandle=xmlParser.ChildElement(#variableName,i);\
		if(!tempHandle.Node()) \
		{\
			UniERROR("Number doesn't match--ClassName:%s;VariableName:%s\n",#thisClassName,#variableName);\
			break;\
		}\
		TiXmlElement* tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=atof(tempElement->GetText());\
	}\
}
#endif

#ifndef FILL_FORCE_NEST_ARRAY
#define FILL_FORCE_NEST_ARRAY(superClassName,nestClassName,nestVariableName,repeatNum)\
if(repeatNum==0)\
	;\
else if(xmlParser.FirstChild(#nestVariableName).Node())\
{\
		int i=0;\
		for(i=0;i<(UINT)(repeatNum);i++)\
		{\
			TiXmlHandle tempHandle=xmlParser.ChildElement(#nestVariableName,i);\
			if(!tempHandle.Node())\
			{\
				UniERROR("Array Number doesn't match in FILL_FORCE_NEST_ARRAY--ClassName:%s;VariableName:%s\n",#superClassName,#nestVariableName);\
				break;\
			}\
			tempHandle=tempHandle.FirstChildElement(#nestClassName);\
			if(tempHandle.Node())\
			{\
				if(nestVariableName[i].decodeFromXML(tempHandle,fsm)<=0)\
				{\
						UniERROR("Error in FILL_FORCE_NEST_ARRAY(): The array-field does not exist. ClassName:%s, field:%s[%d] ",#nestClassName,#nestVariableName,i);\
						return FALSE;\
				}\
			}\
			else \
			{\
					UniERROR("Error in FILL_FORCE_NEST_ARRAY(): The array-field does not exist. ClassName:%s, field:%s[%d] ",#superClassName,#nestVariableName,i);\
					return FALSE;\
			}\
		}\
}else\
{\
		UniERROR("Error in FILL_FORCE_NEST_ARRAY(): The nest-field is not optional but does not exist. ClassName:%s, field:%s(%s)",#superClassName,#nestVariableName);\
		return FALSE;\
}
#endif

#ifndef FILL_OPTION_NEST_ARRAY
#define FILL_OPTION_NEST_ARRAY(superClassName,nestClassName,nestVariableName,repeatNum)\
	if((repeatNum>0)&&xmlParser.FirstChild(#nestVariableName).Node())\
	{\
		optionSet |= superClassName::nestVariableName##_flag;\
		int i=0;\
		for(i=0;i<(UINT)(repeatNum);i++)\
		{\
			TiXmlHandle tempHandle=xmlParser.ChildElement(#nestVariableName,i);\
			if(!tempHandle.Node())\
			{\
				UniERROR("Array Number doesn't match in FILL_OPTION_NEST_ARRAY--ClassName:%s;VariableName:%s\n",#superClassName,#nestVariableName);\
				break;\
			}\
			tempHandle=tempHandle.FirstChildElement(#nestClassName);\
			if(tempHandle.Node())\
			{\
				if(nestVariableName[i].decodeFromXML(tempHandle,fsm)<=0)\
				{\
						UniERROR("Error in FILL_OPTION_NEST_ARRAY(): The array-field does not exist. ClassName:%s, field:%s[%d] ",#nestClassName,#nestVariableName,i);\
						return FALSE;\
				}\
			}\
		}\
	}
#endif

#ifndef FILL_FORCE_VCHAR_ARRAY
#define FILL_FORCE_VCHAR_ARRAY(thisClassName,variableName,repeatNum)\
if(repeatNum==0)\
	;\
else	if(xmlParser.FirstChild(#variableName).Node())\
	{\
			int i;\
			for(i=0;i<repeatNum;i++)\
			{\
					TiXmlHandle  tempHandle=xmlParser.ChildElement(#variableName,i);\
					if(!tempHandle.Node())\
					{\
							UniERROR("Error in FILL_FORCE_VCHAR_ARRAY,some value doesn't exist\n");\
							break;\
					}\
					TiXmlElement*tempElement=tempHandle.Element();\
					if(tempElement)\
						variableName[i].SetVarCharContent(tempElement->GetText(),strlen(tempElement->GetText()));\
			}\
	}\
else\
{\
	UniERROR("Error in FILL_FORCE_VCHAR_ARRAY(): The field is forcible but does not exist. ClassName:%s, field:%s",#thisClassName,#variableName);\
	return FALSE;\
}
#endif

#ifndef FILL_OPTION_VCHAR_ARRAY
#define FILL_OPTION_VCHAR_ARRAY(thisClassName,variableName,repeatNum)\
	if((repeatNum>0)&&xmlParser.FirstChild(#variableName).Node())\
	{\
			optionSet |= thisClassName::variableName##_flag;\
			int i;\
			for(i=0;i<repeatNum;i++)\
			{\
					TiXmlHandle  tempHandle=xmlParser.ChildElement(#variableName,i);\
					if(!tempHandle.Node())\
					{\
							UniERROR("Error in FILL_FORCE_VCHAR_ARRAY,some value doesn't exist\n");\
						break;\
					}\
					TiXmlElement*tempElement=tempHandle.Element();\
					if(tempElement)\
						variableName[i].SetVarCharContent(tempElement->GetText(),strlen(tempElement->GetText()));\
			}\
	}
#endif

#ifndef FILL_FORCE_FCHAR_ARRAY
#define FILL_FORCE_FCHAR_ARRAY(thisClassName,variableName,repeatNum)\
if(repeatNum==0)\
	;\
else	if(xmlParser.FirstChild(#variableName).Node())\
	{\
			int i;\
			for(i=0;i<repeatNum;i++)\
			{\
					TiXmlHandle  tempHandle=xmlParser.ChildElement(#variableName,i);\
					if(!tempHandle.Node())\
					{\
							UniERROR("Error in FILL_FORCE_FCHAR_ARRAY,some value doesn't exist\n");\
							break;\
					}\
					TiXmlElement*tempElement=tempHandle.Element();\
					if(tempElement)\
						variableName[i].set(tempElement->GetText());\
			}\
	}\
else\
{\
	UniERROR("Error in FILL_FORCE_FCHAR_ARRAY(): The field is forcible but does not exist. ClassName:%s, field:%s",#thisClassName,#variableName);\
	return FALSE;\
}
#endif

#ifndef FILL_OPTION_FCHAR_ARRAY
#define FILL_OPTION_FCHAR_ARRAY(thisClassName,variableName,repeatNum)\
	if((repeatNum>0)&&xmlParser.FirstChild(#variableName).Node())\
	{\
			optionSet |= thisClassName::variableName##_flag;\
			int i;\
			for(i=0;i<repeatNum;i++)\
			{\
					TiXmlHandle  tempHandle=xmlParser.ChildElement(#variableName,i);\
					if(!tempHandle.Node())\
					{\
							UniERROR("Error in FILL_FORCE_FCHAR_ARRAY,some value doesn't exist\n");\
						break;\
					}\
					TiXmlElement*tempElement=tempHandle.Element();\
					if(tempElement)\
						variableName[i].set(tempElement->GetText());\
			}\
	}
#endif


#ifndef FILL_OPTION_I64
#define FILL_OPTION_I64(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		optionSet |= thisClassName::variableName##_flag;\
		TiXmlElement* tempElement=xmlParser.FirstChildElement(#variableName).Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=atoll(tempElement->GetText());\
	}
#endif

#ifndef FILL_FORCE_I64
#define FILL_FORCE_I64(thisClassName,variableName)\
	if(xmlParser.FirstChildElement(#variableName).Node())\
	{\
		TiXmlHandle tempHandle=xmlParser.FirstChildElement(#variableName);\
		TiXmlElement*tempElement=tempHandle.Element();\
		if(tempElement&&tempElement->GetText())\
				variableName[i]=atoll(tempElement->GetText());\
		else\
		{\
				UniERROR("Error in FILL_OPTION_ARRAY(): some error occurs,check the xml file. ClassName:%s, field:%s\n",#thisClassName,#variableName);\
				return FALSE;\
		}\
	}\
	else\
	{\
		UniERROR("Error in FILL_FORCE_I64(): The field is forcible but does not exist. ClassName:%s, field:%s",#thisClassName,#variableName);\
		return FALSE;\
	}
#endif
		
#endif
