#include "func.h"
#include "info.h"

//字符串不分大小写比较
// src > dst：源字符串大于目的字符串 return >0
// src = dst：字符串相等 return 0
// src < dst：源字符串小于目的字符串 return <0
int strnotcasecmp( char* src , char* dst )
{
    if(strlen(src)!=strlen(dst)) return -1;
    return strncasecmp(src,dst,strlen(dst));
}

void strCopy(char* dest,const char* src,int len)
{
   strncpy(dest, src, len);
   dest[len-1]=0;
}

CHAR _getHexChar(CHAR i)
{
	if(i>16) i=i%16;
	if(i>=0 && i<10) return '0'+i;
	if(i>=10) return 'A'+(i-10);
}
// 整型(32位）转成16进制字符串（8个字符）
void intToHex(char*s, UINT n)
{
	if(s==NULL) return;
	CHAR temp[4];
	UINT iTemp = htonl(n); //字节顺序变换一下。
	memcpy(temp,&iTemp,4);
	char *p = s;
	for(int i=0;i<4;i++)
	{
		p[0]=_getHexChar((temp[i]&0xf0)>>4);
		p[1]=_getHexChar(temp[i]&0x0f);
		p+=2;
	}
	s[8] = 0;

}

//得到一个系统全局唯一的字符串ID(24个字节,16进制形式）（根据当前时间计算）
//可以在一些需要全局唯一标志的地方使用
void get_unique_hexid(char* uniqueHexId)
{
	uniqueHexId[0]=0;

	//得到前8个字节（当前时间，单位秒，32位整数，转成16进制，需32/4=8个字符）
	UINT currTimeSec = get_currtime_sec();
	char* p= uniqueHexId;
	intToHex(p,currTimeSec);

	//得到中间8个字节（当前时间，单位秒，32位整数，转成16进制，需32/4=8个字符）
	p = uniqueHexId+8;
	intToHex(p,0);

	//得到最后8个字节
	UINT static autoIncrement=0;
	autoIncrement++;
	p = uniqueHexId+16;
	intToHex(p,autoIncrement);

	uniqueHexId[24]=0;
}

//将标准时间间隔转换为自定义的结构化时间
//typedef struct 
//{
//    I32 hour;
//    I32 minute;
//    I32 second;
//    I32 usec; //微秒
//}TTimeFormatted;  
void timeIntervalFormatted(time_t timeInterval, TTimeFormatted & timeConverted)
{
    timeConverted.hour      =  timeInterval/3600;
    timeInterval            -= timeConverted.hour*3600;
    timeConverted.minute    =  timeInterval/60;
    timeConverted.second    =  timeInterval-timeConverted.minute*60;
    timeConverted.usec      =  0; 
    return;
}

#ifndef WIN32
void timeIntervalFormatted(struct timeval & startTime,struct timeval & endTime,TTimeFormatted & timeConverted)
{
    time_t timeInterval;
    timeInterval = endTime.tv_sec -  startTime.tv_sec;
    timeConverted.hour      =  timeInterval/3600;
    timeInterval            -= timeConverted.hour*3600;
    timeConverted.minute    =  timeInterval/60;
    timeConverted.second    =  timeInterval-timeConverted.minute*60;
    if(endTime.tv_usec>startTime.tv_usec)
    {
       timeConverted.usec = endTime.tv_usec - startTime.tv_usec ;
    }
    else
    {
        timeConverted.second --;
        timeConverted.usec = 1000000 - (startTime.tv_usec - endTime.tv_usec);
    }
}
#endif


#ifdef WIN32
void timeIntervalFormatted(DWORD & startTime, DWORD & endTime,TTimeFormatted & timeConverted)
{
    I32 timeInterval =(I32)(endTime-startTime); //毫秒为单位
    timeConverted.hour      =  timeInterval/(3600*1000);
    timeInterval           -=  timeConverted.hour*3600*1000; //剩下的毫秒数
    timeConverted.minute    =  timeInterval/(60*1000);
    timeInterval           -=  timeConverted.minute*60*1000; //剩下的毫秒数
    timeConverted.second    =  timeInterval/1000;
    timeInterval           -=  timeConverted.second*1000;
    timeConverted.usec      =  timeInterval*1000;
}
#endif






UINT64  getSystemTime()
{
   assert(sizeof(UINT64) == 64/8);

#if defined(WIN32) || defined(UNDER_CE)
   FILETIME ft;

#ifdef UNDER_CE
   SYSTEMTIME st;
   ::GetSystemTime(&st);
   ::SystemTimeToFileTime(&st,&ft);
#else
   ::GetSystemTimeAsFileTime(&ft);
#endif

   ULARGE_INTEGER li;
   li.LowPart = ft.dwLowDateTime;
   li.HighPart = ft.dwHighDateTime;
   return li.QuadPart/10;

#else //#if defined(WIN32) || defined(UNDER_CE)

   UINT64 time=0;

   struct timeval now;
   gettimeofday( &now , NULL );
   //assert( now );
   time = now.tv_sec;
   time = time*1000000;
   time += now.tv_usec;
   return time;
#endif
}

UINT64 getTimeMicroSec()
{
   return getSystemTime();
}

UINT64 getTimeMs()
{
   return getSystemTime()/1000LL;
}

UINT64 getTimeSecs()
{
   return getSystemTime()/1000000LL;
}

void tmToASCTime(tm* tmTime,TASCTime ascTime)
{
   sprintf(ascTime, "%04d%02d%02d%02d%02d%02d"
      , tmTime->tm_year+1900
      , tmTime->tm_mon+1
      , tmTime->tm_mday
      , tmTime->tm_hour
      , tmTime->tm_min
      , tmTime->tm_sec
   );
   ascTime[MaxTimeLength]=0;
}

void tmToTimeString(tm* tmTime,TTimeString timeString)
{
   sprintf(timeString,"%04d-%02d-%02d_%02d:%02d:%02d"
   ,tmTime->tm_year+1900
   ,tmTime->tm_mon+1
   ,tmTime->tm_mday
   ,tmTime->tm_hour
   ,tmTime->tm_min
   ,tmTime->tm_sec);
   timeString[MaxTimeStringLength]=0;
}

void getASCDate8(char* currentASCDate8)
{
   currentASCDate8[0]=0;
   time_t currTime = get_currtime_sec();
   struct tm*  uni_tm;
   uni_tm = localtime(&currTime);

   if ( uni_tm != NULL )
   {
      sprintf(currentASCDate8,"%04d%02d%02d"
         ,uni_tm->tm_year+1900
         ,uni_tm->tm_mon+1
         ,uni_tm->tm_mday);
   }
   return;
}

void getASCDate8AndTime(char* currentASCDate8AndTime)
{
   currentASCDate8AndTime[0]=0;
   time_t currTime;
   //time(&currTime);
   currTime = get_currtime_sec();
   struct tm*  uni_tm;
   uni_tm = localtime(&currTime);
   tmToASCTime(uni_tm,currentASCDate8AndTime);
   currentASCDate8AndTime[14]=0;
   return;
}

void getASCDate6AndTime4(char* currentASCDate6AndTime)
{
   currentASCDate6AndTime[0]=0;
   char tmp[15];    tmp[0]=0;
   char* p=tmp;
   getASCDate8AndTime(p);
   p+=2;    
   strncpy(currentASCDate6AndTime,p,10);
   currentASCDate6AndTime[10] = 0;
   return;
}

void getASCDate6AndTime6(char* currentASCDate6AndTime)
{
   char tmp[15];  tmp[0]=0;
   char* p=tmp;
   getASCDate8AndTime(p);
   p+=2;    
   strncpy(currentASCDate6AndTime,p,12);
   currentASCDate6AndTime[12] = 0;
   return;

}

UINT get_currtime_sec()//以秒计
{
//modified by lxm.2008.07.28
//使用Purify测试表明，time()函数占用了绝大部分处理时间(90%以上），
//因此，决定由frameControl提供当前时间值，不直接调用系统函数time().
//(只在mainLoop中调用一次time()，其他地方都使用get_currtime_sec()从frameControl中获取)
#ifdef XFRAME_TIME
   if(g_frameControl)
      return g_frameControl->getCurrTimeSec();
   else
   {
      time_t currTime;
      time(&currTime);
      return currTime;

   }
#else
   time_t currTime;
   time(&currTime);
   return currTime;
#endif
}

/////////////////////////////////////
////    进程/线程/调度相关函数
/////////////////////////////////////

timeval g_waitTime;

//added by Long Xiangming. 2006.06.30
void     uniSleep(int millisec)
{
   //睡眠一段时间（毫秒）。屏蔽Windows和Linux/Unix差别
    if(millisec<=0) return;
   uni_usleep(millisec*1000);
}

void     uni_usleep(int usec)
{
   //睡眠一段时间（微秒）。屏蔽Windows和Linux/Unix差别
    if(usec<=0) return;

/*
#ifndef WIN32
   usleep(usec);   
#else
   UINT msec = usec/1000;
   if(msec<=0)
      msec = 1;
   Sleep(msec); 
#endif
*/
   //do not use usleep because it is high cpu overload.
   //use select because it is high performance.
#ifndef WIN32
   g_waitTime.tv_sec = 0;
   g_waitTime.tv_usec = usec;
   if(select(FD_SETSIZE,(fd_set*)0,(fd_set*)0,(fd_set*)0,&g_waitTime)>0)
   {
   UniERROR("uni_usleep() ERROR: select() > 0. no sleep here.");
   }   
#else
   UINT msec = usec/1000;
   if(msec<=0)
      msec = 1;
   Sleep(msec); 
#endif
}

TSignalTable g_signalTable[]={
#ifndef WIN32
   {1,0,1,   SIGHUP,      "SIGHUP: Terminal hangup",0}, /* 捕捉但忽略 */
   {1,0,3,   SIGQUIT,   "SIGQUIT: Terminal quit",0},        /* 捕捉但忽略 */
   {1,1,4,   SIGILL,      "SIGILL: Illegal instruction",1},
   {1,1,5,   SIGTRAP,   "SIGTRAP: Trace trap",1},
   /*{1,1,6,   SIGIOT,      "SIGIOT: IOT trap",1}, ==Abort */
   {1,1,7,   SIGBUS,      "SIGBUS: BUS error",1},
   {1,1,9,   SIGKILL,   "SIGKILL: Kill, unblockable",0}, /* 实际上进程无法捕捉到此信号 */
   {1,1,10,SIGUSR1,   "SIGUSR1: User-defined signal 1",0},
   {1,1,12,SIGUSR2,   "SIGUSR2: User-defined signal 2",0},
   {0,0,13,SIGPIPE,   "SIGPIPE: Broken pipe",0}, /* 在socket.C 中单独设置 */
   {1,1,14,SIGALRM,   "SIGALRM: Alarm clock",0}, /* 可能有应用需要用此方法构造定时器 */
   {1,1,16,SIGSTKFLT,   "SIGSTKFLT: Stack fault",0},
   {0,0,17,SIGCHLD,   "SIGCHLD: Child status has changed",0},/* 在framectrl.C中单独设置 */
   {0,0,18,SIGCONT,   "SIGCONT: Continue",0}, /* gdb debug usage ?? */
   {1,1,19,SIGSTOP,   "SIGSTOP: Stop, unblockable",0}, /* 实际上进程无法捕捉到此信号 */
   {1,0,20,SIGTSTP,   "SIGTSTP: Keyboard stop",0},/* 捕捉但忽略 */
   {1,0,21,SIGTTIN,   "SIGTTIN: Background read from tty",0},/* 捕捉但忽略 */
   {1,0,22,SIGTTOU,   "SIGTTOU: Background write to tty",0},/* 捕捉但忽略 */
   {1,1,23,SIGURG,      "SIGURG: Urgent condition on socket",0}, /* 实际上进程无法捕捉到此信号 */
   {1,1,24,SIGXCPU,   "SIGXCPU: CPU limit exceeded",1},
   {1,1,25,SIGXFSZ,   "SIGXFSZ: File size limit exceeded",1},
   {1,1,26,SIGVTALRM,   "SIGVTALRM: Virtual alarm clock",0},
   {1,1,27,SIGPROF,   "SIGPROF: Profiling alarm clock",0},
   {1,0,28,SIGWINCH,   "SIGWINCH: Window size change",0},/* 捕捉但忽略 */
   {1,1,29,SIGIO,      "SIGIO(SIGPOLL):I/O now possible(Pollable event occurred)",0},/* 捕捉但忽略 */
   {1,0,30,SIGPWR,      "SIGPWR: Power failure restart",0},/* 捕捉但忽略 */
   {1,1,31,SIGSYS,      "SIGSYS:  Bad system call",1},
   {1,0,34,34,         "SIGRT:  Real time signal",0},

#endif
   /* Linux/Unix/Windows 均支持的信号 */
   {1,1,2,   SIGINT,      "SIGINT: Ctrl+C",0},
   {1,1,6,   SIGABRT,   "SIGABRT: Abort",1},
   {1,1,8,   SIGFPE,      "SIGPIPE: Floating-point exception",1},
   {1,1,11,SIGSEGV,   "SIGSEGV: Segmentation fault",1},
   {1,1,15,SIGTERM,   "SIGTERM: kill",0},
   {0,0,-1,-1,         "none signal",0}
   
};

/*
// Signals.  
// <bits/signum.h>
#define SIGHUP          1       // Hangup (POSIX).  
#define SIGINT          2       // Interrupt (ANSI).  
#define SIGQUIT         3       // Quit (POSIX).  
!!#define SIGILL          4       // Illegal instruction (ANSI).  
!!#define SIGTRAP         5       // Trace trap (POSIX).  
#define SIGABRT         6       // Abort (ANSI).  
!!#define SIGIOT          6       // IOT trap (4.2 BSD).  
#define SIGBUS          7       // BUS error (4.2 BSD).  
#define SIGFPE          8       // Floating-point exception (ANSI).  
#define SIGKILL         9       // Kill, unblockable (POSIX).  
#define SIGUSR1         10      // User-defined signal 1 (POSIX).  
#define SIGSEGV         11      // Segmentation violation (ANSI).  
#define SIGUSR2         12      // User-defined signal 2 (POSIX).  
#define SIGPIPE         13      // Broken pipe (POSIX).  
!!#define SIGALRM         14      // Alarm clock (POSIX).  
#define SIGTERM         15      // Termination (ANSI).  
!!#define SIGSTKFLT       16      // Stack fault.  
#define SIGCLD          SIGCHLD // Same as SIGCHLD (System V).  
#define SIGCHLD         17      // Child status has changed (POSIX).  
!!#define SIGCONT         18      // Continue (POSIX).  
#define SIGSTOP         19      // Stop, unblockable (POSIX).  
!!#define SIGTSTP         20      // Keyboard stop (POSIX).  
!!#define SIGTTIN         21      // Background read from tty (POSIX).  
!!#define SIGTTOU         22      // Background write to tty (POSIX).  
!!#define SIGURG          23      // Urgent condition on socket (4.2 BSD).  
!!#define SIGXCPU         24      // CPU limit exceeded (4.2 BSD).  
!!#define SIGXFSZ         25      // File size limit exceeded (4.2 BSD).  
!!#define SIGVTALRM       26      // Virtual alarm clock (4.2 BSD).  
!!#define SIGPROF         27      // Profiling alarm clock (4.2 BSD).  
!!#define SIGWINCH        28      // Window size change (4.3 BSD, Sun).  
!!#define SIGPOLL         SIGIO   // Pollable event occurred (System V).  
!!#define SIGIO           29      // I/O now possible (4.2 BSD).  
!!#define SIGPWR          30      // Power failure restart (System V).  
!!#define SIGSYS          31      // Bad system call.  
#define SIGUNUSED       31
*/

void   sig_action(sig_handler handler)
{
   int nTotal=0;
   int nInstalled=0;
   int nCaughtAndQuit=0;
   int nIgnored      =0;
   int nActDefault   =0;   
   int nCanCoreDump  =0;
   char strCaughtAndQuit[256];
   char strIgnored[256];
   char strActDefault[256];
   char strCanCoreDump[256];
   char temp[32];
   strCaughtAndQuit[0]=0;
   strIgnored[0]=0;
   strActDefault[0]=0;
   strCanCoreDump[0]=0;

   for(int i=0;i<ARRAY_SIZE(g_signalTable);i++)
   {
      if(g_signalTable[i].signo>0)
      {//if-1
         nTotal++;
         if(g_signalTable[i].catchEnabled==1)
         {//if-2
            nInstalled++;

            switch(g_signalTable[i].sigaction)
            {   
               case 0: //忽略
                  {
                     signal(g_signalTable[i].signo,SIG_IGN);
                     nIgnored++;
                     sprintf(temp,"%d",g_signalTable[i].signo);
                     if(strIgnored[0]==0)
                        strcpy(strIgnored,temp);
                     else
                     {
                        strcat(strIgnored,",");
                        strcat(strIgnored,temp);
                     }

                  }
                  break;
               case 1: //捕捉且退出
                  {
                     signal(g_signalTable[i].signo,handler);
                     nCaughtAndQuit++;
                  
                     sprintf(temp,"%d",g_signalTable[i].signo);
                     if(strCaughtAndQuit[0]==0)
                        strcpy(strCaughtAndQuit,temp);
                     else
                     {
                        strcat(strCaughtAndQuit,",");
                        strcat(strCaughtAndQuit,temp);
                     }

                     if(g_signalTable[i].canCoreDump)
                     {
                        nCanCoreDump++;
                        if(strCanCoreDump[0]==0)
                        strcpy(strCanCoreDump,temp);
                        else
                        {
                           strcat(strCanCoreDump,",");
                           strcat(strCanCoreDump,temp);
                        }
                     }

                  }
                  break;
               case 2: //捕捉且按系统缺省
                  {
                     signal(g_signalTable[i].signo,SIG_DFL);
                     nActDefault++;
                     sprintf(temp,"%d",g_signalTable[i].signo);
                     if(strActDefault[0]==0)
                        strcpy(strActDefault,temp);
                     else
                     {
                        strcat(strActDefault,",");
                        strcat(strActDefault,temp);
                     }

                     if(g_signalTable[i].canCoreDump)
                     {
                        nCanCoreDump++;
                        if(strCanCoreDump[0]==0)
                        strcpy(strCanCoreDump,temp);
                        else
                        {
                           strcat(strCanCoreDump,",");
                           strcat(strCanCoreDump,temp);
                        }
                     }

                  }
                  break;
               default:
                  UniERROR("ERROR in sig_action(): unsupported action: %d, signo:%d",
                     g_signalTable[i].sigaction,g_signalTable[i].signo);
                  break;
            }//end of switch
         }//end of if-2

      }//end of if-1

   }//end of for
   
   UniINFO("Set signal handler. total defined: %d.  total installed: %d. ",nTotal,nInstalled);
   // sprintf(uniTempStr,"The installed signals:  I-caught_quit: %d(%s),  II-ignored: %d(%s), III-act_default: %d(%s). core_dump supported: %d(%s).",
   //   nCaughtAndQuit,strCaughtAndQuit,nIgnored,strIgnored,nActDefault,strActDefault,nCanCoreDump,strCanCoreDump);
   //UniINFO(uniTempStr);
      

/*
#ifndef WIN32
   //当终端连接断开时,我们不退出程序,所以这两个信号设置为忽略
   //signal(SIGHUP,SIG_IGN);  //(signo: 1)
   //signal(SIGQUIT,SIG_IGN);

   //以下异常退出信号，需要回收资源，主要是关闭socket, 文件句柄等
   signal(SIGBUS,handler);
   signal(SIGKILL,handler);
   signal(SIGSTOP,handler); //(signo: 17,19,23)
   //其他信号
   //Added by Long Xiangming. 2008.1.31
   signal(SIGILL,handler);
   //signal(SIGPIPE,handler); 管道错误暂不退出程序，由uniframe在心跳失败后重连(重连比退出程序要好)
   
#endif
   //Linux/Unix/Windows 均支持的信号
   signal(SIGINT,handler); //Ctrl+C
   signal(SIGABRT,handler);
   signal(SIGSEGV,handler); //断错误
   signal(SIGTERM,handler);//kill命令或者kill()函数 (signo:15)
    signal(SIGFPE,handler);// 浮点错误

*/
}

char* getSigName(int sig)
{
   for(int i=0;i<ARRAY_SIZE(g_signalTable);i++)
   {
      if(g_signalTable[i].signo==sig)
         return g_signalTable[i].signame;
   }
   return "unknown";

/*
   switch(sig)
   {
#ifndef WIN32
      case SIGBUS:  return "SIGBUS: BUS error";
      case SIGKILL: return "SIGKILL";
      case SIGSTOP: return "SIGSTOP";
      case SIGHUP:  return "SIGHUP: Terminal quit";
      case SIGQUIT: return "SIGQUIT: Terminal quit";
#endif
      case SIGINT:  return "SIGINT: Ctrl+C";
      case SIGABRT: return "SIGABRT: Abort";
      case SIGSEGV: return "SIGSEGV: Segmentation fault";
      case SIGTERM: return "SIGTERM: kill";
      case SIGFPE:  return "SIGFPE: Float error";
      default:      return "unknown";
   }
*/
}

BOOL isCanCoreDump(int signo)
{
   for(int i=0;i<ARRAY_SIZE(g_signalTable);i++)
   {
      if(g_signalTable[i].signo==signo)
      {
         if(g_signalTable[i].canCoreDump)
            return TRUE;   
      }
   }

   return FALSE;

}

//added by Long Xiangming. 2008.3.10
//支持core_dump能力
//0：执行core dump, 进入子进程
//1: 不执行core dump,返回父进程
int _core_dump(int signo)
{
   int pid=1;

#ifndef  WIN32
   if(!isCanCoreDump(signo))
   {
      return 1;
   }
   UniINFO("The core dump is supported for this signal %d(%s).",signo,getSigName(signo));
   pid = fork();
   if (pid == 0) 
   {
#ifndef _CORE_SIZE
#define _CORE_SIZE (256 * 1024 * 1024)
#endif /* _CORE_SIZE */
      struct rlimit limit;
      limit.rlim_cur = _CORE_SIZE;
      limit.rlim_max = _CORE_SIZE;
      setrlimit(RLIMIT_CORE, &limit);
      /* reset the signal handler to default handler,
       * then raise the corresponding signal. */
      signal(signo, SIG_DFL);
      raise(signo);
   }
#endif
   return pid;
}

///////////////////////////////////////
/////    系统环境/硬件/网络相关函数
///////////////////////////////////////

BOOL   close_socket(int sockfd)
{
   if(sockfd<=0) return FALSE;
#ifdef WIN32
   //if(shutdown(sockfd,2)==0) //成功关闭socket连接句柄
   if(closesocket(sockfd)==0)
#else
   if(close(sockfd)==0)
#endif
   {
   #ifdef WIN32
      WSACleanup();
   #endif

      return TRUE;
   }
   else return FALSE;
}

INT   read_socket_raw(int sockfd,char* buff,int length)
{
   if(buff==NULL) return -4;
   if(length<=0 || sockfd<=0)  return 0;
#ifdef WIN32
      return   recv(sockfd,buff,length,MSG_PARTIAL); 
      // 最后一个参数在windows下必须使用MSG_PARTIAL,否则如果使用MSG_PEEK,
       // 则不会清空缓冲区，总是要连同接收以前的数据。
#else
      return   read(sockfd,buff,length); 
#endif
}

//Added by Xfzhou. 2007.12.10
//得到本机IP地址信息
INT getLocalHostAddress(TIfCfg& ifcfg)
{
#ifndef WIN32
   register int fd, intrface;
   struct ifreq buf[MAX_NET_INTERFACES];
   struct ifconf ifc;

   if ((fd = socket (AF_INET, SOCK_DGRAM, 0)) < 0)
   {
      return -1;
   }

   ifc.ifc_len = sizeof(buf);
   ifc.ifc_buf = (caddr_t) buf;
   if (ioctl (fd, SIOCGIFCONF, (char *) &ifc) < 0)
   {
      return -1;
   }

   intrface = ifc.ifc_len/sizeof(struct ifreq);
   while (intrface-- > 0)
   {

      if ((ioctl (fd, SIOCGIFFLAGS, (char *) &buf[intrface])) < 0)
      {
         continue;
      }
      snprintf(ifcfg.interfaces[ifcfg.size].name,sizeof(ifcfg.interfaces[ifcfg.size].name),"%s",buf[intrface].ifr_name);
      if (buf[intrface].ifr_flags & IFF_PROMISC)
      {
         ifcfg.interfaces[ifcfg.size].status=TIfCfg::PROMISC;
      }
      else
      {
         if (buf[intrface].ifr_flags & IFF_UP)
         {
            ifcfg.interfaces[ifcfg.size].status=TIfCfg::UP;
         }
         else
         {
            if (buf[intrface].ifr_flags & IFF_RUNNING)
            {
               ifcfg.interfaces[ifcfg.size].status=TIfCfg::RUNNING;
            }
         }
      }
      if (!(ioctl (fd, SIOCGIFADDR, (char *) &buf[intrface])))
      {
         snprintf(ifcfg.interfaces[ifcfg.size].address,sizeof(ifcfg.interfaces[ifcfg.size].address),"%s",inet_ntoa(((struct sockaddr_in*)(&buf[intrface].ifr_addr))->sin_addr));
      }
      else
         continue;
      
      ifcfg.size++;

   }
   close (fd);
   return ifcfg.size;
#else
   return -1;
#endif
}


I32 existFile( char* fileName )
{
   FILE* fp;

   if ((fp = fopen(fileName,"r")) == NULL)
      return 0;
   else
   {   
      fclose(fp);
      return 1;
   }
}

char* getFileLine(char *buf, int buf_size, FILE *fp, int* lineNo)
{
   while(fgets(buf, buf_size, fp)!=NULL)
   {
      if(lineNo!=NULL) (*lineNo)++;
      char* s=buf;
      while(isspace(*s))
         s++;
      if(*s==0 || *s=='#')
         continue;
      return buf;
   }
   return NULL;
}

off_t getFileLength(const char* fileName)
{
   struct stat fStat;
   if(stat(fileName,&fStat)==0)
      return fStat.st_size;
   return 0;
} 

 
