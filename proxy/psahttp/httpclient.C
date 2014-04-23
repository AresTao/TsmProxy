/* httpclient.C, by Zhang Haibin, 2011-9-6
 */

#include "httpclient.h"
#include "info.h"
CHttpClientLink::CHttpClientLink()
{
   mCode.content = new char[MaxMsgLength];
   mCode.length = MaxMsgLength;

   clearParseState();
   setShortConnectionMode();
}

CHttpClientLink::~CHttpClientLink()
{
   delete[] mCode.content;
}

void CHttpClientLink::clearParseState()
{
   state = http_r_GetHeaderLine;
   contentLength = -1;
   version.clear();
   rCode = 0;
   line.clear();
   content.clear();
}

   
int CHttpClientLink::parseStream(const char* p, int length)
{
   for(int i=0; i<length; i++)
   {
      switch(state)
      {
      case http_r_GetHeaderLine:
         if(p[i] == '\r')
         {
            if(line.empty())
            {
               state = http_r_GetCrlf2;
            }
            else
               state = http_r_GetCrlf;
         }
         else
            line += p[i];
         break;
      case http_r_GetCrlf:
         if(p[i] == '\n')
         {
            // get a line
            if(version.empty())
            {
               line.cut(" ", version);
               CStr rCodeStr;
               line.cut(" ", rCodeStr);
               rCode = rCodeStr.toInt();
            }
            else
            {
               CStr name;
               line.cut(":", name);
               if(strcasecmp(name.c_str(), "Content-Length")==0)
               {
                  line.trim();
                  contentLength = line.toInt();
               }
            }
            line.clear();
            state = http_r_GetHeaderLine;
         }
         else
         {
            UniERROR("http response missing LF");
            return -1;
         }
         break;
      case http_r_GetCrlf2:
         if(p[i] == '\n')
         {
            if(contentLength == -1 || contentLength == 0)
               return 1;
            state = http_r_GetContent;
         }
         else
         {
            UniERROR("http response missing LF");
            return -1;
         }
         break;
      case http_r_GetContent:
         if(downloadFile.empty())
         {
            content.nCat(p + i, length - i);
            logDownloadState(content.length());
            if(content.length() >= contentLength)
               return 1;
         }
         else
         {
            if(!writeToFile(p + i, length - i))
               return -1;
            logDownloadState(downloadBytes);
            if(downloadBytes >= contentLength)
            {
               return 1;
            }
         }
         return 0;
      default:
         UniERROR("invalid state %d when client parse http response");
         return -1;
      }
   }
   return 0;
}

//void httpResponseToApp(int appKey, int linkId, CStr& version, int rCode, CStr& content);

int CHttpClientLink::processRecv()
{
   mCode.clear();
   int rt = receive(mCode);
   if(rt==1)
   {
      int re = parseStream(mCode.content, mCode.length);
      if(re == 1)
      {
  //       httpResponseToApp(appKey, linkId(), version, rCode, content);
         return -1;
      }
      else if(re<0)
      {
         return -1;
      }
   }
   else if( -9 == rt || 0 == rt)
   {
      return -1;
   }
   return 1;
}

void CHttpClientLink::setDownload(const char* downloadFileName)
{
   downloadFile = downloadFileName; 
   downloadBytes = 0;
}

void CHttpClientLink::logDownloadState(int count)
{
      return;
}

bool CHttpClientLink::writeToFile(const char* s, int len)
{
   FILE* f = NULL;
   if(downloadBytes == 0)
   {
   }
   else
      f = fopen(downloadFile.c_str(), "a");
   if(f == NULL)
   {
      UniERROR("fopen %s failed", downloadFile.c_str());
      return false;
   }
   int n = fwrite(s, 1, len, f);
   fclose(f);
   if(n != len)
   {
      UniERROR("fwrite failed, %d/%d wrote", n, len);
      return false;
   }
   downloadBytes += len;
   return true;
}
