/*************************************************************************

 * Copyright (c)2005-2008, by Beijing TeleStar Telecom Technology Institue
 * All rights reserved.

 * FileName：       ccs.C
 * System：         UniNetSSCLite v1.0 
 * SubSystem：      呼叫服务
 * Author：         Zhou Xiaofei
 * Date：           2006.02.24
 * Version：        1.0
 * Description：
   	为UniNetSSCLite v1.0提供认证鉴权，号码分析，路由等功能。
	 
 *
 * Last Modified:
     2006.02.24, 完成初始版本
        By Zhou Xiaofei
**************************************************************************/
#include "RegisterCS.h"
#include "func.h"
#include <time.h>

#include <iostream>
//extern struct tm*				uni_tm;
//time_t					current_time;
using namespace resip;
using namespace std;
#define RESIPROCATE_SUBSYSTEM Subsystem::APP

void ConvStrtoByte(CHAR* ptag,const CHAR* psrc)
{
	INT j,len;
	CHAR ucaTemp[3];
	len=strlen(psrc)/2;
	ptag[0] = 0;
	ucaTemp[2] = 0;
	for(j=0; j<len; j++)   
    {
    	strncpy(ucaTemp,psrc+j*2,2);
    	ptag[j]=strtol(ucaTemp,NULL,16);
//      sprintf((CHAR*)ucaTemp, "%.2x", psrc[j]);     //<-<-<-<-<-<-<-<-<-<-
//      strcat((CHAR*)ptag, (CHAR*)ucaTemp);          //<-<-<-<-<-<-<-<-<-<-
    }
}

void ConvBytetoStr(UCHAR* ptag,const UCHAR* psrc,INT len)
{
	INT j;
	CHAR ucaTemp[3];
	ptag[0] = 0;
	ucaTemp[2] = 0;
	for(j=0; j<len; j++)
    {

      sprintf((CHAR*)ucaTemp, "%.2x", psrc[j]);     //<-<-<-<-<-<-<-<-<-<-
      strcat((CHAR*)ptag, (CHAR*)ucaTemp);          //<-<-<-<-<-<-<-<-<-<-
    }
//    ptag[j]=0;
}

CRegCallService::CRegCallService()
{
//	cleanSQLStatement();
//	sprintf(m_cSQLStatement,"SELECT maxactiveuser FROM qos;");
//	if (CDB::instance()->execSQL(m_cSQLStatement))
//	{
//		selectResult = CDB::instance()->getSelectResult();
//		if ((selectResult!=NULL)&&(selectResult->rowNum>0))
//		{
//			row = selectResult->pRows;
//			MaxRegisterUserNum = row->arrayField[0].value.intValue;
//		}
//	}
	MaxRegisterUserNum=0;

}

SREGISTER CRegCallService::CS_DIRECTREGISTER(const CHAR* UserNo, const SSIPContact *Contact, const CSIPCredentials *Credentials, CDB *db, bool &isAuthorization, CSIPRegDialogInfo &regsipdialog)
{
	SREGISTER registerres;
	REGACTION regType=QUERYREG;

        bool isMyRequest = true;

	char *usernum,*passwd;
      //  cout<<"username"<<regsipdialog.username.c_str()<<endl;
      //  cout<<"password"<<regsipdialog.password.c_str()<<endl;
        isAuthorization = regsipdialog.isAuth;
	if (!cs_Auth(usernum=(char*)regsipdialog.username.c_str(), passwd=(char*)regsipdialog.password.c_str(), Credentials))
	{
             //   cout<<"auth test"<<endl;
		registerres.result =  AUTHFAIL;
                return registerres;
        }
	if(Contact!=NULL)
	{
         		if(Contact->expires>0)
			{	//expire!=0 意味着用户登记注册
				if(strcmp(Credentials->GetNonceRef().c_str(),regsipdialog.nonce.c_str()) != 0)
                                        regType=ADDNEWREG;
                                else
                                        regType=REFREASHREG;
			}
			else
			{
				//expire=0 意味着注销，但由于没有活动记录，因此直接判断为成功
				regType=DELREG;
 				if(strcmp(Credentials->GetNonceRef().c_str(),regsipdialog.nonce.c_str()) == 0)
			        {
                                        isMyRequest = true;	        
                                }else
                                        isMyRequest = false;	        

			}

/*		if (!cs_CheckActiveUser(UserNo, db))
		{

			if(Contact->expires>0)
			{	//expire!=0 意味着用户登记注册
				if(strcmp(Credentials->GetNonceRef().c_str(),regsipdialog.nonce.c_str()) != 0)
                                        regType=ADDNEWREG;
                                else
                                        regType=REFREASHREG;
			}
			else
			{
				//expire=0 意味着注销，但由于没有活动记录，因此直接判断为成功
				regType=QUERYREG;
			}
		}
		else
		{
			if(Contact->expires>0)
			{	//expire!=0意味着刷新用户登记注册信息
				regType=REFREASHREG;
			}
			else
			{
				//expire=0 意味着注销
				regType=DELREG;
				if(strcmp(Credentials->GetNonceRef().c_str(),regsipdialog.nonce.c_str()) == 0)
			        {
                                        isMyRequest = true;	        
                                }else
                                        isMyRequest = false;	        
			}
		}*/
	}
	else
	{
		regType=QUERYREG;
	}
	switch(regType)
	{
		case ADDNEWREG:
//                        cout<<"addnewreg way"<<endl;
			registerres.result=cs_RegUser(UserNo, Contact, db);
			regsipdialog.nonce = Credentials->GetNonceRef();
                        regsipdialog.regtime = time(0);
			break;
		case REFREASHREG:
                       // cout<<"direct refresh"<<endl;
                        //regsipdialog.regtime = time(0) + Contact->expires+1;
			registerres.result =  REGSUCC;
			break;
		case DELREG:
                        
    //                    cout<<"delete way"<<endl;
                        if(isMyRequest)
                        {
                                //cout<<"direct unregister"<<endl;
			        registerres.result=cs_UnRegUser(UserNo, Contact, db);
                                
                        }else
                        {
                                registerres.result = UNREGSUCC;
                        }
			break;
		case QUERYREG:
			registerres.result =  REGQUERY;
	}
	if ((REGSUCC==registerres.result)||(REGQUERY==registerres.result)||(UNREGSUCC==registerres.result))
	{
		cleanSQLStatement();
		sprintf(m_cSQLStatement,"SELECT ipAddr,port,unregistertime,nonce FROM registeruser WHERE userno=\'%s\';", UserNo);
		if (db->execSQL(m_cSQLStatement))
		{
			selectResult = db->getSelectResult();
			if ((selectResult!=NULL)&&(selectResult->rowNum>0))
			{
				row = selectResult->pRows;
				while (row != NULL)
				{
					registerres.contact.host.SetVarCharContent(row->arrayField[0].value.stringValue ,strlen(row->arrayField[0].value.stringValue));
					registerres.contact.port = row->arrayField[1].value.intValue;
                                        //向注册dialog中插入用户信息
                                       // regsipdialog.ipAddr = registerres.contact.host.c_str();
                                       // regsipdialog.port = registerres.contact.port;
					registerres.contact.mAllContacts=false;
					if(REGQUERY==registerres.result)
						registerres.contact.expires=row->arrayField[2].value.intValue-time(0);
					else
						registerres.contact.expires = Contact->expires;
                                       // regsipdialog.nonce = row->arrayField[3].value.stringValue;
                                        registerres.contact.nonce = row->arrayField[3].value.stringValue;
					row = row->next;
				}
				//registerres.result = AUTHSUCC;	//成功的话，保留原来成功注册注销的返回类型
			}
			//else
			//{
			//	registerres.result = NOTREG;
			//}
		}
	}

	return registerres;

}

SREGISTER CRegCallService::CS_REGISTER(const CHAR *UserNo, const SSIPContact *Contact, const CSIPCredentials *Credentials, CDB *db, bool &isAuthorization, CSIPRegDialogInfo &regsipdialog)
{
	SUSER UserItem;
	SREGISTER registerres;

	//REGNEW: 新注册登录用户;
	//REFREASHREG: 已经注册登录了，刷新注册登录；
	//UNREGACTIVE: 已经注册登录了，删除登录记录；
	//UNREGNEW: 还没有注册登录，却要删除登录记录；
	REGACTION regType=QUERYREG;

        isAuthorization = false;
//检查用户是否注册，检查当前登录用户数量，清理过期用户，检查是否过载
	if (cs_QueryUser(UserNo, UserItem, db))	
	{
                /*if(cs_CheckNonce(UserNo, Credentials->GetNonceRef().c_str(), db))
                {
                        registerres.result =  AUTHFAIL;
			return registerres;
                        
                }*/
		if(Contact!=NULL)
		{
			if(Contact->expires>0)
			{	//expire!=0 意味着用户登记注册
				regType=ADDNEWREG;
			}
			else
			{
				//expire=0 意味着注销，但由于没有活动记录，因此直接判断为成功
				regType=DELREG;
			}

			//µ±Ç° ·¢Æð×¢²áÇëÇóµÄÊÇÒÑµÇ¼ÇÓÃ»§£¬ÅÐ¶ÏÊÇ·ñÒÑ¾­×¢²á
		/*	if (!cs_CheckActiveUser(UserNo, db))
			{

				if(Contact->expires>0)
				{	//expire!=0 意味着用户登记注册
					regType=ADDNEWREG;
				}
				else
				{
					//expire=0 意味着注销，但由于没有活动记录，因此直接判断为成功
					regType=QUERYREG;
				}
			}
			else
			{
				if(Contact->expires>0)
				{	//expire!=0意味着刷新用户登记注册信息
					regType=REFREASHREG;
				}
				else
				{
					//expire=0 意味着注销
					regType=DELREG;
				}
			}*/
		}
		else
		{
			regType=QUERYREG;
		}

		if (UserItem.Authorization!=0)
		{
                        isAuthorization = true;
		//如果用户需要鉴权的话，执行鉴权
			char *usernum,*passwd;
			if(Credentials==NULL)
			{
				registerres.result =  NEEDAUTH;
				return registerres;
			}
			if(!Credentials->valid)
			{
				UniDEBUG("User need Auth: User=%s",UserItem.UserNo.c_str());
				registerres.result =  NEEDAUTH;
				return registerres;
			}

			if (!cs_Auth(usernum=(char*)UserItem.UserNo.c_str(), passwd=(char*)UserItem.Password.c_str(), Credentials))
			{
                                UniDEBUG("AUTH: User Auth Faile User=%s",UserItem.UserNo.c_str());
				registerres.result =  AUTHFAIL;
				return registerres;
			}
                        regsipdialog.username = UserItem.UserNo.c_str();
                        regsipdialog.password = UserItem.Password.c_str();
		}
		regsipdialog.isAuth = true;
/*
		if(regType==ADDNEWREG)
		{
			//新登记的用户需要过载检查
			//登记过的用户就不用过载检查了
			cleanSQLStatement();
			sprintf(m_cSQLStatement,"SELECT count(*) FROM registeruser;");
			if(db->execSQL(m_cSQLStatement))
			{
				selectResult = db->getSelectResult();
				if ((selectResult!=NULL)&&(selectResult->rowNum>0))
				{
					row = selectResult->pRows;
					unsigned int currusernum = row->arrayField[0].value.intValue;
					if(currusernum>=MaxRegisterUserNum)
					{
					    UniDEBUG("test:%d",MaxRegisterUserNum);
						registerres.result =OVERLOAD;
						return registerres;
					}
				}
			}
		}*/
		//执行用户登录或刷新
               
		switch(regType)
		{
			case ADDNEWREG:
				registerres.result=cs_RegUser(UserNo, Contact, db);
				break;
			case REFREASHREG:
				regsipdialog.ipAddr = Contact->host.c_str();
				regsipdialog.port = Contact->port;
				regsipdialog.regtime = time(0) ;
                                regsipdialog.nonce = Credentials->GetNonceRef();
				registerres.result =  REGSUCC;
				break;
			case DELREG:
			    	registerres.result=cs_UnRegUser(UserNo, Contact, db);
			    //    registerres.result = UNREGSUCC;
				break;
			case QUERYREG:
				registerres.result =  REGQUERY;
		}
	}
	else
	{
	//没有用户注册记录，需要拒绝
		registerres.result =  NOTEXIST;
		return registerres;
	}

	//目前只支持一个用户注册，以后如果支持多个的话，就需要重新查询了
	if ((REGSUCC==registerres.result)||(REGQUERY==registerres.result)||(UNREGSUCC==registerres.result))
	{
		cleanSQLStatement();
		sprintf(m_cSQLStatement,"SELECT ipAddr,port,unregistertime,nonce transport FROM registeruser WHERE userno=\'%s\';", UserNo);
		if (db->execSQL(m_cSQLStatement))
		{
			selectResult = db->getSelectResult();
			if ((selectResult!=NULL)&&(selectResult->rowNum>0))
			{
				row = selectResult->pRows;
				while (row != NULL)
				{
					registerres.contact.host.SetVarCharContent(row->arrayField[0].value.stringValue ,strlen(row->arrayField[0].value.stringValue));
					registerres.contact.port = row->arrayField[1].value.intValue;
                                        //向注册dialog中插入用户信息
                                        regsipdialog.ipAddr = registerres.contact.host.c_str();
                                        regsipdialog.port = registerres.contact.port;
					registerres.contact.mAllContacts=false;
					if(REGQUERY==registerres.result)
						registerres.contact.expires=row->arrayField[2].value.intValue-time(0);
					else
						registerres.contact.expires = Contact->expires;
                                        regsipdialog.nonce = row->arrayField[3].value.stringValue;
                                        registerres.contact.nonce = row->arrayField[3].value.stringValue;
                                        registerres.contact.transport = row->arrayField[4].value.intValue;
                                        regsipdialog.transport = row->arrayField[4].value.intValue;
					row = row->next;
				}
				//registerres.result = AUTHSUCC;	//成功的话，保留原来成功注册注销的返回类型
			}
			//else
			//{
			//	registerres.result = NOTREG;
			//}
		}
	}

	return registerres;

}
/*
INT CRegCallService::CS_3ptyREGISTER(const CHAR *UserNo, S3ptyREGISTER& reglist, CDB* db)
{

	cleanSQLStatement();
	sprintf(m_cSQLStatement,"SELECT category.ipaddr, category.port FROM category, stlist WHERE stlist.servReg=1 and stlist.STPrefix=\'%s\' and (stlist.userType=1 or stlist.userType=2) and stlist.AppServID=category.appservid;", UserNo);
	if (db->execSQL(m_cSQLStatement))
	{
		selectResult = db->getSelectResult();
		if ((selectResult!=NULL)&&(selectResult->rowNum>0))
		{
			row = selectResult->pRows;
			int i=0;
			while (row != NULL && i<32)
			{
				reglist.servAddr[i].host.SetVarCharContent(row->arrayField[0].value.stringValue ,strlen(row->arrayField[0].value.stringValue));
				reglist.servAddr[i].port = row->arrayField[1].value.intValue;
				i++;
				row = row->next;
			}
			reglist.servnum=i;
		}
		else
		{
			reglist.servnum=0;
		}
	}
	else
	{
		reglist.servnum=0;
	}

	return reglist.servnum;
}

*/
BOOL CRegCallService::cs_QueryUser(const CHAR* UserNo, SUSER &UserItem, CDB* db)
{
	cleanSQLStatement();
	sprintf(m_cSQLStatement,"SELECT password,authorization FROM user WHERE userno=\'%s\';", UserNo);
//	cout<<"QueryUser:"<<m_cSQLStatement;
	if (db->execSQL(m_cSQLStatement))
	{
		selectResult = db->getSelectResult();
		if ((selectResult!=NULL)&&(selectResult->rowNum>0))
		{
			row = selectResult->pRows;
			while (row != NULL)
			{
				UserItem.Password.SetVarCharContent(row ->arrayField[0].value.stringValue, strlen(row ->arrayField[0].value.stringValue));
				UserItem.Authorization = row->arrayField[1].value.intValue;
				row = row->next;
			}
			UserItem.UserNo.SetVarCharContent(UserNo, strlen(UserNo));

			return TRUE;
		}
	}
	return FALSE;
}

REG CRegCallService::cs_RegUser(const CHAR *UserNo,const SSIPContact *Contact,CDB* db)
{
	time_t regtime;
	if (NULL==Contact || UserNo==NULL)
		return REGFAIL;

	regtime = time(0);

//		cleanSQLStatement();
//		sprintf(m_cSQLStatement,"SELECT UserNo FROM registeruser WHERE UserNo=\'%s\';", UserNo);
//		if (CDB::instance()->execSQL(m_cSQLStatement))
//		{
//			selectResult = CDB::instance()->getSelectResult();
//			if ((selectResult!=NULL)&&(selectResult->rowNum>0))
//			{
//				cleanSQLStatement();
//				sprintf(m_cSQLStatement,"UPDATE registeruser SET ipaddr=\'%s\',port=\'%d\',unregisterTime=\'%d\'  WHERE userno=\'%s\';", \
//						Contact->host.c_str(),Contact->port,regtime,UserNo);
//				if (!CDB::instance()->execSQL(m_cSQLStatement))
//					printf("\nDB ERROR\n");
//			}
//			else
//			{

	cleanSQLStatement();
	sprintf(m_cSQLStatement,"DELETE FROM registeruser WHERE userno = \'%s\'",UserNo);
	if (db->execSQL(m_cSQLStatement))
	{
		//cout<<"delete success"<<endl;
	}

	cleanSQLStatement();
	sprintf(m_cSQLStatement,"INSERT INTO registeruser (userno, sourceid, ipaddr, port, unregistertime, userstate, nonce, transport) VALUES(\'%s\',\'%d\',\'%s\',\'%d\',\'%d\',\'%d\',\'%s\',\'%d\');",
			UserNo,Contact->sourceid,Contact->host.c_str(),Contact->port,regtime, 0, Contact->nonce.c_str(), Contact->transport);
	if (!db->execSQL(m_cSQLStatement))
	{
		printf("\nDB ERROR: %s\n",db->getErrMsg());
		return REGFAIL;
	}
	else
	{
		return REGSUCC;
	}
}


REG CRegCallService::cs_RefreashUser(const CHAR *UserNo,const SSIPContact *Contact, CDB* db)
{
	time_t regtime;
	if (NULL==Contact || UserNo==NULL)
		return REGFAIL;

	regtime = time(0);
	cleanSQLStatement();
	sprintf(m_cSQLStatement,"UPDATE registeruser SET ipaddr=\'%s\',port=\'%d\',unregisterTime=\'%d\'  WHERE userno=\'%s\';", \
				Contact->host.c_str(),Contact->port,regtime,UserNo);
	if (!db->execSQL(m_cSQLStatement))
	{
		printf("\nDB ERROR: %s\n",db->getErrMsg());
		return REGFAIL;
	}
	else
	{
		return REGSUCC;
	}

}

REG CRegCallService::cs_UnRegUser(const CHAR *UserNo,const SSIPContact *Contact, CDB* db)
{
	time_t regtime;
	if (NULL==Contact || UserNo==NULL)
		return REGFAIL;

	//×¢Ïú
	if(TRUE == Contact->mAllContacts)
	{
		cleanSQLStatement();
		sprintf(m_cSQLStatement,"DELETE FROM registeruser WHERE userno=\'%s\';", UserNo);
		if (db->execSQL(m_cSQLStatement))
		{

			return UNREGSUCC;
		}
		else
		{
			printf("\nDB ERROR: %s\n",db->getErrMsg());
			return UNREGFAIL;
		}
	}
	else
	{
			cleanSQLStatement();
			sprintf(m_cSQLStatement,"DELETE FROM registeruser WHERE userno=\'%s\' AND ipaddr=\'%s\' AND port=%d;", UserNo,Contact->host.c_str(),Contact->port);
			if (db->execSQL(m_cSQLStatement))
			{
				return UNREGSUCC;
			}
			else
			{
				printf("\nDB ERROR: %s\n",db->getErrMsg());
				return REGFAIL;
			}
	}
}

BOOL CRegCallService::cs_UpdateDialogToDB(CSIPRegDialogInfo &regsipdialog , CDB* db)
{
	cleanSQLStatement();
	sprintf(m_cSQLStatement,"UPDATE registeruser SET unregisterTime=\'%d\'  WHERE userno=\'%s\';", \
				time(0),regsipdialog.username.c_str());
	if (!db->execSQL(m_cSQLStatement))
	{
		printf("\nDB ERROR: %s\n",db->getErrMsg());
		return false;
	}
	else
	{
		return true;
	}
        return false;
       
}
BOOL CRegCallService::cs_CheckActiveUser(const CHAR *UserNo, CDB* db)
{
	time_t currtime;
	currtime=time(0);

	cleanSQLStatement();
	sprintf(m_cSQLStatement,"SELECT ipAddr,port,unregistertime FROM registeruser WHERE userno=\'%s\';", UserNo);
	if (db->execSQL(m_cSQLStatement))
	{
		selectResult = db->getSelectResult();
		if ((selectResult!=NULL)&&(selectResult->rowNum>0))
		{
			return TRUE;
		}
	}
	else
	{
		UniERROR("CheckActiveUser_select active items Error - DB ERROR:%s ", db->getErrMsg());
	}

	return FALSE;
}

////////////////////////////////////////////////////////////
// Function£ºCheckPwd
// Description:½øÐÐ¶ÔSIPÓÃ»§ÃÜÂëµÄºË¶Ô
// Args:
//        struCredentials£ºÓÃ»§Ìá¹©µÄÈÏÖ¤Êé
// Return Values:BOOL
//        ÈôÃÜÂë¼ì²éÕýÈ·£¬Ôò·µ»ØTRUE£»ÈôÃÜÂë¼ì²é´íÎó£¬Ôò·µ»ØFALSE
////////////////////////////////////////////////////////////
//modified by zhgsh 2004.9.16
BOOL CRegCallService::cs_Auth(const CHAR *cUserName,const CHAR *cPassword,const CSIPCredentials *sipCredentials)
{
	CMD5 tmpCmd5;
	UCHAR* tmpDegist;
	UCHAR  tmpDegistBuf[512];

	UCHAR *tmpInputA1,*tmpInputA2;
	UCHAR ucMd5DigestA1[257];
	//pcMd5DigestA1[0] = '\0';
	UCHAR ucMd5DigestA2[257];
	//  pcMd5DigestA2[0] = '\0';
	UCHAR ucInputDigestA1[257];
	//  pcInputDigestA1[0] = '\0';
	tmpInputA1 = ucInputDigestA1;
	UCHAR ucInputDigestA2[257];
	//  pcInputDigestA2[0] = '\0';
	tmpInputA2 = ucInputDigestA2;
	UCHAR cCodeA1[257];
	UCHAR intCharStr[33];
	UCHAR MD5HA[33];
	UCHAR DigestRes[33];

	if (NULL==sipCredentials)
		return FALSE;

	//录脝脣茫A1
	/* DigestCalcHA1("111",sipCredentials,
	   tmpInputA1,
	   MD5HA,
	   ucInputDigestA1,
	   cCodeA1,
	   tmpCmd5);
	   cout<<"DegistHA1="<<MD5HA<<endl;
	   */

	//录脝脣茫A2
	DigestCalcHA2(sipCredentials,
			tmpInputA2,
			MD5HA,
			ucInputDigestA2,
			tmpCmd5);
	tmpDegist=tmpDegistBuf;
	memcpy(tmpDegist,cPassword,strlen(cPassword));
	tmpDegist += 32;

	tmpDegist[0] = ':';
	tmpDegist += 1;

	memcpy(tmpDegist,(sipCredentials->GetNonceRef().c_str())
			,(sipCredentials->GetNonceRef().length()));
	tmpDegist += (sipCredentials->GetNonceRef().length());

	tmpDegist[0] = ':';
	tmpDegist += 1;
        
	if(sipCredentials->GetQopRef()=="auth" )
	{

		memcpy(tmpDegist,(sipCredentials->GetNonceCountRef().c_str()) ,(sipCredentials->GetNonceCountRef().length()));
		tmpDegist += (sipCredentials->GetNonceCountRef().length());

		tmpDegist[0] = ':';
		tmpDegist += 1;

		memcpy(tmpDegist,(sipCredentials->GetCNonceRef().c_str()),(sipCredentials->GetCNonceRef().length()));
		tmpDegist += (sipCredentials->GetCNonceRef().length());

		tmpDegist[0] = ':';
		tmpDegist += 1;

		memcpy(tmpDegist,(sipCredentials->GetQopRef().c_str()),(sipCredentials->GetQopRef().length()));
                tmpDegist += (sipCredentials->GetQopRef().length());

		tmpDegist[0] = ':';
		tmpDegist += 1;
	}
	memcpy(tmpDegist,MD5HA,32);
	tmpDegist +=32;
        tmpDegist[0] = 0;

	memcpy(DigestRes,tmpCmd5.GetDigest(tmpDegistBuf, tmpDegist-tmpDegistBuf,1),32);
	DigestRes[32]=0;
        
	int result=memcmp(DigestRes,(UCHAR*)sipCredentials->GetResponseRef().c_str(),32);
	if(0==result)
	{
		UniDEBUG("Checkpassword Successful, User=%s",cUserName);
		return TRUE;
	}
	else
	{
		UniDEBUG("Checkpassword Failed, User=%s",cUserName);
		return FALSE;
	}


}

/* calculate H(A1) as per spec */
//added by zhgsh 2004.9.16

void CRegCallService::DigestCalcHA1(
	const CHAR* cPassword,
	const CSIPCredentials *sipCredentials,
	UCHAR* tmpInputA1,
	UCHAR* ucMd5DigestA1,
	UCHAR* ucInputDigestA1,
	UCHAR* cCodeA1,
	CMD5 &tmpCmd5)
{
    memcpy(tmpInputA1,sipCredentials->GetUserNameRef().c_str()
            ,sipCredentials->GetUserNameRef().size());
    tmpInputA1 += sipCredentials->GetUserNameRef().size();

    tmpInputA1[0] = ':';
    tmpInputA1 += 1;

    memcpy(tmpInputA1,sipCredentials->GetRealmRef().c_str()
    ,sipCredentials->GetRealmRef().size());
    tmpInputA1 += sipCredentials->GetRealmRef().size();

    tmpInputA1[0] = ':';
    tmpInputA1 += 1;

    memcpy(tmpInputA1,cPassword,strlen(cPassword));
    tmpInputA1 += strlen(cPassword);

   	//H( unq(username-value) ":" unq(realm-value)":" passwd )
    memcpy(ucMd5DigestA1,
    	tmpCmd5.GetDigest(ucInputDigestA1,tmpInputA1-ucInputDigestA1),16);


}

/* calculate H(A2) as per spec */
//modify by zhgsh 2004.9.16

void CRegCallService::DigestCalcHA2(
	const CSIPCredentials *sipCredentials,
	UCHAR* tmpInputA2,
	UCHAR* ucMd5DigestA2,
	UCHAR* ucInputDigestA2,
	CMD5 &tmpCmd5)
{
    switch (sipCredentials->GetMsgType())
    {
      case resip::INVITE:
        memcpy(tmpInputA2,"INVITE",strlen("INVITE"));
        tmpInputA2 += strlen("INVITE");

        break;
      case resip::ACK:
        memcpy(tmpInputA2,"ACK",strlen("ACK"));
        tmpInputA2 += strlen("ACK");

        break;
      case resip::OPTIONS:
        memcpy(tmpInputA2,"OPTIONS",strlen("OPTIONS"));
        tmpInputA2 += strlen("OPTIONS");

        break;
      case resip::BYE:
        memcpy(tmpInputA2,"BYE",strlen("BYE"));
        tmpInputA2 += strlen("BYE");

        break;
      case resip::CANCEL:
        memcpy(tmpInputA2,"CANCEL",strlen("CANCEL"));
        tmpInputA2 += strlen("CANCEL");

        break;
      case resip::REGISTER:
        memcpy(tmpInputA2,"REGISTER",strlen("REGISTER"));
        tmpInputA2 += strlen("REGISTER");

        break;
	case resip::SUBSCRIBE:
        memcpy(tmpInputA2,"SUBSCRIBE",strlen("SUBSCRIBE"));
        tmpInputA2 += strlen("SUBSCRIBE");

        break;
	case resip::PUBLISH:
        memcpy(tmpInputA2,"PUBLISH",strlen("PUBLISH"));
        tmpInputA2 += strlen("PUBLISH");

        break;
	case resip::MESSAGE:
        memcpy(tmpInputA2,"MESSAGE",strlen("MESSAGE"));
        tmpInputA2 += strlen("MESSAGE");

        break;
      default:
        break;
    }

    tmpInputA2[0] = ':';
    tmpInputA2 += 1;

    memcpy(tmpInputA2,(sipCredentials->GetURIRef().c_str())
    	,(sipCredentials->GetURIRef().length()));
    tmpInputA2 += (sipCredentials->GetURIRef().length());
    //H(A2)
    memcpy(ucMd5DigestA2,tmpCmd5.GetDigest(ucInputDigestA2,tmpInputA2-ucInputDigestA2,1),32 );

}

void CRegCallService::CvtHex(const UCHAR *Bin, UCHAR *Hex)
{
    unsigned short i;
    unsigned char j;

    for (i = 0; i < 16; i++)
   {
        j = (Bin[i] >> 4) & 0xf;
        if (j <= 9)
            Hex[i*2] = (j + '0');
         else
            Hex[i*2] = (j + 'a' - 10);
        j = Bin[i] & 0xf;
        if (j <= 9)
            Hex[i*2+1] = (j + '0');
         else
            Hex[i*2+1] = (j + 'a' - 10);
    }
    Hex[32]='\0';
};

bool CRegCallService::cs_CheckNonce(const CHAR* UserNo, const CHAR* nonce, CDB* db )
{
	cleanSQLStatement();
	sprintf(m_cSQLStatement,"SELECT nonce FROM registeruser WHERE userno=\'%s\';", UserNo);
	if (db->execSQL(m_cSQLStatement))
	{
		selectResult = db->getSelectResult();
		if ((selectResult!=NULL)&&(selectResult->rowNum>0))
		{
                        TRow *row = selectResult->pRows;
                        while(row != NULL)
                        {
//                                cout<<nonce<<endl;
//                                cout<<row->arrayField[0].value.stringValue<<endl;
				if( strcmp(row->arrayField[0].value.stringValue, nonce)== 0)
                                        return true;
                                row = row->next;
			}
		
		}
	}
        return false;
   
}
