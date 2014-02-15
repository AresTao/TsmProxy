/******************************************************************************
*Copyright(c)2003-2008,by BeiJing Telestar Network Technology Company Ltd.(MT2)
*All rights reserved

*FileName:     comtypedef.h
*System:       UniFrame
*SubSystem:    Common
*Author:       Long Xiangming
*Date：         2005-2008.05.24
*Version：      1.0
*Description：
   基本数据类型定义,用于标准化数据的传输，屏蔽各个不同操作系统与硬件之

   间的差异. 不同的操作系统拥有不同的数据类型的长度，在这里使用统一长度

   的数据类型.

   (从Softswitch系统继承）

 *
 * Last Modified:
 *   2006.06.27, by Long Xiangming
 *     增加TPhysicalAddress定义（由comtypedef_uninet.h中挪过来）

*******************************************************************************/

#ifndef _COMTYPEDEF_H
#define _COMTYPEDEF_H

#include "defs.h"  // _CLASSDEF() etc.
#ifdef WIN32
   #include <time.h>
   #include <sys/timeb.h>
   #include <windef.h>  // type definitions on windows,such as BOOL
   #include <winsock2.h> // struct timeval
#else
   #include <sys/time.h>

#endif

#include <strstream>
using namespace std;

/* 暂时注释掉! 因为SP与应用服务器互联有问题。可能是应用服务器没有进行大小尾处理。
#if (defined(WIN32) || defined(LINUX) )
#ifndef _LITTLE_ENDIAN
#define _LITTLE_ENDIAN
#endif
#endif
*/

typedef char I8;
typedef short int I16;
typedef int I32;

#ifdef WIN32
   typedef long I64;
   typedef unsigned long UI64;
#else
   typedef long long I64;
   typedef unsigned long long UI64;
#endif

typedef unsigned char UI8;
typedef unsigned short int UI16;
typedef unsigned int UI32;


typedef float FLOAT;
typedef double DOUBLE;

/*extended type definition*/
typedef I8      CHAR;
typedef UI8      UCHAR;
typedef UI8      BYTE;
typedef I16      SHORT;
typedef UI16   USHORT;
typedef I32      INT;
typedef UI32   UINT;
typedef I64      LONG;
typedef UI64   ULONG;
typedef UI64   UINT64;

#ifndef WIN32
#ifndef BOOL
typedef UI32 BOOL;
#endif
#endif

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL_VALUE
#define NULL_VALUE 0
#endif


#ifndef NULL
#define NULL 0
#endif

const I8  LengthOfDate=3;
typedef char TDate[LengthOfDate];

const I8 LengthOfDate8=4;
typedef char TDate8[LengthOfDate8];

const I8 LengthOfDate8AndTime=7;
typedef char TDate8AndTime[LengthOfDate8AndTime];

const   I8   LengthOfDateAndTime=6;
typedef I8   TDateAndTime[LengthOfDateAndTime];

struct TTimeFormatted
{
   I32 hour;
   I32 minute;
   I32 second;
   I32 usec; //微秒
};

#ifdef WIN32
typedef DWORD        TTimeVal;
#else
typedef timeval      TTimeVal;
#endif

typedef I32          SINT32;
typedef unsigned int UINT32;
typedef long         TFilePos;
typedef long         TPointer;

typedef I8  TOCTET;
typedef I32 TINTEGER;
typedef I32 TInteger4;

//modified by lxm. 2008.10.21
//增加优先级标志。心跳消息可以作为高优先级消息，一般不会丢掉。
#define code_normal       0
#define code_emergency    1
class CCode
{
public:
   CCode()
   {
      flag = code_normal;
      content=NULL;
      length=0;
   }
   ~CCode()
   {
      //released by user. maybe the content is pointed to static variable.
      //if(content)
      //   delete content;
      //content=NULL;
   }

   void clear()
   {
      length=0;
      if(content==NULL) return;
      content[0]=0;
   }
   INT length;
   CHAR* content;
   UINT flag; //added by lxm. 2008.10.21
};

typedef enum SCommState /*socket状态*/
{
   CLOSE   =0,
   OPEN   =1
} TCommState;

//#ifndef _HP_aCC_NonAA
//using namespace std;
//#endif

//typedef string STRING;
// 定义hash值的类型   如果程序编译成 64位，此值需要改为ULONG
// 如果程序编译成 32位，此值需要改为UINT
typedef UINT HASH_CODE;

typedef I32 TObjectType; //used in generalobj.h

typedef I32 TTime;

typedef I32 TMsgID;
typedef I32 TAppKey;
typedef I32 TFEID;
typedef UINT TPhysicalAddress;

typedef I32 TFSMInstanceID; //-1 stands for wrong number

typedef I32 THowManyNodes;
typedef I32 THowTo;
typedef I32 TMsgStreamNo;
typedef I32 TErrorNum;

typedef I32 TClassID;

enum TSendOrAccept {Send,Accept};
enum TOnHeadOrRear {Head,Rear};
enum TEventR       {Accepted,_Save_,EReport,ECancel,FSMDelete};
enum TErrorType    {OnlyReport,DisplayAndReport,WarningDisplayAndReport};

typedef UINT32 TTimerKey;
typedef UINT32 TTimeMark;
typedef UINT32 TTimeDelay;

struct TTimeMarkExt
{
#define MAX_TIMER_NAME 32
   TTimeMark  timerId;
   CHAR       timerName[MAX_TIMER_NAME];
   TTimeDelay timerDelay;
   INT        resendNum;  //可以使用-1表示不停重发;
};


struct TElapseTimeMark
{
   TElapseTimeMark()
   {
      dayMark=0;
      hourMark=0;
      hourTick=0;
      oneMinuteMark=0;
      oneMinuteTick=0;
      doTimeStepTick=0;
      doTimeStepMark=0;
   }
   I32 dayMark;
   I32 hourMark;
   I32 hourTick;
   I32 oneMinuteMark;
   I32 oneMinuteTick;
   I32 doTimeStepTick;
   I32 doTimeStepMark;
};

enum TUniProcessRole
{
   role_mp,
   role_mrd,
   role_mrdmp,
   role_sc0,
   role_scdaemon
};

enum TProcessState
{
   process_init,
   process_active,
   process_noanswer,
   process_down,
   process_failed //多次重启仍然失败

};

enum TAlertType
{
   alert_msg,
   alert_warning,
   alert_error
};

//最大网络适配器个数
#define MAX_NET_INTERFACES 16


//added by XfZhou, 2007.11.12
//网络适配器地址信息定义。通过调用getLocalHostAddress()返回（func.h)
class TIfCfg
{
   public:
   enum IFSTATUS {UNKOWN,PROMISC,UP,RUNNING,DOWN};
   struct Sifcfg
      {
      CHAR name[16];
      CHAR address[16];
      IFSTATUS status;
   };
   Sifcfg interfaces[MAX_NET_INTERFACES];
   INT size;

   TIfCfg()
   {
      size=0;
   }
};

#define memcpy_safe(DST, SRC, N) \
   if (DST==0) DST = new char[N];\
   else if (SRC==0) return *this; \
   memcpy(DST, SRC, N);

#define memcpy_safec(DST, SRC, N) \
   if (DST==0) DST = new char[N];\
   else if (SRC!=0) memcpy(DST, SRC, N);


typedef UINT         TModuleType;       //模块类型的数据类型定义
typedef UINT         TModuleInst;       //模块实例的数据类型定义
//added by Long Xiangming. 2007.07.19
typedef TModuleType  TModuleID;         //模块ID的数据类型定义

typedef TModuleType  TLogicalAddress;   //系统逻辑地址的数据类型
typedef TModuleInst  TTaskInstanceID;   //任务实例标识的数据类型

typedef UINT         TUniNetMsgType;    //消息类型的数据类型定义
typedef UINT         TUniNetMsgName;    //消息名的数据类型定义

typedef UINT         TIndexID;          //各种ID标识的数据类型定义


enum DBType
{
	DB_MYSQL=0,
	DB_ORACLE=1,
	DB_SQLITE=2
};

//added by LJL
#ifndef OBJTYPE
#define objtype_Invalid 0
#define objsubtype_Invalid 1
#define objtype_General 2
#define objtype_Kernal 3
#define objtype_Task 4
#define objtype_Psa 5
#define objtype_Extended 6
#endif

class STuple
{
	public:
		UINT     taskType;
		UINT 	 taskID;
		UINT 	 instID;

		INT size() { return sizeof(INT)+sizeof(INT)+sizeof(INT);}
		BOOL  operator==(STuple&);
		STuple    &operator=(const STuple &r);
		void      print(ostrstream& st);

};

//==========================================================
// 消息类定义
_CLASSDEF(TMsgPara);
//消息参数基类，所有的消息参数都应该从这里派生
class TMsgPara
{
   public:
      TMsgPara(){};
      virtual ~TMsgPara(){};

      //子类必须实现之
      virtual   INT size() { return 0;}
      virtual   INT encode(CHAR* &buf) { return 0;}
      virtual   INT decode(CHAR* &buf) { return 0;}
      virtual   CHAR* getMsgNameStr() { return getMsgName(); }
      virtual   CHAR* getMsgName(){ return "msgPara";}
      virtual   void  print(ostrstream&)=0;
      virtual   BOOL  operator==(TMsgPara&)=0; //将isEqual换成==操作符，By Long Xiangming. 2006.3.24
      virtual   PTMsgPara cloneMsg(){ return NULL;}    //只在其直接子类如TUniNetMsg中实现，其他子类不须实现
      virtual   PTMsgPara cloneCtrlMsg(){ return NULL;} //只在TUniNetMsg中实现，其他子类不须实现
      int getIndent() {return 0;}//新加的函数，输出的时候用到了。
      void incIndent(){}//新加的函数，输出的时候用到了。
	  void decIndent(){}//新加的函数，输出的时候用到了。
};



inline BOOL  STuple::operator==(STuple& r)
{
	if(taskType==r.taskType && taskID==r.taskID && instID==r.instID) return TRUE;
	else return FALSE;


}

inline STuple  &STuple::operator=(const STuple &r)
{
	taskType = r.taskType;
	taskID   = r.taskID;
	instID   = r.instID;
	return *this;

}


inline void	  STuple::print(ostrstream& st)
{
	st << "    $taskType  = ";
	st << taskType << endl;
	st << "    $taskID = ";
	st << taskID << endl;
	st << "    $instID   = ";
	st << instID << endl;

}


#endif


