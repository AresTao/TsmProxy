/* httpmessage.C, by Zhang Haibin, 2011-1-26
 */

#include "unihtml.h"
#include "httpmessage.h"
#include "env.h"
#include "func.h"

CHttpMessage::CHttpMessage()
{
   state = http_Idle;
   headerList = NULL;
   contentLength = 0;
}

CHttpMessage::CHttpMessage(int i)
{
   linkId = i;
   state = http_Idle;
   headerList = NULL;
   contentLength = 0;
}
CHttpMessage::CHttpMessage(const CHttpMessage & r)
{
        init(r);
}

CHttpMessage &CHttpMessage::operator=(const CHttpMessage &r)
{
	freeMem();
	init(r);
        return *this;
}
void CHttpMessage::init(const CHttpMessage &r)
{
	content =r.content;
	state = r.state;
	query = r.query;
	curLine = r.curLine;
	contentLength = r.contentLength;
	headerList = r.headerList;
	linkId = r.linkId;
	method = r.method;
	path = r.path;
	version = r.version;
}

void CHttpMessage::freeMem()
{
    while(headerList!=NULL)
   {
      CHttpHeaderItem* p = headerList;
      headerList = p->next;
      p->next = NULL;
      delete p;
   }
     
}
CHttpMessage::~CHttpMessage()
{
   while(headerList!=NULL)
   {
      CHttpHeaderItem* p = headerList;
      headerList = p->next;
      p->next = NULL;
      delete p;
   }
}

void CHttpMessage::addHeader(CStr line)
{
   char* p = (char*)strstr(line.c_str(), ":");
   *p=0;
   p++;
   while(*p==' ')
      p++;
   CHttpHeaderItem* item = new CHttpHeaderItem;
   item->name = line.c_str();
   item->value = p;
   item->next = NULL;

   if(strcasecmp(item->name.c_str(), "Content-Length") == 0)
   {
      contentLength = atoi(item->value.c_str());
      contentLengthStr = item->value;
   }

   if(headerList == NULL)
   {
      headerList = item;
      return;
   }
   CHttpHeaderItem* pItem = headerList;
   while(pItem->next != NULL)
      pItem = pItem->next;
   pItem->next = item;
}

int CHttpMessage::parseStream(CCode code)
{
   sourceContent.nCat(code.content, code.length);
   char* p = code.content;
   char* begin = code.content;
   char* end = code.content + code.length;
   *end = 0;
   isRequest = true; 
   while(p < end)
   {
      switch(state) 
      {
      case http_Idle: 
         while( p < end && *p != ' ')
         {
            method += *p;
            p++;
         }
         if(*p == ' ')
         {
            p++;
            p++;
            state = http_GetPath;
            continue;
         }
         return 0;

      case http_GetPath:
         while( p < end && *p != ' ' && *p != '?')
         {
            path += *p;
            p++;
         }
         if( *p == '?' )
         {
            p++;
            state = http_GetQuery;
            continue;
         }
         if( *p == ' ' )
         {
            p++;
            state = http_GetVersion;
            continue;
         }
         return 0;
            
      case http_GetQuery:
         while( p < end && *p != ' ')
         {
            query += *p;
            p++;
         }
         if( *p == ' ' )
         {
            p++;
            state = http_GetVersion;
            continue;
         }
         return 0;

      case http_GetVersion:
         while( p < end && *p != '\r')
         {
            version += *p;
            p++;
         }
         if( *p == '\r' )
         {
            if(version != "HTTP/1.0" && version != "HTTP/1.1")
            {
               UniERROR("invalid http version [%s]", version.c_str());
               return -1;
            }
            p++;
            state = http_GetCRLF;
            continue;
         }
         return 0;

      case http_GetCRLF:
         if(*p != '\n')
         {
            cout<<"http message missing '\\n'";
            return -1;
         }
         p++;
         curLine.clear();
         state = http_GetLineBegin;
         continue;

      case http_GetLineBegin:
         if(*p=='\r')
         {
            p++;
            state = http_GetDoubleCRLF;
            continue;
         }
         state = http_GetLine;
         continue;

      case http_GetDoubleCRLF:
         if(*p != '\n')
         {
            cout<<"http message missing '\\n2'";
            return -1;
         }
         if(contentLength==0)
         {
            return 1;
         }
         p++;
         state = http_GetContent;
         continue;

      case http_GetLine:
         while( p < end && *p != '\r')
         {
            curLine += *p;
            p++;
         }
         if(*p == '\r')
         {
            addHeader(curLine);
            p++;
            state = http_GetCRLF;
            continue;
         }
         return 0;

      case http_GetContent:
         cout<<"http get content"<<endl;
         while(p < end)
         {
             content += *p;
             p++;
         }
         cout<<content.c_str()<<endl;
         //content.nCat(p, code.length - (p-code.content));
         //if(content.length() >= contentLength)
         //{
         //   return 1;
         //}
         return 1;

      default:
         cout<<"http request messgage parse: invalid state"<<endl;
         return -1;
      }
   }
   return 0;
}

void CHttpMessage::print(CStr& str)
{
   str.fCat("linkid = %d\n\n", linkId);
   str.fCat("method = %s\n", method.c_str());
   str.fCat("path = %s\n", path.c_str());
   str.fCat("query = %s\n", query.c_str());
   str.fCat("version = %s\n", version.c_str());
   str.fCat("contentLength = %d\n", contentLength);
  /* CHttpHeaderItem* p = headerList;
   while(p != NULL)
   {
      str.fCat("%s = %s\n", p->name.c_str(), p->value.c_str());
      p = p->next;
   }
   str.fCat("content = \n");
   str.fCatBin(content.c_str(), content.length());*/
}

const char* CHttpMessage::pathExt()
{
   if(path.length()<2)
      return NULL;
   const char* p = path.c_str() + path.length()-2;
   while(p>path.c_str() && *p != '/')
   {
      if(*p == '.')
         return p+1;
      p--;
   }
   return NULL;
}

static const char* g_httpCodeTable[]=
{
   "Continue",
   "Switching Protocols",
   "","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
   "OK",
   "Created",
   "Accepted",
   "Non-Authoritative Information",
   "No Content",
   "Reset Content",
   "Partial Content",
   "","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","", 
   "Multiple Choices",
   "Moved Permanently",
   "Found",
   "See Other",
   "Not Modified",
   "Use Proxy",
   "",
   "Temporary Redirect",
   "","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","",
   "Bad Request",
   "Unauthorized",
   "Payment Required",
   "Forbidden",
   "Not Found",
   "Method Not Allowed",
   "Not Acceptable",
   "Proxy Authentication Required",
   "Request Timeout",
   "Conflict",
   "Gone",
   "Length Required",
   "Precondition Failed",
   "Request Entity Too Large",
   "Request-URI Too Long",
   "Unsupported Media Type",
   "Requested Range Not Satisfiable",
   "Expectation Failed",
   "","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","","", 
   "Internal Server Error",
   "Not Implemented",
   "Bad Gateway",
   "Service Unavailable",
   "Gateway Timeout",
   "HTTP Version Not Supported",
};
#define HttpCodeTableLen (sizeof(g_httpCodeTable)/sizeof(const char*))

const char* httpCodeStr(int code)
{
   if(code<100)
      return "Unknown";
   code -= 100;
   if(code >= HttpCodeTableLen)
      return "Unknown";
   if(g_httpCodeTable[code][0]==0)
      return "Unknown";
   return g_httpCodeTable[code];
}

void printHttpCodeTable(CStr& str)
{
   int i;
   for(i=0; i<HttpCodeTableLen; i++)
   {
      if(g_httpCodeTable[i][0]!=0)
         str.fCat("%d %s\n", i+100, g_httpCodeTable[i]);
   }
}

void CHttpMessage::genTestResponseMessage(CStr& str)
{
   CStr msgInfo;
   print(msgInfo);
   msgInfo.htmlEncode();

   CHtml str1;
   str1.initial("psahttp self test"); 
   CStr sourceContent1 = sourceContent;
   sourceContent1.htmlEncode();
   str1.elem("div", sourceContent1.c_str());
   str1.elem("div", msgInfo.c_str());
   str1.enclose();

   str.fCat("%s 200 OK\r\n" , version.c_str());
   str.fCat("Content-Length: %d\r\n", str1.length());
   str.fCat("Content-Type: text/html\r\n");
   str += "\r\n";
   str += str1;
}

void CHttpMessage::genResponseMessage(CStr& str, const char* type, const char* rContent)
{
   str.fCat("%s 200 OK\r\n" , version.c_str());
   str.fCat("Content-Length: %d\r\n", strlen(rContent));
   str.fCat("Content-Type: %s\r\n", type);
   str += "\r\n";
   str += rContent;
}

void CHttpMessage::genErrorMessage(CStr& str, int rCode, const char* info)
{
   const char* codeStr = httpCodeStr(rCode);
   // str.fCat("%s %d %s\r\n" , version.c_str(), rCode, codeStr);
   str.fCat("%s 200 OK\r\n" , version.c_str());

   CHtml rContent;
   rContent.initial(codeStr); 
   rContent.elem("h1", codeStr);
   rContent.elem("p", info);
   rContent.enclose();

   str.fCat("Content-Length: %d\r\n", rContent.length());
   str.fCat("Content-Type: text/html\r\n");
   str += "\r\n";
   str += rContent;
};

struct CMimeFileExtTypeMap
{
   const char* ext;
   const char* type;
}g_mimeFileExtTypeMapTable[]=
{
   { "html", "text/html" },
   { "js",   "application/x-javascript" },
   { "css",  "text/css" },

   { "jpg",  "image/jpeg" },
   { "jpeg", "image/jpeg" },
   { "gif",  "image/gif"  },
   { "ico",  "image/ico" },
   { "png",  "image/png" },
   { "mpeg", "video/mpeg" },
   { "wav",  "audio/x-wav" },
   { "midi", "audio/x-midi" },
   { "avi",  "video/x-msvideo" },

   { "xml",  "text/xml" },
   { "vxml", "text/vxml" },
   { "txt",  "text/plain" },
   { "text", "text/plain" },
   { "si",   "text/vnd.wap.si" },

   { "smil", "application/smil" },
   { "pdf",  "application/unknow" },
   { "doc",  "application/unknow" },
   { "docx", "application/unknow" },

   { "rar",  "application/unknow" },
   { "zip",  "application/unknow" },
   { "tgz",  "application/unknow" },
   { "gz",   "application/unknow" },
   { "",     "application/octet-stream" }
};
const char* mimeFileExtType(const char* ext)
{
   int i = 0;
   while(g_mimeFileExtTypeMapTable[i].ext[0]!=0)
   {
      if(strcmp(g_mimeFileExtTypeMapTable[i].ext, ext)==0)
         return g_mimeFileExtTypeMapTable[i].type;
      i++;
   }
   return NULL;
}
void CHttpMessage::genFileResponseMessage(CStr& str)
{
   const char* ext = pathExt();
   if(ext==NULL)
   {
      genErrorMessage(str, 400, "no ext");
      return;
   }
   const char* type = mimeFileExtType(ext);
   if(type==NULL)
   {
      genErrorMessage(str, 400, "invalid ext");
      return;
   }

   CStr fileName;
   fileName << "/app/" << path;
   int length = getFileLength(fileName.c_str());
   if(length == 0)
   {
      CStr errMsg;
      errMsg << "file '" << fileName << "' not existed or size is 0";
      genErrorMessage(str, 400, errMsg.c_str());
      return;
   }
   
   str.fCat("%s 200 OK\r\n" , version.c_str());
   str.fCat("Content-Length: %d\r\n", length);
   str.fCat("Content-Type: %s\r\n", type);
   str += "\r\n";

   str.fileCat(fileName.c_str());
}

static int _headerLostCounter = 0;

#define BUILD_ARGV \
   argv[0] = path.c_str();\
   argv[1] = query.c_str();\
   argv[2] = contentLengthStr.c_str();\
   argv[3] = content.c_str();\
   argc = 4;\
   CHttpHeaderItem* p = headerList;\
   while(argc<size-1 && p != NULL)\
   {\
      argv[argc] = p->name.c_str();\
      argc++; \
      argv[argc] = p->value.c_str();\
      argc++;\
      p = p->next;\
   }\
   if(p!=NULL && (_headerLostCounter++)%1000==0)\
   {\
      UniERROR("some http header lost");\
   }\

void CHttpMessage::buildAsyncAppArgv(int& argc, CStr argv[], int size)
{
   BUILD_ARGV
   argv[argc] = "All-Header";
   argc++;
   p = headerList;
   while(p != NULL)
   {
      argv[argc] << p->name << ": " << p->value << "\r\n";
      p = p->next;
   }
   argc++;
}

void CHttpMessage::buildDllCallArgv(int& argc, const char** argv, int size)
{
   BUILD_ARGV
}

