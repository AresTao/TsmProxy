/*****************************************************************************
 * msg.C
 * It is an implementation file of message definition.
 *
 * Note: This file is created automatically by msg compiler tool.
 *       Please do not modify it.
 *
 * Created at Thu May  5 12:00:51 2011
.
 *
 ******************************************************************************/
#include "msg.h"
#include "info.h"
#include "msgutil.h"

/////////////////////////////////////////////
//           for class TMsg
/////////////////////////////////////////////
TMsg& TMsg::operator=(const TMsg &r)
{
	sender = r.sender;
	return *this;
}
PTMsg TMsg::clone(){
    PTMsg pmsg = new TMsg();
    pmsg->sender = sender;
    return pmsg;
}

BOOL TMsg::operator == (TMsg& msg)
{
	if(sender==msg.sender) return TRUE;
	else return FALSE;
}

INT TMsg::size()
{
	return sender.size();
}


void TMsg::print(ostrstream& st)
{
	st << "TMsg" << endl;
	st << "  $sender  = ";
	sender.print(st);
}

/////////////////////////////////////////////
//           for class TTimeOutMsg
/////////////////////////////////////////////
PTMsg TTimeOutMsg::clone()
{
	PTTimeOutMsg amsg = new TTimeOutMsg();
	amsg->sender					= sender;
	amsg->timerMark                 = timerMark;
	amsg->timerDelay                = timerDelay;
	amsg->timerKey                  = timerKey;
	return amsg;
}
TTimeOutMsg& TTimeOutMsg::operator=(const TTimeOutMsg &r)
{
	sender					  = r.sender;
	timerMark                 = r.timerMark;
	timerDelay                = r.timerDelay;
	timerKey                  = r.timerKey;
	return *this;
}

BOOL TTimeOutMsg::operator == (TTimeOutMsg& msg)
{
	if(sender==msg.sender && timerMark==msg.timerMark && timerDelay==msg.timerDelay && timerKey==msg.timerKey) return TRUE;
	else return FALSE;
}

INT TTimeOutMsg::size()
{
	INT tmpSize = 0;

	tmpSize += sender.size();
	tmpSize += sizeof(INT);
	tmpSize += sizeof(INT);
	tmpSize += sizeof(INT);

	return tmpSize;
}


void TTimeOutMsg::print(ostrstream& st)
{
	TMsg::print(st);
	st << "TTimeOutMsg" << endl;
	st << "  $timerMark  = ";
	st << timerMark << endl;
	st << "  $timerKey   = ";
	st << timerKey << endl;
	st << "  $timerDelay = ";
	st << timerDelay << endl;

}


/////////////////////////////////////////////
//           for class TEventPara
/////////////////////////////////////////////
PTMsg TEventMsg::clone()
{
	PTEventMsg amsg = new TEventMsg();
	amsg->sender					= sender;
	amsg->eventID                   = eventID;
	amsg->transID                   = transID;
	amsg->status					= status;
	amsg->eventInfo                 = eventInfo;
	return amsg;
}
TEventMsg& TEventMsg::operator=(const TEventMsg &r)
{
	sender					  = r.sender;
	eventID                   = r.eventID;
	transID                   = r.transID;
	status                    = r.status;
	eventInfo                 = r.eventInfo;
	return *this;
}

BOOL TEventMsg::operator == (TEventMsg& msg)
{
	if(sender==msg.sender && eventID==msg.eventID && transID==msg.transID && status==msg.status) return TRUE;
	else return FALSE;
}

INT TEventMsg::size()
{
	INT tmpSize = 0;

	tmpSize += sender.size();
	tmpSize += sizeof(UINT);
	tmpSize += sizeof(UINT);
	tmpSize += sizeof(INT);
	tmpSize += eventInfo.size();

	return tmpSize;
}

void TEventMsg::print(ostrstream& st)
{
	TMsg::print(st);
	st << "TEventMsg" << endl;
	st << "  $eventID   = ";
	st << eventID << endl;
	st << "  $transID   = ";
	st << transID << endl;
	st << "  $status   = ";
	st << status << endl;
	st << "  $eventInfo = ";
	st << "\"" << eventInfo.c_str() << "\"" << endl;
}

