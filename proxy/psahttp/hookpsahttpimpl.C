/* hookpsahttpimpl.C, by Zhang Haibin, 2011-1-25
 */
#include "uniframe.h"
#include "dyncomponent.h"
#include "httpserver.h"
#include "infoext.h"
#include "abstractamehandler.h"
#include "unihtml.h"
#include "msgdef_uninet.h"
#include "msgdef_com.h"
#include "httpclient.h"

/********************************************************************************************************
========协议栈声明============
INIT_PSA_COMP 宏用于声明协议栈名字，定义以该协议栈名字命名的相关协议栈初始化函数。
	协议栈初始化函数由 INIT_PSA_COMP 定义，形式为 initXXXXXX，其中XXXXXX与INIT_PSA_COMP参数一致。
	协议栈初始化函数一定要在 INIT_PSA_COMP 宏之前声明，以保证INIT_PSA_COMP宏能够找到该函数
	例如，声明以PsaName为名字的协议栈：
	
	void initPsaName(INT psaId);
	
	INIT_PSA_COMP(PsaName)
	
========协议栈初始化===========
协议栈初始化函数（initXXXX）将把协议栈中定义的一组函数挂接到uniframe中，以支持消息收发，这一组功能主要包括：
	setHookBuildFdSet(psaId, fdset设置函数);
	setHookProcFdSet(psaId, fdset处理函数);
	setHookSendUniNetMsg(psaId, 消息发送处理函数);
	setHookCloseResource(psaId, 协议栈关闭前的资源清理函数);
	
	四种函数的定义示例：
	void hookBuildFdSetPsaNameImpl(CFdSet& fdSet){}						//fdset设置函数
	BOOL hookProcFdSetPsaNameImpl(CFdSet& fdSet) { return TRUE/FALSE;}	//fdset处理函数
	BOOL hookSendMsgPsaNameImpl(TUniNetMsg* msg) { return TRUE/FALSE;}	//消息发送处理函数
	void hookCloseResourcePsaNameImpl(){}								//协议栈关闭前的资源清理函数
	
=========发送消息================
当应用把消息发送到协议栈，则uniframe会调用协议栈初始化函数中用setHookSendUniNetMsg函数注册的发送消息函数。
	例如：
	
	BOOL hookSendMsgPsaNameImpl(TUniNetMsg* msg) 
	{
		//该函数将传入UniNetMsg消息指针，协议栈将该消息发送出去即可
		return TRUE/FALSE;
	}

=========接收消息================
当需要检测网络上报的消息，则需要做三个步骤	
1、实现FDSet设置函数
	系统启动的时候如果设置了fdset设置函数，则uniframe会调用该函数，传入uniframe统一管理的fdset。
	
	例如：
	void hookBuildFdSetPsaNameImpl(CFdSet& fdSet)
	{
		//这个FDSet由Uniframe统一管理，设置FDSet函数中需要把网络socket对象设置到该fdset中，保证fdset能够对协议栈进行统一的select阻塞。
	}		
	
2、实现FDSet处理函数
	当实现了fdset设置，则当检测到网络收到消息，则fdset会解除select阻塞，此时fdset处理函数会被调用
	例如：
	BOOL hookProcFdSetPsaNameImpl(CFdSet& fdSet) 
	{ 
		//协议栈需要根据fdset判断是否自己收到消息，并进行收消息处理
		return TRUE/FALSE;
	}

=========关闭协议栈================	
当协议栈被关闭的时候，协议栈关闭函数会被调用，系统在此释放协议栈分配的资源。
	例如：
	void hookCloseResourcePsaNameImpl()
	{	}	
************************************************************************************************************/


//========初始化函数定义===========
// INT psaId：当前协议栈被分配的标识，这一标识在uniframe中用于唯一标识协议栈，标识协议栈进程
void initPsaHttp(INT psaId);

//初始化协议栈自定义函数的名字
INIT_PSA_COMP(PsaHttp)

CDict serviceIdMap;

CHttpServer* g_httpServer = NULL;
int g_printSocket = 0;
int g_printDownload = 0;
CTcpConnectionManager* g_httpClient = NULL;
int g_thisPsaId;
void httpResponseToApp(int appKey, int linkId, CStr& version, int rCode, CStr& content);

//===========fdset设置函数============
// CFdSet& fdSet：fdset对象
//	把协议栈new出的socket对象设置到fdset对象中，保证fdset对象收集到全部的socket，以统一进行监听
void hookBuildFdSetPsaHttpImpl(CFdSet& fdSet)
{
   if(g_httpServer!=NULL)
      g_httpServer->prepareFdSet(fdSet);
   if(g_httpClient!=NULL)
      g_httpClient->buildFdSet(fdSet);
}

BOOL hookProcFdSetPsaHttpImpl(CFdSet& fdSet)
{
   BOOL re = FALSE;
   if(g_httpServer!=NULL)
      re |= g_httpServer->check(fdSet);
   if(g_httpClient!=NULL)
      re |= g_httpClient->procFdSet(fdSet);
   return re;
}

int processHttpMessageAsync(int appKey, CHttpMessage* httpMsg)
{
   TUniNetMsg* msg = new TUniNetMsg;
   msg->oAddr.logAddr = g_thisPsaId;
   msg->oAddr.taskInstID = httpMsg->linkId;
   msg->tAddr.logAddr = appKey;
   msg->msgName = SIMPLE_PSA_REQUEST;

   TArgvMsgBody* pArgv = new TArgvMsgBody;
   httpMsg->buildAsyncAppArgv(pArgv->argc, pArgv->argv, MAX_MSG_ARGV_NUM-3);
   msg->msgBody = pArgv;
   msg->setMsgBody();

   sendMsgToFEAM(msg);

   return 1;
}

int checkAsyncAppKey(const char* path)
{
   int i = 0;
   if(isdigit(path[0]))
      i = atoi(path);
   else
   {
      CStr key;
      serviceIdMap.get(path, key);
      if(key.endWith(".usl"))
      {
         CStr errInfo;
         i = CUniFrame::loadScript(key.c_str(), errInfo);
         if(i<0)
         {
            UniERROR("load script %s failed: %s", key.c_str(), errInfo.c_str());
            return -1;
         }
      }
      else
         i = key.toInt();
   }
   if(i > 100)
      return i;
   return -1;
}

// return 0, sync process; return 1, async process
int processHttpMessage(CHttpMessage* msg)
{
   if(toPrintPsaMsg(g_thisPsaId))
      UniPRINT("link %d received: %s %s\n",msg->linkId, msg->method.c_str(), msg->path.c_str());
   msg->path.uricDecode();
   if(msg->path == "psahttpselftest")
   {
      CStr strRes;
      msg->genTestResponseMessage(strRes);
      g_httpServer->send(msg->linkId, strRes);
      return 0;
   }

   if(msg->path.empty())
      msg->path = "index";
   int asyncAppKey = checkAsyncAppKey(msg->path.c_str());
   if(asyncAppKey > 0)
      return processHttpMessageAsync(asyncAppKey, msg);

   const char* ext = msg->pathExt();
   if(ext == NULL)
   {
      msg->path += ".so";
      ext = "so";
   }
   if(strcmp(ext, "usl")==0)
   {
      CStr str;
      int appKey = CUniFrame::loadScript(msg->path.c_str(), str);
      if(appKey < 0)
      {
         CStr strRes;
         msg->genErrorMessage(strRes, 400, str.c_str());
         g_httpServer->send(msg->linkId, strRes);
         return 0;
      }
      return processHttpMessageAsync(appKey, msg);
   }

   if(strcmp(ext, "so")==0)
   {
      int argc;
      const char* argv[30];
      msg->buildDllCallArgv(argc, argv, sizeof(argv)/sizeof(const char*));
      CHtml str;
      int re = CUniFrame::dllCall(msg->path.c_str(), "processHttpRequest", argc, argv, str);
      CStr strRes;
      if(re!=0)
         msg->genErrorMessage(strRes, 400, str.c_str());
      else
         msg->genResponseMessage(strRes, "text/html", str.c_str());

      g_httpServer->send(msg->linkId, strRes);
      return 0;
   }
   CStr strRes;
   msg->genFileResponseMessage(strRes);
   g_httpServer->send(msg->linkId, strRes);
   return 0;
}

void buildHttpRequest(CHttpClientLink* link, const char* host, int port, const char* path, CStr& content, int otherArgNum, CStr* otherArgs, CStr& result)
{
#define S_CRLF "\r\n"
   result << (content.empty()?"GET":"POST") << " /" << path << " HTTP/1.1" << S_CRLF;
   result << "Host: " << host << ":" << port << S_CRLF;
   result << "Connection: keep-alive" << S_CRLF;
   result << "User-Agent: UniFrame" << S_CRLF;
   if(!content.empty())
      result << "Content-Length: " << content.length() << S_CRLF;
   for(int i=0; i<otherArgNum; i++)
   {
      if(otherArgs[i].beginWith("-file "))
      {
         link->setDownload(otherArgs[i].c_str() + 6);
         continue;
      }
      if(i < otherArgNum-1)
      {
         result << otherArgs[i] << ": " << otherArgs[i+1] << S_CRLF;
         i++;
      }
   }
   result << S_CRLF;
   if(!content.empty())
      result << content; 
}

int g_clientRequestCount = 0;
int g_clientReceivedCount = 0;

BOOL processAsyncResponse(TUniNetMsg* msg)
{
   int linkId = msg->tAddr.taskInstID;
   PTArgvMsgBody pArgv = (PTArgvMsgBody)msg->msgBody;
   if(pArgv == NULL || pArgv->argc<2)
   {
      UniERROR("SIMPLE_PSA_RESPONSE message missing type and content");
      return FALSE;
   }
   CHttpMessage* httpMsg = g_httpServer->getLinkMsg(linkId);
   if(httpMsg == NULL)
   {
      UniERROR("invalid link %d, can not get the saved request message", linkId);
      return FALSE;
   }
   CStr strRes;
   if(isdigit(pArgv->argv[0].c_str()[0]))
      httpMsg->genErrorMessage(strRes, pArgv->argv[0].toInt(), pArgv->argv[1].c_str());
   else
      httpMsg->genResponseMessage(strRes, pArgv->argv[0].c_str(), pArgv->argv[1].c_str());
   g_httpServer->deleteLinkMsg(linkId);

   g_httpServer->send(linkId, strRes);
   return TRUE;
}

void parseUrl(CStr& url, CStr& addr, int& port)
{
   CStr cutResult;
   url.cut("//", cutResult);
   url.cut("/", cutResult);
   cutResult.cut(":", addr);
   if(cutResult.empty())
      port = 80;
   else
      port = cutResult.toInt();
}

BOOL hookSendMsgPsaHttpImpl(TUniNetMsg* msg)
{
   if(msg == NULL)
   {
      UniERROR("message from app is null");
      return FALSE;
   }

   if(msg->msgName == SIMPLE_PSA_RESPONSE)
   {
      return processAsyncResponse(msg);
   }

   if( msg->msgName != SIMPLE_PSA_REQUEST)
   {
      UniERROR("invalid message '%s' from app", msg->getMsgNameStr());
      return FALSE;
   }

   PTArgvMsgBody pArgv = (PTArgvMsgBody)msg->msgBody;
   if(pArgv == NULL || pArgv->argc<2)
   {
      UniERROR("SIMPLE_PSA_REQUEST message missing url argument");
      return FALSE;
   }

   CStr url = pArgv->argv[0];
   CStr addr;
   int port;
   parseUrl(url, addr, port);
   const char* path = url.c_str();

   if(g_httpClient==NULL)
      g_httpClient = new CTcpConnectionManager();

   int linkId = msg->oAddr.taskInstID;
   
   CHttpClientLink* link = (CHttpClientLink*)(g_httpClient->findByLinkId(linkId));
   if(link == NULL)
   {
      CUri uri(addr.c_str(), port, NULL);
      link = new CHttpClientLink();
      if(link->connect(uri.host(), uri.port(), "", 0))
      {
         link->remoteUri() = uri;
         link->linkId() = linkId;
         ((CHttpClientLink*)link)->appKey = msg->oAddr.logAddr;
         g_httpClient->addConnection(link);
      }
      else
      {
         UniERROR("open the connection failed. remoteUri: %s.", uri.c_str());
         delete link;
         CStr temp;
         httpResponseToApp(msg->oAddr.logAddr, msg->oAddr.taskInstID, temp, 400, temp);
         return FALSE;
      }
   }

   CStr httpRequest;
   buildHttpRequest(link, addr.c_str(), port, path, pArgv->argv[pArgv->argc-1], pArgv->argc-2, &(pArgv->argv[1]), httpRequest);
   CCode code;
   code.length = httpRequest.length();
   code.content = new char[code.length+1];
   memcpy(code.content, httpRequest.c_str(), code.length);
   code.content[code.length] = 0;
   if(g_printSocket)
      link->traceSend(code.content, code.length, "Http client");
   link->send(code);
   delete code.content;
   g_clientRequestCount++;
   return TRUE;
}

void httpResponseToApp(int appKey, int linkId, CStr& version, int rCode, CStr& content)
{
   TUniNetMsg* msg = new TUniNetMsg;
   msg->dialogType = DIALOG_CONTINUE;
   msg->oAddr.logAddr = g_thisPsaId;
   msg->tAddr.taskInstID = linkId;
   msg->tAddr.logAddr = appKey;
   if(msg->tAddr.logAddr<0)
   {
      delete msg;
      UniERROR("can not find the fsm %d when send response to app");
      return;
   }
   msg->msgName = SIMPLE_PSA_RESPONSE;

   TArgvMsgBody* pArgv = new TArgvMsgBody;
   pArgv->argc = 3;
   pArgv->argv[0] = version;
   pArgv->argv[1] = rCode;
   pArgv->argv[2] = content;

   msg->msgBody = pArgv;
   msg->setMsgBody();
   sendMsgToFEAM(msg);
   g_clientReceivedCount++;
}

int initServiceIdMap()
{
   CStr fileName = uniDir;
   fileName << "/etc/psahttp.cfg";
   int re = serviceIdMap.loadFromFile(fileName.c_str(), 0);
   if(re<=0)
   {
      UniINFO("no file %s", fileName.c_str());
      return 0;
   }
   UniINFO("%d item loaded from file %s", re, fileName.c_str());
   return re;
}

void printHttpCodeTable(CStr& str);
void cmd_http(int argc,char* argv[])
{
   if(argc==2 && strcmp(argv[1], "printon")==0)
   {
      g_printSocket = 1;
      AmeFlush();
      return;
   }
   if(argc==2 && strcmp(argv[1], "printdl")==0)
   {
      g_printDownload = 1;
      AmeFlush();
      return;
   }
   if(argc==2 && strcmp(argv[1], "printoff")==0)
   {
      g_printSocket = 0;
      g_printDownload = 0;
      AmeFlush();
      return;
   }
   if(argc==2 && strcmp(argv[1], "code")==0)
   {
      CStr str;
      printHttpCodeTable(str);
      AmeINFO("%s\n", str.c_str());
      AmeFlush();
      return;
   }
   if(argc==2 && strcmp(argv[1], "map")==0)
   {
      CStr str;
      serviceIdMap.printState(str);
      AmeINFO("%s\n", str.c_str());
      AmeFlush();
      return;
   }
   if(argc==3 && strcmp(argv[1], "map")==0 && strcmp(argv[2], "load")==0)
   {
      int re = initServiceIdMap();
      AmeINFO("%d item loaded\n", re);
      AmeFlush();
      return;
   }

   g_httpServer->list();

   if(g_httpClient!=NULL)
   {
      AmeINFO("Client: request: %d, received %d\n", g_clientRequestCount, g_clientReceivedCount);
      CStr str;
      g_httpClient->printState(str);
      AmeINFO("%s\n", str.c_str());
   }
   AmeFlush();
}

TCommandItem psahttpCommandTable[]=
{
   { 0, "http",      cmd_http,     "list the http server and the connections\n"},
   { 0, "",          cmd_http,      ""}
};

void hookCloseResourcePsaHttpImpl()
{
   UniINFO("close resources of PSAHTTP");
   delete g_httpServer;
}

void initPsaHttp(INT psaId)
{
   setHookBuildFdSet(psaId, hookBuildFdSetPsaHttpImpl);		//统一设置fdset
   setHookProcFdSet(psaId, hookProcFdSetPsaHttpImpl);		//处理网络侧接收的消息（或者网络侧fdset检测到网络消息，或者fdset超时）
   setHookSendUniNetMsg(psaId, hookSendMsgPsaHttpImpl);		//处理Kernal要发送到网络侧的消息
   setHookCloseResource(psaId, hookCloseResourcePsaHttpImpl);	//关闭资源

   //设置一个监听端口，这里可以从配置文件获得一个端口
   int port = 79 + CUniFrame::getBasePort() + uniProcessId;
	
   //初始化协议栈
   g_httpServer = new CHttpServer;
   g_httpServer->init(NULL, port);
   UniINFO("init http server, port=%d", port);

   //设置ame控制台输出，当使用ame命令连接到该协议栈的时候，可使用help命令看到这里设置的信息
   new TAbstractAMEHandler(psahttpCommandTable,psaId);

   //设置当前PSA的ID为系统传送进来的psaid
   g_thisPsaId = psaId;

   initServiceIdMap();
}
