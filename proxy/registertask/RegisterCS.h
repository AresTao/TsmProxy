/*************************************************************************


 * FileName：       RegisterCS.h
 * System：         CallServer v1.0 
 * SubSystem：      注册
 * Author：         ztwu
 * Date：           2013.09.15
 * Version：        1.0
 * Description：
   	注册功能的数据库交互，主要包括注册、注销、刷新等。
	 
 *
 * Last Modified:
     
**************************************************************************/
#ifndef _REGISTER_CCS
#define _REGISTER_CCS

#include <stdio.h>
#include <strings.h>
#include <time.h>
#include "cmd5.h"
#include "db.h"
#include "comtypedef_vchar.h"
#include "resip/stack/MethodTypes.hxx"
#include "msghelper_sip.h"
//#include "RegisterTask.h"

#define SQL_STATEMENT_LEN 512
using namespace std;
enum REG {NOTEXIST=0, NOTREG,NEEDAUTH, REGSUCC, REGFAIL, REGQUERY, AUTHSUCC, AUTHFAIL,OVERLOAD, UNREGSUCC, UNREGFAIL};
enum REGACTION { QUERYREG=0, ADDNEWREG, REFREASHREG, DELREG};

struct SUSER
{
	CVarChar32 	UserNo;
	CVarChar32 	Password;
	INT		Authorization;
};

struct SHopAddr
{
	CVarChar32 host;
	UINT port;
};

struct SREGUSER
{
	CVarChar32	UserNo;
	CVarChar32	IpAddr;
	INT 		port;
	INT		UnRegisterTime;
};

struct SSIPContact
{
	BOOL mAllContacts;
	CVarChar32 host;
	UINT port;
	int expires;
        CStr nonce;
        int sourceid;
        int transport;
};


struct SREGISTER
{
	REG result;
	SSIPContact	contact;
};

class CSIPRegDialogInfo
{
    public:
        CStr username;
        CStr password;
        int nc;
        CStr nonce;
        CStr ipAddr;
        int regtime;
        int port;
        int nonceTimer;
        bool isAuth;
        int transport;
};

_CLASSDEF(CRegCallService);
class CRegCallService
{
public:
	CRegCallService();
        
	SREGISTER CS_DIRECTREGISTER(const CHAR *UserNo, const SSIPContact *Contact, const CSIPCredentials *Credentials, CDB* db, bool &isAuthorization,  CSIPRegDialogInfo & regsipdialog);
	SREGISTER CS_REGISTER(const CHAR *UserNo, const SSIPContact *Contact, const CSIPCredentials *Credentials, CDB* db, bool &isAuthorization, CSIPRegDialogInfo & regsipdialog);
//	INT CS_3ptyREGISTER(const CHAR *UserNo, S3ptyREGISTER& reglist, CDB* db);
        BOOL cs_UpdateDialogToDB(CSIPRegDialogInfo &regsipdialog, CDB *db);

	void setMaxRegUserNum(int num) {MaxRegisterUserNum=num;};

private:
	BOOL cs_QueryUser(const CHAR *UserNo, SUSER &UserItem, CDB *db);
	//新用户登录
	REG cs_RegUser(const CHAR *UserNo,const SSIPContact *Contact, CDB *db);
	//用户注销
	REG cs_UnRegUser(const CHAR * UserNo, const SSIPContact * Contact, CDB* db);
	//用户刷新
	REG cs_RefreashUser(const CHAR * UserNo, const SSIPContact * Contact, CDB* db);
	//检查用户状态
	BOOL cs_CheckActiveUser(const CHAR * UserNo, CDB * db);
	BOOL cs_ActiveUser(const CHAR *UserNo, SREGUSER &UserItem, CDB* db);
        bool cs_CheckNonce(const CHAR* UserNo, const CHAR* nonce, CDB* db );


	BOOL cs_Auth(/*CHAR cCallerNo[MAX_E164],*/const CHAR *cUserName,const CHAR *cPassword,/*CSIPChallenge &sipChallenge,*/const CSIPCredentials *sipCredentials);
	void DigestCalcHA1(
		const CHAR* cPassword,
		const CSIPCredentials *sipCredentials,
		UCHAR* tmpInputA1,
		UCHAR* ucMd5DigestA1,
		UCHAR* ucInputDigestA1,
		UCHAR* cCodeA1,
		CMD5 &tmpCmd5);
	void DigestCalcHA2(
		const CSIPCredentials *sipCredentials,
		UCHAR* tmpInputA2,
		UCHAR* ucMd5DigestA2,
		UCHAR* ucInputDigestA2,
		CMD5 &tmpCmd5);
	void CvtHex(const UCHAR *Bin, UCHAR *Hex);
	CHAR* getCurrentTime();

    	CHAR m_cSQLStatement[SQL_STATEMENT_LEN];
	void cleanSQLStatement() { memset(m_cSQLStatement, 0, SQL_STATEMENT_LEN); };

	TSelectResult *selectResult;
	TRow   *row;
	unsigned int MaxRegisterUserNum;

};

#endif
