/* httpserver.C, by Zhang Haibin, 2011-1-26
 */

#include "httpserver.h"
#include "info.h"

int processHttpMessage(CHttpMessage* msg);

CHttpServer::CHttpServer(CHttpHandler *task) : mTask(task),listener(FALSE)
{
   mFifo = new Fifo<CHttpMessage>();
   listener.setShortConnectionMode();
   receivedCount = 0;
   responseCount = 0;
   acceptCount = 0;
}

CHttpServer::~CHttpServer()
{
   links.closeAll();
}

void CHttpServer::init(char* host, int port)
{
   int re = listener.open(host, port);
}

void CHttpServer::list()
{
   CStr s;
   listener.serverInfo(&s);
   s.fCat(", accept %d, received %d, response %d\n", acceptCount, receivedCount, responseCount);
   s.fCat("LINK FD   STATE SEND-BUFFER REMOTE-ADDR\n");
   cout<<"server list"<<endl;
   links.printConnectionsState(s);
}

CHttpMessage* CHttpServer::getLinkMsg(int linkId)
{
   CHttpLink* link = (CHttpLink*)links.findByLinkId(linkId);   
   if(link == NULL)
      return NULL;
   return link->msg;
}

void CHttpServer::deleteLinkMsg(int linkId)
{
   CHttpLink* link = (CHttpLink*)links.findByLinkId(linkId);   
   if(link == NULL || link->msg == NULL)
      return;
   delete link->msg;
   link->msg = NULL;
}

void CHttpServer::send(int linkId, CStr& str)
{
   CHttpLink* link = (CHttpLink*)links.findByLinkId(linkId);   
   if(link==NULL)
   {
      UniERROR("send message to a null link");
      return;
   }
   link->sendStream.content = str.steal(link->sendStream.len);
   responseCount ++;
}

//extern int g_printSocket;

void CHttpLink::processSend()
{
   if(sendStream.content==NULL)
      return;
   if(sendStream.pos >= sendStream.len)
   {
      sendStream.clear();
      return;
   }

   CCode code;
   code.length = sendStream.len - sendStream.pos;
   if(code.length > SOCKET_CODE_MAXLENGTH)
      code.length = SOCKET_CODE_MAXLENGTH;
   code.content = sendStream.content+sendStream.pos;
  // if(g_printSocket)
   //   traceSend(code.content, code.length, "Http server");
   sendCode(code);

   sendStream.pos += code.length;
   if(sendStream.pos >= sendStream.len)
      sendStream.clear();
}

int CHttpLink::processRecv()
{
   mCode.length = 0;
   recvCode(mCode);
   if(mCode.length == 0)
      return -1;

//   if(g_printSocket)
 //     traceReceive(mCode.content, mCode.length, "Http server");
   if(msg==NULL)
      msg=new CHttpMessage(linkId());
   int re = msg->parseStream(mCode);
   cout<<"httplink processrecv"<<endl;
   if(re>0)
   {
       postTo();
       delete msg;
       msg=NULL;
 /*  if(processHttpMessage(msg) == 0)
      {
         delete msg;
         msg = NULL;
      }*/
   }
   return re;
}

void CHttpLink::printState(CStr& str)
{
   if(getFd() <= 0)
      return;
   char sendBufferState[32];
   sendStream.state(sendBufferState);
   str.fCat("[%2d] %-4d %-5s %-11s %s\n"
      , linkId()
      , getFd()
      , isSocketStateClose()?"CLOSE":"OPEN"
      , sendBufferState
      , remoteUri().c_str()
   );
}
bool CHttpLink::postTo()
{
   if(msg != NULL)
   {
       mServer->add(msg);
   }
}

BOOL CHttpServer::check(CFdSet& fdSet)
{
   int i;
   if(listener.accept(fdSet))
   {
      acceptCount ++;
      cout<<"acceptCount" << acceptCount<<endl;
      CHttpLink* link = new CHttpLink(this);
      listener.dupConnection(link);
      link->linkId() = acceptCount;
      links.addConnection(link);
   }
   receivedCount += links.procFdSet(fdSet);
   return TRUE;
}

void CHttpServer::prepareFdSet(CFdSet& fdSet)
{
   cout<<"buildfdset"<<endl;
   listener.buildFdSet(fdSet);
   links.buildFdSet(fdSet);
}
bool CHttpServer::add(CHttpMessage *msg)
{
   mFifo->add(msg);
   cout<<mFifo->size()<<endl;
   procMsg();
   return true;
}
bool CHttpServer::procMsg()
{
   CHttpMessage* msg = mFifo->getNext(1000);
   if(msg != NULL)
   {   mTask->add(msg);
   mTask->post();
   }
}
bool CHttpServer::process()
{
}
void CHttpServer::closeLink(int i)
{
   links.closeByLinkId(i);
}
