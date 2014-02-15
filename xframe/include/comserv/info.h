#ifndef INFO_H
#define INFO_H

#ifndef WIN32
#include <syslog.h>
#include <unistd.h>
#endif

#include <iostream>
#include <stdarg.h>

#include "threadif.h"
#include "threadmutex.h"
#include "threadlock.h"
#include "threadcondition.h"
#include "comconst.h"
#include "env.h"

enum sLogType
{
   Default = 0,
   Cout = 1,
   Both = 2,
   File = 3
};

enum sLogLevel
{
   None = 0,
#ifdef WIN32
   Err = 3,
   Info = 6,
   Debug = 7
#else
   Err,
   Info = LOG_INFO,
   Debug = LOG_DEBUG
#endif
};

typedef enum sLogType 	LogType;
typedef enum sLogLevel	LogLevel;

	//初始化日志，并设置输出方式
	//	输出方式：	type
	//				CInfo::LogType::Cout   屏幕
	//				CInfo::LogType::File   文件
	//	日志级别：	level
	//				CInfo::LogLevel::Err   错误日志
	//				CInfo::LogLevel::Info  日常运行信息日志
	//				CInfo::LogLevel::Debug 调试信息日志
	//	日志文件大小：	单位M，默认500M
	//	日志文件前缀：	logFileName（日志文件前缀，默认UNI.INFO，日志名字形式：UNI.INFO.20110210.AppName.log）
	//					如果文件超过预期的大小，则将旧的文件名字修改为 UNI.INFO.20110210.AppName.log.时间
void InitLogInfo(LogType type, LogLevel level, int logFileLength=0, const char* logFileName = 0);

	//设置日志输出级别
	//	日志级别：	level
	//				CInfo::LogLevel::Err   错误日志
	//				CInfo::LogLevel::Info  日常运行信息日志
	//				CInfo::LogLevel::Debug 调试信息日志
void setLevel(LogLevel level);
void setType(LogType type);
void setFileName(char* logFileName);
void setFileLength(int logFileLength);


//全局设置
#ifndef  _SINGLE_THREAD
static  Mutex           g_loginfoMutex;
#endif
static  LogLevel        g_logLevel;
static  LogType         g_logType;
static  char            g_logFileName[256];
static  int             g_logFileMaxLength;     //10~10000 10M~10G
//g_logFileName 和 g_logFileMaxLength 只能在启动的时候设置


//如果ame有联接，要判断是不是输出到ame，这部分比较复杂，要根据线程id输出指定线程的log
//以后需修改为按照线程id设置tty输出
void toAme(const char *s);

//输出到文件
void toPrint(const char *s, const char* appName);

//输出到屏幕
void toCout(const char *s);

#define LogPRINT(args_)\
{\
   std::ostrstream uniLogStr;\
   GeneralThreadEnv* pEnv=(GeneralThreadEnv*)threadEnvGet();\
   if(pEnv==NULL)\
   {\
     uniLogStr args_ << std::ends << std::endl;\
     if(g_logType>=Both) toPrint(uniLogStr.str(), NULL);\
     if(g_logType<=Both) toCout(uniLogStr.str());\
   }\
   else\
   {\
     uniLogStr << "[" << pEnv->threadName << "]" args_ << std::ends << std::endl;\
     if(pEnv->logType>=Both)\
     {\
        toPrint(uniLogStr.str(), pEnv->threadName);\
     }\
     if(pEnv->logType<=Both) toCout(uniLogStr.str());\
   }\
   uniLogStr.freeze(0);\
}

#define LogPRINT_S(args_)\
{\
  std::ostrstream uniLogStr;\
   uniLogStr args_ << std::ends << std::endl;\
     if(g_logType>=Both) toPrint(uniLogStr.str(), NULL);\
     if(g_logType<=Both) toCout(uniLogStr.str());\
   uniLogStr.freeze(0);\
}

//	日志输出方法
//		UniDEBUG 	调试信息输出
//		UniINFO		运行信息输出
//		UniERROR	错误信息输出
//		UniDEBUG( "error info %s, error info2 %d", mTemp1, mTemp2);
//		LogDEBUG( << "error info" << mTemp1 << "error info2" << mTemp2 <<std::endl);

#define LogDEBUG(args_)\
    {\
        GeneralThreadEnv* pEnv=(GeneralThreadEnv*)threadEnvGet();\
	if(pEnv!=NULL) { if(pEnv->logLevel>=Debug) LogPRINT(args_) }\
	else if(g_logLevel>=Debug) { LogPRINT(<< "DEBUG:" args_) }\
    }

#define LogINFO(args_)\
    {\
    GeneralThreadEnv* pEnv=(GeneralThreadEnv*)threadEnvGet();\
	if(pEnv!=NULL) {if(pEnv->logLevel>=Info) LogPRINT( args_) }\
	else if(g_logLevel>=Info) { LogPRINT(<< "INFO:" args_) }\
    }

#define LogWARNING(args_)  LogPRINT(<< "WARNING:" args_)
#define LogERROR(args_)    LogPRINT(<< "ERROR:"  args_)
#define LogCRITICAL(args_) LogPRINT(<< "CRITICAL:"  args_)

#define LogDEBUG_S(args_)\
    if(g_logLevel>=Debug) { LogPRINT_S(<< "DEBUG:" args_) }
#define LogINFO_S(args_)\
        if(g_logLevel>=Info) { LogPRINT_S(<< "INFO:" args_) }
#define LogWARNING_S(args_)  LogPRINT_S(<< "WARNING:" args_)
#define LogERROR_S(args_)    LogPRINT_S(<< "ERROR:"  args_)
#define LogCRITICAL_S(args_) LogPRINT_S(<< "CRITICAL:"  args_)



#define UniINFO(args_...)\
{\
    std::ostrstream uniLogStr;\
    uniLogStr << "[" << __FILE__ << ":" << __LINE__ << "] " << std::ends;\
    _UniINFO(uniLogStr.str(), args_);\
    uniLogStr.freeze(0);\
}

#define UniDEBUG(args_...)\
{\
    std::ostrstream uniLogStr;\
    uniLogStr << "[" << __FILE__ << ":" << __LINE__ << "] " << std::ends;\
    _UniDEBUG(uniLogStr.str(), args_);\
    uniLogStr.freeze(0);\
}

#define UniERROR(args_...)\
{\
    std::ostrstream uniLogStr;\
    uniLogStr << "[" << __FILE__ << ":" << __LINE__ << "] " << std::ends;\
    _UniERROR(uniLogStr.str(), args_);\
   uniLogStr.freeze(0);\
}

#define UniINFO_(args_)\
{\
	std::ostrstream uniLogStr;\
	uniLogStr << "[" << __FILE__ << ":" << __LINE__ << "] " << std::ends args_;\
	_UniINFO(uniLogStr.str(),"%s");\
	uniLogStr.freeze(0);\
}

#define UniERROR_(args_)\
{\
        std::ostrstream uniLogStr;\
        uniLogStr << "[" << __FILE__ << ":" << __LINE__ << "] " << std::ends args_;\
        _UniINFO(uniLogStr.str(),"%s");\
        uniLogStr.freeze(0);\
}

#define UniDEBUG_(args_)\
{\
        std::ostrstream uniLogStr;\
        uniLogStr << "[" << __FILE__ << ":" << __LINE__ << "] " << std::ends args_;\
        _UniDEBUG(uniLogStr.str(),"%s");\
        uniLogStr.freeze(0);\
}

void _UniDEBUG(const char* pfx, const char* fmt, ...);
void _UniERROR(const char* pfx, const char* fmt, ...);
void _UniINFO(const char* pfx, const char* fmt, ...);


#endif
