/******************************************************************************
*Copyright(c) 2005,by BeiJing Telestar Network Technology Company Ltd.(MT2)
*All rights reserved

*FileName:     generalobj.C
*System:       uniframe 
*SubSystem:    common
*Author:       Long Xiangming 
*Date：         2005.09.20
*Version：      1.0
*Description：
     the implementation for TGeneralObject and TGeneralObjectPool
    to support the ADCGlueAIC's operation.   
 *
 * Last Modified:
    2006.06.26, By Long Xiangming.
     修正saveObject()中的一个bug. 
   
*******************************************************************************/

#include <stdio.h>
#include <stdarg.h>

#include "generalobj.h"
#include "info.h"



TGeneralObject::TGeneralObject()
{
   objectType=objtype_Invalid;
}

TObjectType TGeneralObject::getObjectType()
{
   return objectType;
}

CHAR*  TGeneralObject::getObjectTypeStr()
{
   switch(objectType)
   {
      case objtype_General     : return "General";
      case objtype_Kernal      : return "Kernal";
      case objtype_Task        : return "Task";
	  case objtype_Psa         : return "PSA";
      case objtype_Extended    : return "Extended";
      default                  : return "(unknown)";                       
   }
}


void TGeneralObject::putObjectType(TObjectType type)
{
   objectType=type;
   return;
}

void TGeneralObject::errorLog(const char* fmt, ...)
{
   char tempBuffer[8192];
   va_list args;
   va_start(args, fmt);
   vsprintf(tempBuffer, fmt, args);
   va_end(args);
   LogERROR( << tempBuffer);
}

void TGeneralObject::print(const char* fmt, ...)
{
   char tempBuffer[8192];
   va_list args;
   va_start(args, fmt);
   vsprintf(tempBuffer, fmt, args);
   va_end(args);
   LogINFO( << tempBuffer);
}
