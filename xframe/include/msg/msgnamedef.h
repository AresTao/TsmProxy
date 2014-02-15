/****************************************************************
 * Copyright (c)2005, by Beijing TeleStar Network Technology Company Ltd.(MT2) 
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code
 
 * FileName:    msgnamedef.h 
 * Version:        1.0
 * Author:      Li Jinglin
 * Date:        2011.2.10 
 * Description:
 * Last Modified:
****************************************************************/

#ifndef __MSGNAMEDEF_H_
#define __MSGNAMEDEF_H_

#include "comtypedef.h"

//===========================================================================
//--------------------- AppInternal msgid ------------------------------------
const TUniNetMsgName  KERNAL_APP_HEATBEAT  = 11;
const TUniNetMsgName  KERNAL_APP_INIT      = 12;	//系统初始化 
const TUniNetMsgName  KERNAL_APP_STARTED   = 13; 	//初始化完成，系统启动
const TUniNetMsgName  KERNAL_APP_STATUES   = 14; 	
const TUniNetMsgName  KERNAL_APP_RELOAD    = 15; 
const TUniNetMsgName  KERNAL_APP_SHUTDOWN  = 16; 
												
//--------------------- OAM msgid ------------------------------------
const TUniNetMsgName OAM_SET_REQ    	= 70;
const TUniNetMsgName OAM_SET_ACK    	= 71;
const TUniNetMsgName OAM_GET_REQ		= 72;
const TUniNetMsgName OAM_GET_RESP		= 73;
const TUniNetMsgName OAM_ALERT_NOTIFY	= 74;

#endif
