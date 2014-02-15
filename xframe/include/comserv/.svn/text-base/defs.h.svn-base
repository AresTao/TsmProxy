/****************************************************************
 * Copyright (c)2005, by Beijing TeleStar Network Technology Company Ltd.(MT2) 
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code

 * FileName:	defs.h $
 * Version:		1.0
 * Modifier:    hxp
 * Date    :    2005.8.15 

 * Description:

   2005-11-29 v1.2 李静林 修订了格式，保证在Dev-C++中也能编译通过		

****************************************************************/

#ifndef __DEFS_
#define __DEFS_

//#ifndef __DATATYPEDEF
#define _DATATYPEDEF(type,name) \
            typedef type name; \
            typedef name* P##name;\
            typedef name& R##name;\
            typedef name*& RP##name;

#define _ENUMTYPEDEF(name) \
            typedef name* P##name;\
            typedef name& R##name;\
            typedef name*& RP##name;

#define _ARRAYTYPEDEF(type,name,size) \
            typedef type name[size];\
            typedef name* P##name;\
            typedef name& R##name;\
            typedef name*& RP##name;

#define _CLASSDEF(name) \
            class name; \
            typedef name  * P##name;\
            typedef name  & R##name;\
            typedef name  *  & RP##name;\
            typedef const name  * PC##name;\
            typedef const name  & RC##name;

//#endif   // __DATATYPEDEF

#endif

