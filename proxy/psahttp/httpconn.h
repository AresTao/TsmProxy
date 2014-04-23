#ifndef _HTTP_CONN_H
#define _HTTP_CONN_H

#include "socket.h"

class CHttpConnection : public CTcpConnection
{
public:
   void traceSend(const char* s, int len, const char* info);
   void traceReceive(const char* s, int len, const char* info);

private:
   void trace(const char* s, int len, const char* info, int isSend);
};

#endif
