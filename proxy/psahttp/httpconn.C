#include "httpconn.h"

void CHttpConnection::trace(const char* s, int len, const char* info, int isSend)
{
   CStr str;
   str.fCat("%s %s %d bytes %s link %d(%s):"
      , info
      , isSend ? "send" : "receive"
      , len
      , isSend ? "to" : "from"
      , linkId()
      , c_str_remote()
   );
}

void CHttpConnection::traceSend(const char* s, int len, const char* info)
{
   trace(s, len, info, 1);
}

void CHttpConnection::traceReceive(const char* s, int len, const char* info)
{
   trace(s, len, info, 0);
}
