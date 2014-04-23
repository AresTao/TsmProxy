#include "threadif.h"
#include "httpserver.h"

class HttpClientThread : public ThreadIf
{
    public:
        HttpClientThread(CHttpServer& server);
        virtual ~HttpClientThread();
        virtual void process();
     //   virtual void buildFdSet(CFdSet & fdset);
     //   virtual unsigned int getTimeTillNextProcessMS() const;
      //  virtual void beforeProcess() {};
        virtual void onstart();
       // virtual void afterProcess() {};
    private:
        CTcpConnectionManager &mClient;

};
