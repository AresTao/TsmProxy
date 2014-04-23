#include "threadif.h"
#include "httpserver.h"

class HttpThread : public ThreadIf
{
    public:
        HttpThread(CHttpServer& server);
        virtual ~HttpThread();
        virtual void process();
     //   virtual void buildFdSet(CFdSet & fdset);
     //   virtual unsigned int getTimeTillNextProcessMS() const;
      //  virtual void beforeProcess() {};
        virtual void onstart();
       // virtual void afterProcess() {};
    private:
        CHttpServer &mServer;

};
