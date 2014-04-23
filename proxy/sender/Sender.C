#include "Sender.h"
#include "msgdef_http.h"
#include "json.h"
#include "jsonvalue.h"
#include<string.h>
#include<cstdlib>
#include <memory.h>
Sender::Sender() {
  
    //mFifo = new Fifo<CHttpMessage>(); 
    mClient = new CTcpConnectionManager();
    mTimer = setTimer(UPDATETIMER);
    mRegLinkID = 10001;
}

Sender::~Sender()
{
    if(mClient) delete mClient;
}

BOOL Sender::onInit(TiXmlElement* extend)
{
    TiXmlHandle handle(extend);
    TiXmlElement* client = NULL;
    client = handle.FirstChild("client").Element();
    if(client)
    {
        
/*        TiXmlHandle psahandle(psa);
        TiXmlElement * binds = NULL;
        binds = psahandle.FirstChild("binds").Element();
        if(binds)
	{
                TiXmlHandle bindhandle(binds);
	        TiXmlElement*	bind=NULL;
		bind=bindhandle.FirstChild("bind").Element();
                if(bind)
                {
			int port = 8088;
			mServer = new CHttpServer(this);
                        mThread = new HttpThread(*mServer);
			mServer->init(NULL, port);
                        mServer->list();
                        mThread->run();
                    
		}else
                {
                        
			UniERROR("httppsa etc bind error");
                }
         }else
         {
		 UniERROR("httppsa etc binds error");

         }
	 TiXmlElement * targets = NULL;
	 targets = psahandle.FirstChild("target").Element();
         if(targets)
         {
             if(!targets->Attribute("regTask", &mRegID)) mRegID = 1001;
             if(!targets->Attribute("pubTask", &mPubID)) mPubID = 1002;
             if(!targets->Attribute("notTask", &mNotID)) mNotID = 1003;
             if(!targets->Attribute("subTask", &mSubID)) mSubID = 1004;
         }*/
         cout<<"sender init"<<endl;
    }else
    {
         UniERROR("httppsa etc error");
    }
    return true;

}

void Sender::procMsg(std::auto_ptr<TUniNetMsg> msg)
{
    TUniNetMsg * unimsg = msg.get();
    if(!unimsg) return;
    if(!unimsg->hasMsgBody()) return;
    cout<<"sender test procMsg"<<endl;
    THttpMsgReq *httpbody = NULL;
    httpbody = dynamic_cast<THttpMsgReq *>(unimsg->msgBody);
    
    cout<<"hmsg "<<httpbody->hMsg->content.c_str()<<endl;
    CHttpMessage *hmsg = httpbody->hMsg;

   // unimsg->tAddr.logAddr = mRegID;
   // sendMsg(msg);
    
    if(hmsg->content.size() == 0)
    {
        cout<<"no content"<<endl;
        return;
    }else
    {
        //CStr tmp;
        //hmsg->print(tmp);
        //cout<<tmp.c_str()<<endl;
        if(hmsg->isReq())
        {
		cout<<"content :"<<hmsg->content.c_str()<<endl;
		JSONValue *value = JSON::Parse(hmsg->content.c_str());

		if(value)
		{
			JSONObject root;
			if(value->IsObject() == false)
			{
				cout<<"root element is not an object"<<endl;
			}
			else
			{
				root= value->AsObject();
				if(root.find(L"MsgType")!= root.end())
				{
					string msgType;
					wstring tmp  = root[L"MsgType"]->AsString();
					WStringToString(tmp, msgType);
					if(msgType.compare("REGISTER")== 0)
					{
                                            CUri uri("10.109.247.80", 8088, NULL);
                                            CHttpClientLink *reglink = new CHttpClientLink();
                                            if(reglink->connect(uri.host(), uri.port(), "", 0))
                                            {
                                            cout<<"register sender"<<endl;
                                                reglink->remoteUri() = uri;
                                                reglink->linkId() = mRegLinkID;
                                                mClient->addConnection(reglink);
                                            }else
                                            {
                                                cout<<"reg server can't be connected."<<endl;
                                            }
                                            char *request = "test"; 
                                            CCode code;
                                            code.length = strlen(request);
                                            code.content = new char[code.length+1];
                                            memcpy(code.content, request, code.length);
                                            code.content[code.length] = 0;
                                            reglink->send(code);
                                            delete code.content;                          
					}
					else if(msgType.compare("SUBSCRIBE")== 0)
					{
					}
					else if(msgType.compare("PUBLISH")== 0)
					{
					}
					else
					{
					}
					sendMsg(msg);
					return;
				}
				else
				{
					cout<<"no msgtype"<<endl;
					return;
				}
			}    
		}else
		{
			cout<<"消息解析失败"<<endl;
		}
	}else
        {
                cout<<"不合理分配"<<endl;
        }
    }
}
/*
bool Sender::add(CHttpMessage *msg)
{
    mFifo->add(msg);
    return true;
}

void Sender::post()
{
    CHttpMessage *msg = getMsg();
    //cout<<"task post msg "<<msg->content.c_str()<<endl;
    if(msg != NULL)
    {
        cout<<"receiver post"<<msg->content.c_str()<<endl;
        TUniNetMsg* unimsg = new TUniNetMsg();
        THttpMsgReq *msgbody = new THttpMsgReq;
        if(unimsg)
        {
            unimsg->msgBody = msgbody;
            unimsg->setMsgBody(); 

            msgbody->hMsg = msg;
            unimsg->tAddr.logAddr = 101;
            cout<<"unimsg "<<msgbody->hMsg->content.c_str()<<endl;
            sendMsg(unimsg);
        }
         
        
    }
}
*/
void Sender::WStringToString(const wstring &wstr,string &str)
{
        string strtem="";
        copy(wstr.begin(), wstr.end(), strtem.begin());
        const char *s=strtem.c_str();
        char temp[600]={'\0'};
        for(int i=0;i<wstr.length();i++)
        {
        temp[i]=*s;
        s++;
        }
        str=temp;
        memset(temp,0,sizeof(char));

}
void Sender::onTimeOut(TTimerKey timerKey, void *para)
{
    if(timerKey == mTimer)
    {
        mTimer = setTimer(UPDATETIMER);
    }        
}
