#if !defined __MSGHELPER_HTTP_H
#define __MSGHELPER_HTTP_H

#include "msgconstdef_http.h"
#include "msgdef_http.h"

/***************************CSIPCredentials 定义*************************/
class CSIPCredentials
{
  public:
	enum Algthm{MD5,MD5_SESS};
	bool valid;

  private:
    CStr m_UserName;
    CStr m_Realm;
    CStr m_Qop;
    CStr m_Nonce;
    CStr m_Opaque;
    CStr m_URI;
    CStr m_Response;
    Algthm m_ucAlgorithm;
    CStr m_CNonce;
    CStr m_NonceCount;
    resip::MethodTypes m_ucMsgType;
    CStr m_H;


  public:
     CSIPCredentials();

     const CStr& GetUserNameRef() const;
     CStr& GetUserNameRef();
     void SetUserName( const char* );

     const CStr& GetRealmRef() const;
     CStr& GetRealmRef();
     void SetRealm( const char* );

     const CStr& GetQopRef() const;
     void SetQop( const char* );

     const CStr& GetNonceRef() const;
     CStr& GetNonceRef();
     void SetNonce( const char* );

     const CStr& GetOpaqueRef() const;
     CStr& GetOpaqueRef();
     void SetOpaque( const char* );

     const CStr& GetURIRef() const;
     CStr& GetURIRef();
     void SetURI( const char* );

     const CStr& GetResponseRef() const;
     CStr& GetResponseRef();
     void SetResponse( const char* );

     Algthm GetAlgorithm() const;
     void SetAlgorithm( Algthm );

     const CStr& GetCNonceRef() const;
     CStr& GetCNonceRef();
     void SetCNonce( const char* );

     const CStr& GetNonceCountRef() const;
     CStr& GetNonceCountRef();
     void SetNonceCount( const char* );

    resip::MethodTypes GetMsgType() const;
     void SetMsgType( resip::MethodTypes );

     const CStr& GetHRef() const;
     CStr& GetHRef();
     void SetH( const char* );

     INT Size() const;

    void Print(ostrstream &st);

};


_CLASSDEF(HTTPMsgHelper);
class HTTPMsgHelper
{
public:
	static HTTPMETHODTYPE msg_Type(resip::MethodTypes type);
	static CStr computeDialogId(const char* from, const char* to, const char* callid);
	static CStr computeDialogId(resip::SipMessage* msg);
	static void addMsgBody(PTUniNetMsg uniMsg, resip::SipMessage* msg);
	static void addCtrlMsgHdr(PTUniNetMsg uniMsg, resip::SipMessage* msg);
	static resip::SipMessage*  getSipMsgBody(PTUniNetMsg unimsg);

	static BOOL buildCredential(const resip::SipMessage* msg, CSIPCredentials& Credentials);

};


#endif
