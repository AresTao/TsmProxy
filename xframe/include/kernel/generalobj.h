/******************************************************************************
*Copyright(c) 2011,by BUPT
*All rights reserved

*FileName:     generalobj.h
*System:       xframe
*SubSystem:    common
*Author:       Li Jinglin
*Date：        2010.4.4
*Version：      1.0
*Description：
     the declaration for TGeneralObject
 *
 * Last Modified:

*******************************************************************************/

#ifndef _GENERALOBJ_H_
#define _GENERALOBJ_H_

#include "comtypedef_vchar.h"
#include "comtypedef.h"
#include "comconst.h"

_CLASSDEF(TGeneralObject);
class TGeneralObject
{
public:
   TGeneralObject();
   virtual      ~TGeneralObject(){};
   TObjectType  getObjectType();
   CHAR*        getObjectTypeStr();
   void         putObjectType(TObjectType type);
   const CHAR*  getObjectNameStr(){ objectName.c_str();}

   void errorLog(const char* fmt, ...);
   void print(const char* fmt, ...);

   virtual const char*  objectTypeName() { return "general"; }

   int                  logType;
   int                  logLevel;

protected:
   TObjectType  objectType;     //组件类型，如General, Kernal, Task, PSA
   CStr			objectName;		//组件名字
};

#endif

