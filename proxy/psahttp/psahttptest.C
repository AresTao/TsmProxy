#include "excomp.h"
#include "unihtml.h"

EX_COMP_CmdLine(processHttpRequest)
{
   CHtml& html = (CHtml&)result;
   html.initial("PSAHTTP self test");

   if(argc<4)
      html.elem("div", "argv must greater than 3");
   else
   {
      CStr body;
      body << "path = " << argv[0] << "\n";
      body << "query = " << argv[1] << "\n";
      CDict dict(argv[1]);
      dict.printState(body);
      int contentLength = atoi(argv[2]);
      body << "contentLength = " << contentLength << "\n";
      if(contentLength>0)
      body << "content =";
      body.fCatBin(argv[3], contentLength);
      body << "\n";
      int i;
      for(i=4; i<argc-1; i+=2)
      {
         body << argv[i] << " = " << argv[i+1] << "\n";
      }
      body.htmlEncode();
      html.elem("div", body.c_str());
   }
   html.beginForm();
      html.begin("p");
         html << "Input something to test content: ";
         html.input("text", "test");
         html.input("submit", "submit", "OK");
      html.end();
   html.end();
   html.enclose();
   return 0;
}
