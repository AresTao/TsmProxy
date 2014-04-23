/* httpmessage.h, by Zhang Haibin, 2011-1-26
 */

#ifndef _HTTPMESSAGE_H
#define _HTTPMESSAGE_H

#include "comtypedef_vchar.h"

struct CHttpHeaderItem
{
   CStr name;
   CStr value;
   CHttpHeaderItem* next;
};

class CHttpMessage
{
#define http_Idle 0
#define http_GetPath 1
#define http_GetQuery 2
#define http_GetVersion 3
#define http_GetCRLF 4
#define http_GetLineBegin 5
#define http_GetLine 6
#define http_GetDoubleCRLF 7
#define http_GetContent 8
   int state;

   CStr query;
   CStr curLine;
   int contentLength;
   CStr contentLengthStr;
  
   CHttpHeaderItem* headerList;
   void addHeader(CStr line);

   CStr sourceContent;
   bool isRequest;//1 req 0 resp
public:
   string content;
   int linkId;
   CStr method;
   CStr path;
   CStr version;
   
   CHttpMessage();
   CHttpMessage(int i);
   CHttpMessage(const CHttpMessage& r);
   CHttpMessage &operator = (const CHttpMessage &r);
   void init(const CHttpMessage &r);
   void setReq(bool req){ isRequest = req;};
   bool isReq(){ return isRequest;};
   void freeMem();
   ~CHttpMessage();
   int parseStream(CCode code);
   void print(CStr& str);
   const char* pathExt();
   int getContentLength(){ return contentLength; };
   void genTestResponseMessage(CStr& str);
   void genResponseMessage(CStr& str, const char* type, const char* content="");
   void genErrorMessage(CStr& str, int rCode, const char* info="");
   void genFileResponseMessage(CStr& str);

   void buildDllCallArgv(int& argc, const char** argv, int size);
   void buildAsyncAppArgv(int& argc, CStr argv[], int size);
};

#endif
