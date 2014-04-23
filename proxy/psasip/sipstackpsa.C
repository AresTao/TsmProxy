#include "sipstackpsa.h"

#include "info.h"
#include "resip/stack/Helper.hxx"
#include "rutil/Logger.hxx"

#include <memory>
#include <string>

using namespace resip;

#define RESIPROCATE_SUBSYSTEM resip::Subsystem::DUM

SipStackPsa::SipStackPsa() : TAbstractTask() {
	mStack = NULL;
	mIsLooseRoute = true;
	mBindsCount=0;

	stackThread=NULL;

	mRegRefreshInterval=600;
	mCallRefreshInterval=60;
	
}

SipStackPsa::~SipStackPsa() {

	if(stackThread) delete stackThread;

	if (mStack != NULL)
	{
		delete mStack;
		mStack = NULL;
	}

//	for(int i=0;i<stackThreads.size();i++)
//	{
//       delete stackThreads[i];
//        stackThreads[i] = NULL;
//	}
//    for(int i=0;i<transportThreads.size();i++)
//	{
//        delete transportThreads[i];
//        transportThreads[i] = NULL;
//	}
//    for(int i=0;i<recvTransportThreads.size();i++)
//	{
//       delete recvTransportThreads[i];
//        recvTransportThreads[i] = NULL;
//	}
}

const resip::Data& SipStackPsa::name() const
{
	static resip::Data transactionUserName("SipPSA"); 
	return transactionUserName;
}


BOOL SipStackPsa::onInit(TiXmlElement*	extend) 
{
	int  id=0;
	CStr tmp1,tmp2;
	TiXmlHandle handle(extend);
	TiXmlElement*	psa=NULL;
	psa=handle.FirstChild("psa").Element();
	if(psa)
	{
		TiXmlHandle psahandle(psa);
		TiXmlElement*	binds=NULL;
		binds=psahandle.FirstChild("binds").Element();
		if(binds)
		{
			TiXmlHandle bindhandle(binds);
			TiXmlElement*	bind=NULL;
			bind=bindhandle.FirstChild("bind").Element();
			while(bind)
			{
				if(!bind->Attribute("bindID", &id)) id=1;
				if(id>0 && id<__MAX_BINDS_ADDR)
				{
					mBinds[id].sentHost()=bind->Attribute("localIP");
					if(!bind->Attribute("localPort", &mBinds[id].sentPort())) mBinds[id].sentPort()=5060;
					tmp1=bind->Attribute("transportType");
					tmp2=bind->Attribute("networkType");
					if(tmp1=="UDP") mBinds[id].transport()=resip::UDP;
					else if(tmp1=="TCP") mBinds[id].transport()=resip::TCP;
					else if(tmp1=="TLS") mBinds[id].transport()=resip::TLS;
					else mBinds[id].transport()=resip::UDP;
					if(tmp2=="IPv4") mBinds[id].protocolVersion()=resip::V4;
					else if(tmp2=="IPv6") mBinds[id].protocolVersion()=resip::V6;
					else mBinds[id].protocolVersion()=resip::V4;		
					mBindsCount++;
				}
				bind=bind->NextSiblingElement();
			}

			TiXmlElement*	target=NULL;
			target=psahandle.FirstChild("target").Element();
			if(target)
			{
				if(!target->Attribute("regTask", &mRegTask)) mRegTask=1001;
				if(!target->Attribute("procTask", &mTargetTask)) mTargetTask=1002;
			}

			TiXmlElement*	sip=NULL;
			sip=psahandle.FirstChild("sip").Element();
			if(sip)
			{
				if(!sip->Attribute("rgRefreshInterval", &mRegRefreshInterval)) mRegRefreshInterval=600;
				if(!sip->Attribute("callRefreshInterval", &mCallRefreshInterval)) mCallRefreshInterval=600;
			}

			
			mDialog.setLivingTime(mRegRefreshInterval>mCallRefreshInterval?mRegRefreshInterval:mCallRefreshInterval);	//默认生存时间修改为注册刷新时间和呼叫刷新时间最大者

		}
	}
	UniINFO("Init psasip OK!");
	UniINFO("  regTask=%d, procTask=%d",mRegTask,mTargetTask);
	UniINFO("  rgRefreshInterval=%d, callRefreshInterval=%d", mRegRefreshInterval, mCallRefreshInterval);
	for(int i=1;i<=mBindsCount;i++)
	{
		UniINFO("  bind %d/%d:", i, mBindsCount);
		UniINFO("    localIP=%s, localPort=%d, transportType=%d, networkType=%d", mBinds[i].sentHost().c_str(), mBinds[i].sentPort(), mBinds[i].transport(), mBinds[i].protocolVersion());
	}

	
	mStack= new resip::SipStack;
	if(mStack==NULL)
	{
		UniERROR("Error in create sip stack!!!");
		return FALSE;
	}

	for(int i=1;i<=mBindsCount;i++)
	{
		//mStack->addTransport(resip::UDP, mLocalUri.port(),resip::V4);
		mStack->addTransport(mBinds[i].transport(), mBinds[i].sentPort(),mBinds[i].protocolVersion(), resip::StunDisabled, mBinds[i].sentHost());
	}

	mLocalRealm="";

	mLocalUri.host()=resip::Data(mBinds[1].sentHost());	//默认先用bind1
	mLocalUri.port()=mBinds[1].sentPort();
	mLocalVia.sentHost()=resip::Data(mBinds[1].sentHost());
	mLocalVia.sentPort()=mBinds[1].sentPort();
	
	mIsLooseRoute=TRUE;

    UniINFO("PsaId:%d",getTaskId());
	UniINFO("LocalUri:%s:%d",mLocalUri.host().c_str(),mLocalUri.port());
	UniINFO("LocalVia:%s:%d",mLocalVia.sentHost().c_str(),mLocalVia.sentPort());
	UniINFO("LocalRealm:%s",mLocalRealm.c_str());
	UniINFO("Local is Loose Router:%d",mIsLooseRoute);
	UniINFO("TargetTaskId:%d",mTargetTask);
	UniINFO("RegTaskId:%d",mRegTask);

	mStack->registerTransactionUser((*this));
	stackThread = new resip::StackThread(*mStack);
	if(stackThread==NULL)
	{
		UniERROR("Error in create sip stack thread!!!");
		return FALSE;
	}
	
	//resip::Log::initialize(resip::Log::File, resip::Log::Debug, Data("SipStack"), "log/CallServer_SipStack.log");
	resip::Log::initialize(resip::Log::Type(logType), resip::Log::level(logLevel), Data("SipStack"), "log/CallServer_SipStack.log");
	mStack->run();		//start stack transport thread, controller thread, dns thread 
	stackThread->run();	//start stack thread
	//stackThread->detach();

	mRecycleTimerkey=setTimer(TIMER_PSASIP_RECYCLE_DIALOG);	
//	for(int i=0;i<localEnvExt->stackThreadNum;i++)
//	{
//	    // std::cout << "stackthred ok" << std::endl;
//       StackThread* stackThread = new StackThread(*mStack);
//        stackThreads.push_back(stackThread);
//        stackThread->run();
//	}
//	for(int j=0; j<localEnvExt->transportThreadNum;j++)
//	{
//	    //std::cout << "transportThreadNum ok" << std::endl;
//       TransportThread* sendTransportThread = new TransportThread(mStack->getTuSelector(),1,"tp Send Thread");
//        transportThreads.push_back(sendTransportThread);
//        sendTransportThread->run();
//	}
//    for(int k=0;k<localEnvExt->recvTransportThreadNum;k++)
//    {
//        //std::cout << "recvTransportThreadNum ok" << std::endl;
//        TransportThread* recvTransportThread = new TransportThread(mStack->getTuSelector(),2,"tp recv Thread");
//        recvTransportThreads.push_back(recvTransportThread);
//        recvTransportThread->run();
//    }
}

//BOOL SipStackPsa::procSendMsg(PTUniNetMsg msg) {
//	if (msg->hasMsgBody()) {
//		PTSipMsgBody pSipMsgBody = (PTSipMsgBody)msg->msgBody;
//		UniINFO("Send message from:%s:%d",pSipMsgBody->msg.sipMsg->header(h_From).uri().host().c_str(),pSipMsgBody->msg.sipMsg->header(h_From).uri().port());
//		UniINFO("Send message to:%s:%d",pSipMsgBody->msg.sipMsg->header(h_To).uri().host().c_str(),pSipMsgBody->msg.sipMsg->header(h_To).uri().port());
//
//		if (NULL != pSipMsgBody) {
//		    mStack->send(*(pSipMsgBody->msg.sipMsg), this);
//		}
//		delete pSipMsgBody;
//	}
//	return TRUE;
//}

BOOL SipStackPsa::reloadTaskEnv(CStr& cmd, TiXmlElement* & extend)
{
	CStr scmd[5];
	cmd.split(scmd,5);
	if(scmd[0]=="set")
	{
		if(scmd[1]=="logLevel")
		{
			if(scmd[2]=="DEBUG")
			{
				resip::Log::initialize(resip::Log::Type(logType), resip::Log::Debug, Data("SipStack"), "log/CallServer_SipStack.log");
			}
			else if(scmd[2]=="default")
			{
				resip::Log::initialize(resip::Log::Type(logType), resip::Log::level(logLevel), Data("SipStack"), "log/CallServer_SipStack.log");
			}
		}
	}
	else if(scmd[0]=="reload")
	{
		if(scmd[1]=="all" || scmd[1]=="task")
		{

			TiXmlHandle handle(extend);
			TiXmlElement*	psa=NULL;
			psa=handle.FirstChild("psa").Element();
			if(psa)
			{
				TiXmlHandle psahandle(psa);
				TiXmlElement*	sip=NULL;
				sip=psahandle.FirstChild("sip").Element();
				if(sip)
				{
					if(!sip->Attribute("rgRefreshInterval", &mRegRefreshInterval)) mRegRefreshInterval=600;
					if(!sip->Attribute("callRefreshInterval", &mCallRefreshInterval)) mCallRefreshInterval=600;
				}
			
				mDialog.setLivingTime(mRegRefreshInterval>mCallRefreshInterval?mRegRefreshInterval:mCallRefreshInterval);	//默认生存时间修改为注册刷新时间和呼叫刷新时间最大者
			}
		}
	}

}


void SipStackPsa::procMsg(std::auto_ptr<TUniNetMsg> msg)
{
//Psa 线程循环
	UINT port=0;

	mCurrenttime=time(0);	//刷新当前时间

	UniDEBUG("get a unimsg!...");

	TUniNetMsg* unimsg=msg.get();
	if(!unimsg) return;
	if(!unimsg->hasCtrlMsgHdr()) return;
	if(!unimsg->hasMsgBody()) return; 

	TSipMsg* sipbody=NULL;
	sipbody=dynamic_cast<TSipMsg*>(unimsg->msgBody);
	if(!sipbody) return;
	TSipCtrlMsg* sipctrl=NULL;
	sipctrl=dynamic_cast<TSipCtrlMsg*>(unimsg->ctrlMsgHdr);
	if(!sipctrl) return;

	resip::SipMessage* sipmsg=sipbody->sipMsg;
	if(sipmsg==NULL) return;
	

	//LogDEBUG(<<"Received a sip message: " << *sipmsg );
	
	if(unimsg->sender.taskID==0)
	{
		UniDEBUG("  Received a msg from wire....");
	//从网络上收到消息
	
		//先进行消息检查
		if(!checkSipMessage(sipmsg)) return;
			
		//判断消息是否是对话内消息
		CSIPPsaDialogInfo dialoginfo;
		if(mDialog.QueryDialogTab(sipctrl->dialogid, dialoginfo))
		{
			//Dialog内消息，直接转发
			//mDialog.UpdateDialogExpire(dialoginfo.mDialogID_F, mCurrenttime);	//用ID_F 更新，少一次检索时间
			unimsg->tAddr=dialoginfo.mAddr;
			unimsg->dialogType=DIALOG_CONTINUE;
			if(sipctrl->dialogid != dialoginfo.mDialogID_F ) sipctrl->dialogid=dialoginfo.mDialogID_F;

			mDialog.UpdateDialogExpire(dialoginfo.mDialogID_F, mCurrenttime); 

			UniDEBUG("  Received in dialog msg: did=%s; send to task:%d,%d,%d", sipctrl->dialogid.c_str(), unimsg->tAddr.logAddr, unimsg->tAddr.phyAddr, unimsg->tAddr.taskInstID);
			sendMsg(msg);
			return;
		}
		else
		{
			//Dialog 外消息，判断是否应该创建Dialog

			if(unimsg->msgName==SIP_MESSAGE_RSP
				|| sipctrl->method == SIP_ACK 
				|| sipctrl->method == SIP_INFO 
				|| sipctrl->method == SIP_BYE 
				|| sipctrl->method == SIP_CANCEL
				|| sipctrl->method == SIP_PRACK
				|| sipctrl->method == SIP_UPDATE
				|| sipctrl->method == SIP_UNKNOWN)
			{
				//状态码大于0，是响应，不可能对话外，按照直接将消息转发走
				//是请求，但不可能是对话外的请求，则按照宽松路由规则转发

				sipbody->release();	//释放TUninetMsg 的sip 指针
				
				UniDEBUG("  Received unexpired msg: did=%s", sipctrl->dialogid.c_str());
				sendSipMessage(sipmsg);
				return;	
			}

			//是创建对话请求

			CStr did_B;
			did_B=SIPMsgHelper::computeDialogId(sipmsg->header(h_To).uri().user().c_str(), sipmsg->header(h_From).uri().user().c_str(), sipmsg->header(h_CallID).value().c_str());
			mDialog.InsertDialogTab(sipctrl->dialogid, did_B);

			if(sipctrl->method == SIP_REGISTER)
				unimsg->tAddr.logAddr=mRegTask;				//注册请求发送给注册处理任务
			else
				unimsg->tAddr.logAddr=mTargetTask;
			unimsg->dialogType=DIALOG_BEGIN;

			UniDEBUG("  Received out dialog msg: did=%s, send to task:%d,%d,%d", sipctrl->dialogid.c_str(),unimsg->tAddr.logAddr, unimsg->tAddr.phyAddr, unimsg->tAddr.taskInstID);
			sendMsg(msg);
			return;
		}
	}
	else
	{
		UniDEBUG("  Rceived a msg from task...");
		//从内部任务收到消息
		CStr  dialogid;
		CSIPPsaDialogInfo dialoginfo;
		if(sipctrl->dialogid.empty())
			dialogid=SIPMsgHelper::computeDialogId(sipmsg->header(h_From).uri().user().c_str(), sipmsg->header(h_To).uri().user().c_str(), sipmsg->header(h_CallID).value().c_str());
		else
			dialogid=sipctrl->dialogid;
		
		if(mDialog.QueryDialogTab(dialogid, dialoginfo))
		{
			//Dialog内消息

			//如果对话信息中的logAddr 和instID 没有初始化，或与当前消息的源地址不同，则根据消息中的信息更新
			if(!(dialoginfo.mAddr==unimsg->oAddr))
			{
				//dialoginfo.mAddr=unimsg->oAddr;
				mDialog.UpdateDialogAddr(dialogid, unimsg->oAddr);
				
			}
			// 应用下发消息就不更新了
			//mDialog.UpdateDialogExpire(dialoginfo.mDialogID_F, mCurrenttime);	//用ID_F 更新，少一次检索时间
			UniDEBUG("  Received in dialog msg: did=%s, send to stack...", sipctrl->dialogid.c_str());
		}
		else
		{
			//Dialog 外消息，判断是否应该创建Dialog
			if(unimsg->msgName!=SIP_MESSAGE_RSP 
				&& sipctrl->method != SIP_RESPONSE
				&& sipctrl->method != SIP_ACK 
				&& sipctrl->method != SIP_INFO 
				&& sipctrl->method != SIP_BYE 
				&& sipctrl->method != SIP_CANCEL
				&& sipctrl->method != SIP_NOTIFY
				&& sipctrl->method != SIP_PRACK
				&& sipctrl->method != SIP_UPDATE
				&& sipctrl->method != SIP_UNKNOWN)
			{
				CStr did_B;
				did_B=SIPMsgHelper::computeDialogId(sipmsg->header(h_To).uri().user().c_str(), sipmsg->header(h_From).uri().user().c_str(), sipmsg->header(h_CallID).value().c_str());
				mDialog.InsertDialogTab(sipctrl->dialogid, did_B, unimsg->oAddr);
			}

			//是响应，或是请求，但不是对话外的请求，直接下发
			//......
			UniDEBUG("  Received out dialog msg: did=%s, send to stack...", sipctrl->dialogid.c_str());
		}
		
		sipbody->release();	//释放对sip消息指针的控制权
		sendSipMessage(sipmsg);
	
		return;
	}
}


BOOL SipStackPsa::checkSipMessage(resip::SipMessage* sipmsg)
{
	UINT port;
	if (sipmsg->isRequest())
	{
		if (!sipmsg->header(h_From).exists(p_tag))
		{
		//Must Exist From Tag
			UniERROR("From tag missing, make 400 response!!");
			std::auto_ptr<SipMessage> response(Helper::makeResponse(*sipmsg, 400));
			mStack->send(response, this);
			return FALSE;
		}
		// The TU selector already checks the URI scheme for us (Sect 16.3, Step 2)

		// check the MaxForwards isn't too low
		if (!sipmsg->exists(h_MaxForwards) || sipmsg->header(h_MaxForwards).value() > 70)
		{			// .bwc. Add Max-Forwards header if not found.
			sipmsg->header(h_MaxForwards).value()=70;
		}			// .bwc. Unacceptable values for Max-Forwards			
		else if(sipmsg->header(h_MaxForwards).value() <=1)
		{
			UniERROR("MaxForward=0, make 483 response!!");
			std::auto_ptr<SipMessage> response(Helper::makeResponse(*sipmsg, 483));	
			mStack->send(response, this);
			return FALSE;
		}

		//Check the Self-Loop			
		port=sipmsg->header(h_Vias).front().sentPort();
		if(port==0) port=5060;
		if(sipmsg->header(h_Vias).front().sentHost()==mLocalUri.host() && port==mLocalUri.port())
		{
			UniERROR("Critical ERROR detected!!!!!!!!Self-Loop happened!!! top via=self, make 482 response!!"); 
			std::auto_ptr<SipMessage> response(Helper::makeResponse(*sipmsg, 482));
			mStack->send(response, this);				
			return FALSE; 			
		}			

		//Check the Top Route
		if(sipmsg->exists(h_Routes)&&(!sipmsg->header(h_Routes).empty()))
		{
			port=sipmsg->header(h_Routes).front().uri().port();
			if(port==0) port=5060;
			if(sipmsg->header(h_Routes).front().uri().host() != mLocalUri.host() || port != mLocalUri.port())
			{
				//if top route != LocalHost and RequestURI != LocalHost					
				// prev host is a strick Route host					
				port=sipmsg->header(h_RequestLine).uri().port();					
				if(port==0) port=5060;					
				if( sipmsg->header(h_RequestLine).uri().host()!= mLocalUri.host() || port!=mLocalUri.port() )
				{
					UniERROR("Critical ERROR detected!!!!!!!! Need Loose Route!!! request uri=self, make 403 response!!");
					std::auto_ptr<SipMessage> response(Helper::makeResponse(*sipmsg, 403));	
					mStack->send(response, this);
					return FALSE; 
				}
			}
		}

		if(sipmsg->header(h_RequestLine).getMethod()==resip::CANCEL)
		{
			UniDEBUG("Received CANCEL, make 200 response!!");
		   std::auto_ptr<SipMessage> response(Helper::makeResponse(*sipmsg, 200));	
		   mStack->send(response, this);
		}
	}
	else
	{
		port=sipmsg->header(h_Vias).front().sentPort();	
		if (port==0) port=5060;
			if (sipmsg->header(h_Vias).front().sentHost() != mLocalUri.host() || port != mLocalUri.port())
			{
				UniERROR("Critical ERROR detected!!!!!!!! Need Loose Route!!! top via is not me!!");
				//UniERROR(sipmsg->brief());
				//UniERROR("Top Via: "<<sipmsg->header(h_Vias).front()<<" is NOT my Host!\n");
				return FALSE;
			}
	}

	return TRUE;
}


void SipStackPsa::sendSipMessage(resip::SipMessage* sipmsg)
{
	UINT port;
	//下发消息之前进行检查
	if(sipmsg->isResponse())
	{
	//如果是响应，则检查via
		if(sipmsg->header(h_Vias).empty()) return;
		port=sipmsg->header(h_Vias).front().sentPort();	
		if(port==0) port=5060;
		if(sipmsg->header(h_Vias).front().sentHost()==mLocalVia.sentHost() && port==mLocalVia.sentPort())
		{
			sipmsg->header(h_Vias).pop_front();
			if(sipmsg->header(h_Vias).empty()) return;
		}
		
		std::auto_ptr<SipMessage> msg(sipmsg);
		mStack->send(msg, this);
		return;	
	}

	//如果是请求，则要检查route、via 
	//Check and Remove Top Route	
	if(sipmsg->exists(h_Routes)&&(!sipmsg->header(h_Routes).empty())) 
	{
		//remove Top Route if it == LocalHost		
		port=sipmsg->header(h_Routes).front().uri().port();
		if(port==0) port=5060;
		if(sipmsg->header(h_Routes).front().uri().host() == mLocalUri.host() && port == mLocalUri.port())
		{
			sipmsg->header(h_Routes).pop_front();
		}

		//check if next hop is strick Route 	
		if(!sipmsg->header(h_Routes).empty())
		{
			if(!sipmsg->header(h_Routes).front().uri().exists(p_lr))
			{	
				//no lr, is Strick Route;	
				//value the URI of RequestURI with Top Route URI;
				sipmsg->header(h_RequestLine).uri().host()=sipmsg->header(h_Routes).front().uri().host();
				sipmsg->header(h_RequestLine).uri().port()=sipmsg->header(h_Routes).front().uri().port();
				sipmsg->header(h_Routes).pop_front();	
			}
		}
			
	}

	//Check Force Target	
	if (!sipmsg->hasForceTarget())	
	{		
		if(sipmsg->header(h_Routes).empty())
		{
			sipmsg->setForceTarget(sipmsg->header(h_RequestLine).uri());
		}
		else
		{
			sipmsg->setForceTarget(sipmsg->header(h_Routes).front().uri());		
			//remove routes in CANCEL, after setForceTarget	
			if(sipmsg->header(h_RequestLine).getMethod()==resip::CANCEL)	
			{		 
				sipmsg->remove(h_Routes);	
			}
		}	
	}

	//push local via	
	//remove old vias in CANCEL 
	if(!sipmsg->header(h_Vias).empty())	
	{	
		//do not remove CANCEL vias, the top via tid must add by INVITE and CANCEL proc themself!!!!
		//psa do not know the relationship of INVITEs and CANCELs !!!!
		//if(sipmsg->header(h_RequestLine).getMethod()==resip::CANCEL)		
		//	sipmsg->remove(h_Vias); 		
		//else
		//{
			port=sipmsg->header(h_Vias).front().sentPort();		
			if( port==0 ) port = 5060;		
			if( sipmsg->header(h_Vias).front().sentHost() != mLocalUri.host() || port != mLocalUri.port()) 	
			{
				resip::Via via;
				via.sentHost()=mLocalVia.sentHost();
				via.sentPort()=mLocalVia.sentPort();
				sipmsg->header(h_Vias).push_front(via);
			}
		//}
	}

	if(sipmsg->header(h_MaxForwards).value()>0) sipmsg->header(h_MaxForwards).value()--;

	//LogDEBUG(<<"Send a sip message: " << *sipmsg );

	std::auto_ptr<SipMessage> msg(sipmsg);
	mStack->send(msg, this);
	return;

}

void SipStackPsa::closeResources() {
	UniINFO("close mStack.");
    mStack->shutdown();
    UniINFO("ok!");
    UniINFO("close stackThread.");
//	stackThread->detach();
	UniINFO("ok!");
	UniINFO("close transportThread.");
	//transportThread->detach();
	UniINFO("ok!");
	UniINFO("close recvTransportThread.");
	//recvTransportThread->detach();
	UniINFO("ok!");
} 

void SipStackPsa::printState(CStr* cStr) {
	
	if(cStr==NULL) return;

	*cStr<<" TaskInfo:\r\n";
	*cStr<<"   Local addr=";
	*cStr<<mLocalUri.host().c_str();
	*cStr<<":";
	*cStr<<mLocalUri.port();
	*cStr<<", loose router=";
	*cStr<<mIsLooseRoute;
	*cStr<<"\r\n";
	*cStr<<"  Target task: regTask=";
	*cStr<<mRegTask;
	*cStr<<", proxyTask=";
	*cStr<<mTargetTask;
	*cStr<<"\r\n";
	for(int i=1;i<=mBindsCount;i++)
	{
		*cStr<<"  Bind addr=";
		*cStr<<mBinds[i].sentHost().c_str();
		*cStr<<":";
		*cStr<<mBinds[i].sentPort();
		*cStr<<",transport=";
		*cStr<<mBinds[i].transport();
		*cStr<<",version=";
		*cStr<<mBinds[i].protocolVersion();
		*cStr<<"\r\n";
	}
}



void SipStackPsa::postToTransactionUser(resip::Message* msg, resip::TimeLimitFifo<Message>::DepthUsage usage)
{
	post(msg);
}

void SipStackPsa::post(Message* msg)
{
//Sip协议栈上报消息，封装成UniNetMsg 写到消息队列
//这个函数是SipStack 线程调用的，不能访问SipStackPsa 中的参数
	
	//cout<<" Received sip msg!"<<endl;
	SipMessage* sipmsg=dynamic_cast<SipMessage*>(msg);
	if(sipmsg)
	{
		//UniDEBUG("Received a sip message: ");
		DebugLog( << " Received sip msg:" << *sipmsg << "\n");
		TUniNetMsg* unimsg=NULL;
		TSipCtrlMsg* ctrlmsg=NULL;
		TSipMsg* msgbody=NULL;
		unimsg=new TUniNetMsg();
		if(unimsg)
		{
			unimsg->sender.taskID=0;
			unimsg->sender.taskType=0;
			unimsg->sender.instID=0;
			if(sipmsg->isRequest()) unimsg->msgName=SIP_MESSAGE_REQ;
			else unimsg->msgName=SIP_MESSAGE_RSP;
			unimsg->msgType=SIP_MESSAGE_TYPE;

			SIPMsgHelper::addCtrlMsgHdr(unimsg,sipmsg);
			SIPMsgHelper::addMsgBody(unimsg, sipmsg);

			//ostrstream st;
			//unimsg->print(st);
			//DebugLog( << " Received uni msg:" << st << "\n");
			mTaskRxFifo->add((TMsg*)(unimsg));
		}
	}
	else
	{
		//UniERROR("Received a message, but is not a sip message!");
		DebugLog( << " Received a message, but is not a sip message!");
	}
}

void SipStackPsa::onTimeOut(TTimerKey timerKey, void* para)
{
	if(mRecycleTimerkey==timerKey)
	{
		mDialog.CleanExpireDialog();
		mRecycleTimerkey=setTimer(TIMER_PSASIP_RECYCLE_DIALOG);
	}

}
