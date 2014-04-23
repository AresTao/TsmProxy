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
========Э��ջ����============
INIT_PSA_COMP ����������Э��ջ���֣������Ը�Э��ջ�������������Э��ջ��ʼ��������
	Э��ջ��ʼ�������� INIT_PSA_COMP ���壬��ʽΪ initXXXXXX������XXXXXX��INIT_PSA_COMP����һ�¡�
	Э��ջ��ʼ������һ��Ҫ�� INIT_PSA_COMP ��֮ǰ�������Ա�֤INIT_PSA_COMP���ܹ��ҵ��ú���
	���磬������PsaNameΪ���ֵ�Э��ջ��
	
	void initPsaName(INT psaId);
	
	INIT_PSA_COMP(PsaName)
	
========Э��ջ��ʼ��===========
Э��ջ��ʼ��������initXXXX������Э��ջ�ж����һ�麯���ҽӵ�uniframe�У���֧����Ϣ�շ�����һ�鹦����Ҫ������
	setHookBuildFdSet(psaId, fdset���ú���);
	setHookProcFdSet(psaId, fdset��������);
	setHookSendUniNetMsg(psaId, ��Ϣ���ʹ�������);
	setHookCloseResource(psaId, Э��ջ�ر�ǰ����Դ��������);
	
	���ֺ����Ķ���ʾ����
	void hookBuildFdSetPsaNameImpl(CFdSet& fdSet){}						//fdset���ú���
	BOOL hookProcFdSetPsaNameImpl(CFdSet& fdSet) { return TRUE/FALSE;}	//fdset��������
	BOOL hookSendMsgPsaNameImpl(TUniNetMsg* msg) { return TRUE/FALSE;}	//��Ϣ���ʹ�������
	void hookCloseResourcePsaNameImpl(){}								//Э��ջ�ر�ǰ����Դ��������
	
=========������Ϣ================
��Ӧ�ð���Ϣ���͵�Э��ջ����uniframe�����Э��ջ��ʼ����������setHookSendUniNetMsg����ע��ķ�����Ϣ������
	���磺
	
	BOOL hookSendMsgPsaNameImpl(TUniNetMsg* msg) 
	{
		//�ú���������UniNetMsg��Ϣָ�룬Э��ջ������Ϣ���ͳ�ȥ����
		return TRUE/FALSE;
	}

=========������Ϣ================
����Ҫ��������ϱ�����Ϣ������Ҫ����������	
1��ʵ��FDSet���ú���
	ϵͳ������ʱ�����������fdset���ú�������uniframe����øú���������uniframeͳһ������fdset��
	
	���磺
	void hookBuildFdSetPsaNameImpl(CFdSet& fdSet)
	{
		//���FDSet��Uniframeͳһ����������FDSet��������Ҫ������socket�������õ���fdset�У���֤fdset�ܹ���Э��ջ����ͳһ��select������
	}		
	
2��ʵ��FDSet��������
	��ʵ����fdset���ã��򵱼�⵽�����յ���Ϣ����fdset����select��������ʱfdset���������ᱻ����
	���磺
	BOOL hookProcFdSetPsaNameImpl(CFdSet& fdSet) 
	{ 
		//Э��ջ��Ҫ����fdset�ж��Ƿ��Լ��յ���Ϣ������������Ϣ����
		return TRUE/FALSE;
	}

=========�ر�Э��ջ================	
��Э��ջ���رյ�ʱ��Э��ջ�رպ����ᱻ���ã�ϵͳ�ڴ��ͷ�Э��ջ�������Դ��
	���磺
	void hookCloseResourcePsaNameImpl()
	{	}	
************************************************************************************************************/


//========��ʼ����������===========
// INT psaId����ǰЭ��ջ������ı�ʶ����һ��ʶ��uniframe������Ψһ��ʶЭ��ջ����ʶЭ��ջ����
void initPsaHttp(INT psaId);

//��ʼ��Э��ջ�Զ��庯��������
INIT_PSA_COMP(PsaHttp)

CDict serviceIdMap;

CHttpServer* g_httpServer = NULL;
int g_printSocket = 0;
int g_printDownload = 0;
CTcpConnectionManager* g_httpClient = NULL;
int g_thisPsaId;
void httpResponseToApp(int appKey, int linkId, CStr& version, int rCode, CStr& content);

//===========fdset���ú���============
// CFdSet& fdSet��fdset����
//	��Э��ջnew����socket�������õ�fdset�����У���֤fdset�����ռ���ȫ����socket����ͳһ���м���
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
   setHookBuildFdSet(psaId, hookBuildFdSetPsaHttpImpl);		//ͳһ����fdset
   setHookProcFdSet(psaId, hookProcFdSetPsaHttpImpl);		//�����������յ���Ϣ�����������fdset��⵽������Ϣ������fdset��ʱ��
   setHookSendUniNetMsg(psaId, hookSendMsgPsaHttpImpl);		//����KernalҪ���͵���������Ϣ
   setHookCloseResource(psaId, hookCloseResourcePsaHttpImpl);	//�ر���Դ

   //����һ�������˿ڣ�������Դ������ļ����һ���˿�
   int port = 79 + CUniFrame::getBasePort() + uniProcessId;
	
   //��ʼ��Э��ջ
   g_httpServer = new CHttpServer;
   g_httpServer->init(NULL, port);
   UniINFO("init http server, port=%d", port);

   //����ame����̨�������ʹ��ame�������ӵ���Э��ջ��ʱ�򣬿�ʹ��help������������õ���Ϣ
   new TAbstractAMEHandler(psahttpCommandTable,psaId);

   //���õ�ǰPSA��IDΪϵͳ���ͽ�����psaid
   g_thisPsaId = psaId;

   initServiceIdMap();
}