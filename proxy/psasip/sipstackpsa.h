/****************************************************************
 * Copyright (c)2011, by BUPT
 * All rights reserved.
 *      The copyright notice above does not evidence any
 *      actual or intended publication of such source code
 * FileName:     abstractpsa.h $
 * System:       xframe
 * SubSystem:    common
 * Author:       Li Jinglin
 * Date��        2010.4.4
 * Description:
		psa(Protocol Stack Adapter)Э��ջ�Ļ��࣬���е�PSA�����������Ը��ࡣ
 *
 * Last Modified:

**********************************************************************/

#ifndef __SIPSTACKPSA_H
#define __SIPSTACKPSA_H

#include "abstracttask.h"
#include "cmd5.h"

#include "sipdialogmng.h"
#include "sipoverflow.h"

#include "msgconstdef_sip.h"
#include "msgdef_sip.h"
#include "msghelper_sip.h"

#include "resip/stack/TransactionUser.hxx"
#include "resip/stack/SipStack.hxx"
#include "resip/stack/StackThread.hxx"
#include "resip/stack/TransportThread.hxx"

#include <vector>

#define __MAX_BINDS_ADDR 3
const int TIMER_PSASIP_INTERNAL_HEARTBEAT=1;
const int TIMER_PSASIP_RECYCLE_DIALOG=2;

_CLASSDEF(TBinds)
class TBinds
{
public:
	resip::Data		mHost;
	resip::TransportType	mTransportType;
	int			mPort;
	resip::IpVersion	mIPVersion;
	resip::StunSetting	mStun;
	
public:
	TBinds():mHost("0.0.0.0"),mTransportType(resip::UDP),mPort(5060),mIPVersion(resip::V4),mStun(resip::StunDisabled) {}
	resip::Data&		sentHost() {return mHost;}
	resip::TransportType&	transport() {return  mTransportType;}
	int&			sentPort()  {return mPort;}
	resip::IpVersion&	protocolVersion() {return mIPVersion;}
	resip::StunSetting&	stun() {return mStun;}
};

_CLASSDEF(SIPSTACKPSA);
class SipStackPsa : public TAbstractTask, public resip::TransactionUser
{
   public:
      SipStackPsa();
      virtual ~SipStackPsa();
      const resip::Data& name() const; 

	  //����TAbstractTask �ķ���
	  virtual TAbstractTask* clone() { return new SipStackPsa(); }
	  virtual BOOL onInit(TiXmlElement*	extend);
	  virtual void procMsg(std::auto_ptr<TUniNetMsg> msg);
	  virtual void onTimeOut(TTimerKey timerKey, void* para);

	  virtual BOOL reloadTaskEnv(CStr& cmd, TiXmlElement* & extend);
      
	  //����TransactionUser �ķ�������Message ��Ϣ����ΪUninetMsg����ӵ��ڲ������� 
      virtual void post(resip::Message* msg);
	virtual void postToTransactionUser(resip::Message* msg, resip::TimeLimitFifo<resip::Message>::DepthUsage usage);      

	  //���෽�������͵�kernal
	  //void	sendMsg(TUniNetMsg* msg);
	  //void	sendMsg(std::auto_ptr<TUniNetMsg> msg);
 
	  void sendSipMessage(resip::SipMessage* sipmsg);
	  BOOL checkSipMessage(resip::SipMessage* sipmsg);
	  
	  

	  void closeResources();
	  void printState(CStr* cStr);

	  resip::Uri&  	getLocalUri(){return mLocalUri;};
   	  resip::Via&  	getLocalVia(){return mLocalVia;};
	  resip::Data& 	getLocalRealm(){return mLocalRealm;};
	  bool	     	getIsLooseRoute() {return mIsLooseRoute;};
   private:
	  resip::SipStack *mStack;

   protected:
	//move to msghelper_sip
	 // CStr computeDialogId(resip::Uri& from, resip::Uri& to, const char* callid);
	 // void addMsgBody(PTUniNetMsg uniMsg, resip::SipMessage* msg);
	 // void addCtrlMsgHdr(PTUniNetMsg uniMsg, resip::SipMessage* msg);


	  
   protected:

	  INT				mBindsCount;				//�󶨵�ַ����
	  TBinds		mBinds[__MAX_BINDS_ADDR];	//�����Ͽ������ö���󶨵�ַ

	  resip::Uri 		mLocalUri;		
	  resip::Via		mLocalVia;	
	  resip::Data 		mLocalRealm;
	  bool				mIsLooseRoute;

	  int				mRegRefreshInterval;	//ע������ˢ��ʱ��
	  int				mCallRefreshInterval;	//��������ˢ��ʱ��
	  int				mTargetTask;
	  int				mRegTask;

	  int 			mRecycleTimerkey;
	  //std::vector<StackThread *> stackThreads;
	  //std::vector<TransportThread*> transportThreads;
	  //std::vector<TransportThread*> recvTransportThreads;

	  resip::StackThread * 			stackThread;

	  CSIPPsaDialog					mDialog;		// Dialog ���� ����
	  time_t						mCurrenttime;	//ÿ�δ�����Ϣ��ʱ��������ʱ�䣬ʡ�ô���Ի���ʱ���θ���

	  //TAbstractTask�ڲ�����Ϣ���У�Fifo<TMsg>* 	mTaskRxFifo;
	  
};


#endif

