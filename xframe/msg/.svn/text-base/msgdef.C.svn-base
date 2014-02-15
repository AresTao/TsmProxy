/*********************************************************************

 * Copyright (c)2005-2008, by Beijing TeleStar Network Technology Company Ltd.(MT2)
 * All rights reserved.

 * FileName：       msgdef.C
 * System：         XFrame 
 * SubSystem：      Common
 * Author：         Li Jinglin
 * Date：           2011.02.14
 * Version：        1.0
 * Description：
**********************************************************************/
#include "msgdef.h"
#include "info.h"

BOOL TUniNetMsg::operator==(TUniNetMsg& msg)
{
    //如果两者消息体均为空，则比较头部; 否则，只比较消息体
	//PTMsg msg2 =  dynamic_cast<PTMsg>(&msg);
	PTUniNetMsg msg2 =  (PTUniNetMsg)(&msg);
	if(NULL == msgBody && NULL == msg2->msgBody)
	{
		BOOL rt = FALSE;
		if(optionSet == msg2-> optionSet &&  oAddr == msg2->oAddr &&
			 tAddr == msg2->tAddr && dialogType == msg2->dialogType &&
			  msgName == msg2->msgName && msgType == msg2->msgType  )
				   rt = TRUE;

		if(NULL == ctrlMsgHdr && NULL == msg2->ctrlMsgHdr)
		{
			return rt;
		}
		if(ctrlMsgHdr!=NULL && msg2->ctrlMsgHdr!=NULL)
		{
			 return (rt && *ctrlMsgHdr==(*msg2->ctrlMsgHdr));
		}
		return FALSE;
	}
	if(NULL == msgBody || NULL == msg2->msgBody)
		return FALSE;

	//比较消息体
	return *msgBody==(*msg2->msgBody);

}

INT TUniNetMsg::encode(CHAR* &buf)
{
	if(buf == NULL) return 0;

	ENCODE_INT(buf, optionSet)
	oAddr.encode(buf);
	tAddr.encode(buf);
	//ENCODE(dialogType,buf)
	//ENCODE_INT(msgName,buf)
	//ENCODE_INT(msgType,buf)
	if(ctrlMsgHdr != NULL)
		ctrlMsgHdr->encode(buf);
	if(msgBody != NULL)
		msgBody->encode(buf);
	return size();
}

//只解码头部，控制头和消息体需要知道具体的消息对象才能解析
INT TUniNetMsg::decode(CHAR* &buf)
{
	if(buf == NULL) return 0;

	DECODE_INT(optionSet,buf)
	oAddr.decode(buf);
	tAddr.decode(buf);
	//DECODE(dialogType,buf)
	//DECODE_INT(msgName,buf)
	//DECODE_INT(msgType,buf)
	return sizeof(UINT)+sizeof(TDialogType)+sizeof(TUniNetMsgName)+sizeof(TMsgType)+oAddr.size()*2;
}

INT TUniNetMsg::size()
{
	INT iSize = 0;
	iSize=sender.size() + sizeof(UINT)+sizeof(TDialogType)+sizeof(TUniNetMsgName)+sizeof(TMsgType)+oAddr.size()*2;
	if(ctrlMsgHdr!=NULL)
		iSize += ctrlMsgHdr->size();
	if(msgBody!=NULL)
		iSize += msgBody->size();
	return iSize;
}

void TUniNetMsg::print(ostrstream& st)    //Added by LJL 2006-2-14 
{
    st<<"|============TMsg Begin=========="<<endl;
    st<<"|==Common Message Definition==="<<endl; 
    st<<"oAddr:"<<endl;
    st<<"    logAddr="<<oAddr.logAddr<<endl;
    st<<"    phyAddr="<<oAddr.phyAddr<<endl;
    st<<"    taskInstID="<<oAddr.taskInstID<<endl;
    st<<"tAddr:"<<endl;
    st<<"    logAddr="<<tAddr.logAddr<<endl;
    st<<"    phyAddr="<<tAddr.phyAddr<<endl;
    st<<"    taskInstID="<<tAddr.taskInstID<<endl;
    switch(dialogType)
    {
       case DIALOG_BEGIN: 
          st<<"dialogType=DIALOG_BEGIN"<<endl;
          break;
       case DIALOG_CONTINUE:
          st<<"dialogType=DIALOG_CONTINUE"<<endl;
          break;
       case DIALOG_END:
          st<<"dialogType=DIALOG_END"<<endl;
          break;
       case DIALOG_BROADCAST:
          st<<"dialogType=DIALOG_BROADCAST"<<endl;
          break;
       case DIALOG_MULTICAST:
          st<<"dialogType=DIALOG_MULTICAST"<<endl;
          break;
       default:
          st<<"dialogType=ERROR"<<endl;
          break;     
     }
     st<<"msgName="<<msgName<<endl;
 	 CHAR temp[50];
	 sprintf(temp,"0x%x",msgType);
     st<<"msgType="<<temp<<"(="<<msgType<<")"<<endl;
     
     if(hasCtrlMsgHdr())
     {
         st<<"|==Ctrl Message Definition==="<<endl; 
         ctrlMsgHdr->print(st);
     }
     else
     {
         st<<"|==No Ctrl Message Header==="<<endl;                 
     }
     
     if(hasMsgBody())
     {
         st<<"|==Message Body Definition==="<<endl; 
         msgBody->print(st);
     }
     else
     {
         st<<"|==No Message Body==="<<endl;                 
     }    

    st<<"|============TMsg End=========="<<endl;

}


BOOL TUniNetMsg::decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm)
{
	FILL_FIELD_BEGIN
 	FILL_FORCE_NEST(TUniNetMsg,TMsgAddress,oAddr)
 	FILL_FORCE_NEST(TUniNetMsg,TMsgAddress,tAddr)
 	FILL_FORCE_INT(TUniNetMsg,dialogType)
 	FILL_FORCE_INT(TUniNetMsg,msgName)
 	FILL_FORCE_INT(TUniNetMsg,msgType)
	FILL_FIELD_END  

}

INT TCtrlMsg::encode(CHAR* &buf)
{
	return 0;
}

INT TCtrlMsg::decode(CHAR* &buf)
{
	return 0;
}


BOOL TCtrlMsg::decodeFromXML(TiXmlHandle& xmlParser,PCGFSM fsm)
{
	FILL_FIELD_BEGIN
 	FILL_FIELD_END
}
 
 
INT TCtrlMsg::size()
{
	if(hasOrgAddr())
		return orgAddr.size()+sizeof(UINT);
	return sizeof(UINT);
}

void TCtrlMsg::print(ostrstream& st)
{
    if(hasOrgAddr())    //Added by LJL 2006-2-14 
    {
        orgAddr.print(st);
    }
}




