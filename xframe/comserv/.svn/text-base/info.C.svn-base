#include "info.h"
#include "func.h"

void InitLogInfo(LogType type, LogLevel level, int logFileLength, const char* logFileName)
{
	char* defaultFileName="UNI.INFO.";
	#ifndef _SINGLE_THREAD
	Lock lock(g_loginfoMutex);
	#endif
	g_logLevel=level;
	g_logType=type;
	g_logFileName[0]=0;
	if(logFileName==0 && type==File)
	{
		strncpy(g_logFileName, defaultFileName, 256);
	}
	else
	{
		strncpy(g_logFileName, logFileName, 256);
	}

	if(logFileLength>=1000)
		g_logFileMaxLength=1000;
	else if(logFileLength>=10)
		g_logFileMaxLength=logFileLength;
	else if(logFileLength>0)
		g_logFileMaxLength=10;
	else
		g_logFileMaxLength=500;
}

void _UniDEBUG(const char* pfx, const char* fmt, ...)
{
   char tempLogStr[INFO_BUF_SIZE];
   memset(tempLogStr,0,INFO_BUF_SIZE);
   strncpy(tempLogStr, pfx, INFO_BUF_SIZE);
   tempLogStr[INFO_BUF_SIZE-1]=0;
   va_list args;
   va_start(args, fmt);
   vsprintf(tempLogStr+strlen(tempLogStr), fmt, args);
   va_end(args);
#ifndef _SINGLE_THREAD
   LogDEBUG(<< tempLogStr );
#else
   LogDEBUG_S(<< tempLogStr );
#endif
}

void _UniINFO(const char* pfx, const char* fmt, ...)
{
   char tempLogStr[INFO_BUF_SIZE];
   memset(tempLogStr,0,INFO_BUF_SIZE);
   strncpy(tempLogStr, pfx, INFO_BUF_SIZE);
   tempLogStr[INFO_BUF_SIZE-1]=0;
   va_list args;
   va_start(args, fmt);
   vsprintf(tempLogStr+strlen(tempLogStr), fmt, args);
   va_end(args);
#ifndef _SINGLE_THREAD
   LogINFO(<< tempLogStr );
#else
   LogINFO_S(<< tempLogStr );
#endif
}

void _UniERROR(const char* pfx, const char* fmt, ... )
{
   char tempLogStr[INFO_BUF_SIZE];
   memset(tempLogStr,0,INFO_BUF_SIZE);
   strncpy(tempLogStr, pfx, INFO_BUF_SIZE);
   tempLogStr[INFO_BUF_SIZE-1]=0;
   va_list args;
   va_start(args, fmt);
   vsprintf(tempLogStr+strlen(tempLogStr), fmt, args);
   va_end(args);
#ifndef _SINGLE_THREAD
   LogERROR(<< tempLogStr);
#else
   LogERROR_S(<< tempLogStr );
#endif
}



//输出到日志.
void toLogFile(const char* s, char* logInfoFileName)
{
#ifndef WIN32
//判断文件大小
   struct stat buf;
   int rt = lstat(logInfoFileName,&buf);
   if(rt==0)
   {
      int maxSize = g_logFileMaxLength;
      if(buf.st_size>(maxSize*1024*1024))
      {
         //UniERROR("\nWARNING: log file size out of limits(%dM): %s. We will backup it.\n",maxSize,logInfoFileName);
         //看看.bak是否存在
         CHAR bakFile[512];
         bakFile[0]=0;
         sprintf(bakFile,"%s.bak",logInfoFileName);
         if(existFile(bakFile))
         {
            //删除此.bak，将原UNIINFO改名为UNIINFO.bak（即备份原文件)
            remove(bakFile);
            rename(logInfoFileName,bakFile);
         }
         else
            rename(logInfoFileName,bakFile);
      }
   }
#endif

   FILE* fp;
   fp = fopen( logInfoFileName , "a+" );
   if ( fp == NULL )
   {
      //UniERROR( "open file %s error\n" , logInfoFileName );
      perror("XFRAME: Open log file error!!!");
      return;
   }

   time_t timer;
   struct tm *tblock;

   timer = get_currtime_sec();
   tblock = localtime( &timer );
//   if( oldYear != tblock->tm_year || oldYday != tblock->tm_yday )
//   {
//      fprintf( fp, "[%s]:", ctime( &timer ) );
//      oldYear = tblock->tm_year;
//      oldYday = tblock->tm_yday;
//   }

	TTimeString ts;
	tmToTimeString(tblock, ts);
	fprintf( fp,"[%s]", ts);
//   fprintf( fp, "%02d:%02d:%02d: %s\n"
//      ,(I32)tblock->tm_hour
//      ,(I32)tblock->tm_min
//      ,(I32)tblock->tm_sec
//      ,s );

	fprintf( fp, " %s\n", s);

   fclose( fp );
   return;
}


void toPrint(const char *s, const char* appName)
{
	char tmpLogFileName[256];
	tmpLogFileName[0]=0;
	strncat(tmpLogFileName, g_logFileName, 128);
	char p[10];
	p[0]=0;
	getASCDate8(p);
	strncat(tmpLogFileName, p, 8);
	if(appName!=NULL)
	{
		strncat(tmpLogFileName, ".", 4);
		strncat(tmpLogFileName, appName, 32);
	}
	strncat(tmpLogFileName, ".log", 4);
	toLogFile(s, tmpLogFileName);
}

void toCout(const char *s)
{
    toAme(s);
    std::cout<<s<<std::endl;
}

void toAme(const char *s)
{

}


void setLevel(LogLevel level)
{
	#ifndef  _SINGLE_THREAD
	Lock lock(g_loginfoMutex);
	#endif
	g_logLevel=level;
}

void setType(LogType type)
{
	#ifndef  _SINGLE_THREAD
	Lock lock(g_loginfoMutex);
	#endif
	g_logType=type;
}

void setFileName(char* logFileName)
{
	if(logFileName==NULL) return;
	#ifndef  _SINGLE_THREAD
	Lock lock(g_loginfoMutex);
	#endif
	g_logFileName[0]=0;
	strncpy(g_logFileName, logFileName, 256);
}

void setFileLength(int logFileLength)
{
	#ifndef  _SINGLE_THREAD
        Lock lock(g_loginfoMutex);
	#endif
        if(logFileLength>=10000)
            g_logFileMaxLength=10000;
        else if(logFileLength>=10)
            g_logFileMaxLength=logFileLength;
        else if(logFileLength>0)
            g_logFileMaxLength=10;
        else
            g_logFileMaxLength=500;         //小于等于0，取默认值500M
}


