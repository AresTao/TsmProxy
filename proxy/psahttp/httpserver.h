/* httpserver.h, by Zhang Haibin, 2011-1-26
 */

#ifndef _HTTPSERVER_H
#define _HTTPSERVER_H

#include "socket.h"
#include "fdset.h"
#include "httpmessage.h"
#include "httpconn.h"
#include "fifo.h"
#include "abstracttask.h"
#include "httphandler.h"

struct CSendStream
{
   int pos;
   int len;
   char* content;
   CSendStream()
   {
      pos = 0;
      len = 0;
      content = NULL;
   }
   void clear()
   {
      delete content;
      content = NULL;
      pos = 0;
      len = 0;
   }
   ~CSendStream()
   {
      if(content!=NULL)
         delete content;
   }
   void state(char* info)
   {
      if(content == NULL)
         strcpy(info, "(null)");
      else
         sprintf(info, "%dM/%dM", pos/1000000, len/1000000);
   }
};

class CHttpServer;
class CHttpHandler;
struct CHttpLink : public CHttpConnection
{
   CHttpMessage* msg;
   CSendStream sendStream;

   CHttpLink(CHttpServer *server):mServer(server)
   {
      msg = NULL;
      mCode.content = new char[MaxMsgLength];
      mCode.length = MaxMsgLength;
   }
   ~CHttpLink()
   {
      if(msg!=NULL)
         delete msg;
      delete[] mCode.content;
   }
   void processSend();
   int processRecv();
   void printState(CStr& str);
   int needSend() { return sendStream.content!=NULL && sendStream.pos < sendStream.len; }
   bool postTo();
private:
   CCode mCode;
   CHttpServer *mServer;
};

class CHttpServer
{
   CTcpConnectionManager links;
   CTcpListener listener;
   int responseCount;
   int receivedCount;
   int acceptCount;
   CHttpHandler * mTask;
   void closeLink(int i);
   Fifo<CHttpMessage > * mFifo;

public:
   CHttpServer(CHttpHandler *task);
   ~CHttpServer();
   void init(char* host, int port);
   void prepareFdSet(CFdSet& fdSet);
   BOOL check(CFdSet& fdSet);
   void send(int linkId, CStr& str);
   CHttpMessage* getLinkMsg(int linkId);
   void deleteLinkMsg(int linkId);
   void list();
   bool add(CHttpMessage *msg);
   bool procMsg();
   bool process();
};

#endif


