#ifndef __FUNC_H_
#define __FUNC_H_

#ifndef WIN32
#include <values.h>
#include <netinet/in.h> //include htons,htonl,etc.
#include <sys/param.h> 
#include <sys/types.h> 
#include <sys/stat.h> 
#include <unistd.h> //include usleep
#include <sys/time.h> //include gettimeofday
//for getLocalHostAddress
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <net/if_arp.h>
#include <sys/resource.h> //include setrlimit
#else
#include <windef.h>
#include <winsock2.h> //include htons,htonl,etc.
#endif

#include <signal.h>  //include kill(), signal
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comconst.h"
#include "comtypedef.h"
#include "comtypedef_vchar.h"
//*****************************************************
//字符串处理相关函数
//*****************************************************
//字符串不分大小写比较
// src > dst：源字符串大于目的字符串 return >0
// src = dst：字符串相等 return 0
// src < dst：源字符串小于目的字符串 return <0
int strnotcasecmp( char* src , char* dst );
void strCopy(char* dest,const char* src,int len);

typedef char TUniqueHexID[25];
//得到一个系统全局唯一的字符串ID(24个字节,16进制形式）（根据当前时间计算）
//可以在一些需要全局唯一标志的地方使用
void get_unique_hexid(char* hexstr);

void intToHex(char* s, UINT n);

//*****************************************************
//时间处理相关函数
//*****************************************************

//将标准时间间隔转换为自定义的结构化时间
void timeIntervalFormatted(time_t, TTimeFormatted &);
#ifdef WIN32
void timeIntervalFormatted(DWORD & startTime, DWORD & endTime,TTimeFormatted & timeConverted);
#else
void timeIntervalFormatted(struct timeval &,struct timeval &,TTimeFormatted & );
#endif 

//从系统中获得当前时间
UINT64 getSystemTime();
UINT64 getTimeMicroSec();
UINT64 getTimeMs();
UINT64 getTimeSecs();

UINT get_currtime_sec();     //以秒计

// "YYYYMMDDHHMMSS\0"
#define MaxTimeLength 14
// "YYYY-MM-DD HH:MM:SS\0"
#define MaxTimeStringLength 19
typedef char TASCTime[MaxTimeLength+1];
typedef char TTimeString[MaxTimeStringLength+1];

//tm to "YYYYMMDDHHMMSS\0"
void tmToASCTime(tm* tmTime,TASCTime ascTime);
//tm to "YYYY-MM-DD HH:MM:SS\0"
void tmToTimeString(tm* tmTime,TTimeString timeString);
// get the current date. format:YYYYMMDD, 8 bytes ASC.
void getASCDate8(char* currentASCDate8);
// get the current date and time. format:YYYYMMDDHHMMSS, 14 bytes ASC.
void getASCDate8AndTime(char* currentASCDate8AndTime);
// get the current date and time. format:YYMMDDHHMM, 10 bytes ASC.
void getASCDate6AndTime4(char* currentASCDate6AndTime);
// get the current date and time. format:YYMMDDHHMMSS, 12 bytes ASC.
void getASCDate6AndTime6(char* currentASCDate6AndTime);


//求一个结构数组的长度
#ifndef ARRAY_SIZE
#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))
#endif

/////////////////////////////////////
////    进程/线程/信号/调度相关函数
/////////////////////////////////////
//added by Long Xiangming. 2006.06.30
void   uniSleep(int millisec); //睡眠一段时间（毫秒）。屏蔽Windows和Linux/Unix差别
void   uni_usleep(int usec);   //微秒

typedef  void (*sig_handler)(int);
//信号处理函数. 设置相应的信号捕捉函数,做退出程序前的一些清理工作.
void     sig_action(sig_handler handler);
char*    getSigName(int sig);

//added by Long Xiangming. 2008.3.10
//支持core_dump能力(在uniframe捕捉信号的同时)
int		_core_dump(int signo);

//sig_action()函数将设置捕捉TSignalTable中catchEnabled为1的信号
struct  TSignalTable
{
	int  catchEnabled; //通过此值设置系统将捕捉的信号：1-捕捉，0-不捕捉
	int  sigaction;    //捕捉后的动作：1: 捕捉且执行指定函数（退出程序) 0: 捕捉但忽略(SIG_IGN) 2：捕捉且按系统缺省处理(SIG_DFL)
	int  sigid;        //11
	int  signo;        //SIGSEGV
	char signame[64];  //SIGSEGV: Segmentation fault
	int  canCoreDump;  //此值表示此信号是否会引起core dump,是此信号的一个属性，不可改变
};  

///////////////////////////////////////
/////    系统环境/硬件/网络相关函数
/////////////////////////////////////// 
//得到本机所有配置的IP地址
//返回值: -1: 失败
//        >=0: 返回的接口(IP地址）个数
//具体的接口地址保存在ifcfg参数中。TIfCfg的定义见comtypedef.h
INT getLocalHostAddress(TIfCfg& ifcfg);

// return a string like the display of unix command `uname -a`
char* uname_a();

//读取socket。屏蔽windows和Linux差别。
INT    read_socket_raw(int sockfd,char* buff,int length);
BOOL   close_socket(int sockfd);


#define UNIASSERT(e) {if(!(e)) UniERROR("ASSERT FAILED. file:%s, line:%d",__FILE__,__LINE__);}
#define UNIASSERT1(e,s) {if(!(e)) UniERROR("ASSERT FAILED. file:%s, line:%d, [%s]",__FILE__,__LINE__,s);}
#define UNIASSERT2(e,i) {if(!(e)) UniERROR("ASSERT FAILED. file:%s, line:%d, [%d]",__FILE__,__LINE__,i);}

///////////////////////////////////////////////
///////       File Functions
///////////////////////////////////////////////

// if the file exist, return >0, else return 0
I32 existFile( char* fileName );
char* getFileLine(char *buf, int buf_size, FILE *fp, int* lineNo);
off_t getFileLength(const char* fileName);

#endif

