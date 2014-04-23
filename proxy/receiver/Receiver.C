#include "Receiver.h"
#include "msgdef_http.h"
#include "json.h"
#include "jsonvalue.h"
#include<string.h>
#include<cstdlib>

Receiver::Receiver() {
  
    //mFifo = new Fifo<CHttpMessage>(); 
    mServer = NULL;
    mTimer = setTimer(UPDATE);
}

Receiver::~Receiver()
{
    if(mServer) delete mServer;
}

BOOL Receiver::onInit(TiXmlElement* extend)
{
    TiXmlHandle handle(extend);
    TiXmlElement* psa = NULL;
    psa = handle.FirstChild("psa").Element();
    if(psa)
    {
        
        TiXmlHandle psahandle(psa);
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
         }
    }else
    {
         UniERROR("httppsa etc error");
    }
    return true;

}

void Receiver::procMsg(std::auto_ptr<TUniNetMsg> msg)
{
    TUniNetMsg * unimsg = msg.get();
    if(!unimsg) return;
    if(!unimsg->hasMsgBody()) return;
    cout<<"test procMsg"<<endl;
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
						unimsg->tAddr.logAddr = mRegID;
					}
					else if(msgType.compare("SUBSCRIBE")== 0)
					{
						unimsg->tAddr.logAddr = mSubID;
					}
					else if(msgType.compare("PUBLISH")== 0)
					{
						unimsg->tAddr.logAddr = mPubID;
					}
					else
					{
						unimsg->tAddr.logAddr = mNotID;
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
                int linkid = hmsg->linkId;
                const char *data = hmsg->content.data(); 
                CStr respcontent(data);
                cout<<"resp!!!!!"<<respcontent.c_str()<<endl;
                mServer->send(linkid, respcontent);
        }
    }
}
/*
bool Receiver::add(CHttpMessage *msg)
{
    mFifo->add(msg);
    return true;
}*/

void Receiver::post()
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

void Receiver::onTimeOut(TTimerKey timerKey, void *para)
{
    if(timerKey == mTimer)
    {
        mTimer = setTimer(UPDATE);
    }        
}
wstring Receiver::StringToWString(const string &str)
{
        wstring wstr(str.length(),L' ');
        copy(str.begin(), str.end(), wstr.begin());
        return wstr;

}
void  Receiver::WStringToString(const wstring &wstr,string &str)
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
