/* httpclient.h, by Zhang Haibin, 2011-9-6
 */

#ifndef _HTTPCLIENT_H
#define _HTTPCLIENT_H

#include "httpconn.h"

class CHttpClientLink : public CHttpConnection
{
public:
   CHttpClientLink();
   ~CHttpClientLink();

   int appKey;
   void setDownload(const char* downloadFileName);

private:
   int processRecv();
   CCode mCode;

   CStr version;
   int rCode;
   int contentLength;
   CStr line;
   CStr content;

   CStr downloadFile;
   int downloadBytes;

#define http_r_GetHeaderLine 0
#define http_r_GetCrlf 1
#define http_r_GetCrlf2 2
#define http_r_GetContent 3
   int state;

   int parseStream(const char* p, int length);
   void clearParseState();
   void logDownloadState(int count);
   bool writeToFile(const char* s, int len);
};

#endif

