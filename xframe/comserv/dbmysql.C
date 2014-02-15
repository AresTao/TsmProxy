/****************************************************************
 * Copyright (c)2005-2008, by Marching Telecom. Tech. Ltd.
 * All rights reserved.

 * FileName：			dbmysql.C
 * System：          	UniFrame
 * SubSystem：        	Common System Service
 * Author：				Wang Shufeng
 * Date：				2002.05.30
 * Version：			1.0
 * Description：
     	数据库的c++编程接口程序. MySQL实现。


 * Last Modified:
    2002.05.30, 	By Wang Shufeng
    				完成初始版本
	2002.06.02,		By Wang Shufeng
					添加类接口FreeRowMem.
	2002.06.03,		By Wang Shufeng
					添加类接口GetIntoPara;
					增加提供unsinged类型接口.
	2002.06.04,		By Wang Shufeng
					 按照龙湘明的建议,将类接口GetInto
					 更名为GetIntoPara,将其返回值变更
					 为BOOL类型;
	2002.06.17,		By Wang Shufeng
					 为了增强程序的容错性,将类接口
					 GetIntoPara返回值类型变更为INT;对参数
					 输入形式做了一定的修改,使之完全符合scanf()
					 的参数调用形式,也符合大家的编程习惯.
	2002.06.26,		By Wang Shufeng
					增加连接到不同数据库服务的接口
					ConnDB(CHAR* uid,CHAR* pwd,CHAR* dbsv)
	2003.05.15,		By Wang Shufeng
					增加了有关线程安全的语句
					EXEC SQL ENABLE THREADS;
					EXEC SQL CONTEXT ALLOCATE :m_ctx;
					EXEC SQL CONTEXT USE :m_ctx;
					EXEC SQL CONTEXT FREE :m_ctx;
					主要保证在命名服务中使用线程调用cdb接口时不会
					出现内存错误。
	2003.06.07,		By Wang Shufeng
					增加定义类的静态变量sm_bThreadEnabled,以解决在多个
					CDB对象连接数据库时，出现的内存泄漏的问题。
	2003.06.07,		By Wang Shufeng
					修改EXEC SQL CONTEXT USE :m_ctx
	 				为	if (m_ctx != NULL){ EXEC SQL CONTEXT USE :m_ctx;}
					    else
						{
							m_pcErrMsg = (CHAR*)"Thread Context has been freed!Not Connected to Database!";
							return 0;
						}
	 				因为，如果m_ctx==NULL的话，在执行后面的EXEC SQL语句时，
	 				会出现段错误。
	2003.06.07,		By Wang Shufeng
					删除方法INT ConnDB(CHAR* uid,CHAR* pwd)
					统一使用INT ConnDB(CHAR* uid,CHAR* pwd)
	2003.07.13，	By Wang Shufeng
					原程序，如果第一次的结果没有全部释放，
	            	而执行第二次查询，会产生内存泄漏的现象。
	            	应赵文峰建议，进行修改：加入判断，如果m_pstruSlct_Rslt!=NULL,
	            	在第二次查询时，将第一次的查询结果全部释放掉。
	2004.07.30,		By Wang Shufeng
					增加MYSQL版本对“SHOW”语句的支持
					Line 302增加“||(strncasecmp(pcSQLStmt,(CHAR*)"SHOW",4) == 0)"

    2005.10.09      By Long Xiangming
	                将原来mysql和oracle合在一起的cdb.C分开成dbmysql.C和dboracle.C两个文件。
					因为两者公共部分几乎没有，没必要放在一起。
    2006.9.19       By Longxiangming
	 				在Windows下，对LONGLNOG使用atol()函数。
****************************************************************/
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>

#ifdef WIN32
#include <winsock2.h>
#endif

#include "dbmysql.h"

#include "comtypedef_vchar.h"
CDBMysql::CDBMysql():CDB()
{
	strcpy(m_struDBInfo.DBName,"test");
	strcpy(m_struDBInfo.DBAddr ,"localhost");
	m_struDBInfo.DBPort=0; //use the default port of mysql.

	mysql_init(&m_struMYSQL);
	//modified by LXM. 2009.07.07
	//mysql_options(&m_struMYSQL,MYSQL_OPT_CONNECT_TIMEOUT,(CHAR*)"5");
	////自动重新连接，只在MySQL 5.0.13之后支持
	mysql_options(&m_struMYSQL, MYSQL_OPT_RECONNECT, (char *)"1");

	m_pMysqlRes = NULL;
	alreadyInit = FALSE;
}

CDBMysql::~CDBMysql()
{
	//Added by Wang Shufeng,2004,8.23
	//防止发生内存泄漏.
	if (m_pMysqlRes != NULL) mysql_free_result(m_pMysqlRes);
	m_pMysqlRes = NULL;

	if (m_bConnFlag) disConnDB();
	mysql_thread_end();
}


/////////////////////////////////////////////////////////
// Function：	connDB
// Description:
//	执行连接ORACLE数据库的操作
//
// Args：
//	CHAR* uid:指向用户名的字符指针;
//	CHAR* psw:指向用户口令的字符指;
//	CHAR* dbsv:所要连接的数据库服务名.
// Return Values
//	INT 当返回1时,成功连接数据库;
//		当返回0时,连接失败.
//////////////////////////////////////////////////////////
INT CDBMysql::connDB(const CHAR* uid,const CHAR* pwd,const CHAR* dbsv)
{
	return connDB(uid,pwd,dbsv,"localhost",0);

}
//当MySQL服务器单独部署时，需要使用远程连接方式
INT  CDBMysql::connDB(const CHAR* uid,const CHAR* pwd,const CHAR* dbsv,const CHAR* host, INT port)
{
	if (m_bConnFlag) disConnDB();

	m_bConnFlag = FALSE;

	/*
	 *如果用户名、口令或者数据库服务名超过指定长度，错误返回。
	 */
	if ((strlen(uid) >= MAX_UID_LEN)||
		(strlen(pwd) >= MAX_PWD_LEN)||
		(strlen(dbsv) >= MAX_DBSV_LEN))
	{
		strcpy(m_pcErrMsg , (CHAR*)"Too long uid or pwd or dbsv");
		return 0;
	}

	if (mysql_real_connect(&m_struMYSQL, host, uid, pwd, dbsv, port, NULL, 0))
	{
		m_bConnFlag = TRUE;
		mysql_autocommit(&m_struMYSQL,1);
		strcpy(m_pcDBsv,dbsv);
		strcpy(m_pcUsr,uid);
		strcpy(m_pcPwd,pwd);
		strcpy(m_struDBInfo.DBAddr,host);
		m_struDBInfo.DBPort=port;
		strcpy(m_struDBInfo.DBName,dbsv);
		alreadyInit = TRUE;
		return 1;
	}
	else
	{
		strcpy(m_pcErrMsg , (CHAR*)mysql_error(&m_struMYSQL));
		return 0;
	}
}

//重连数据库
INT CDBMysql::reconnect()
{
	if(!alreadyInit) return 0; //至少应连接过一次

	if (m_bConnFlag) disConnDB();
	return connDB(m_pcUsr,m_pcPwd,m_pcDBsv,m_struDBInfo.DBAddr,m_struDBInfo.DBPort);

}

//	断开数据库操作;
void CDBMysql::disConnDB()
{
	if (!m_bConnFlag)
    {
    	return;
    }

	mysql_close(&m_struMYSQL);

	/*
	 *重置数据库连接状态。
	 */
	m_bConnFlag = FALSE;
}

int CDBMysql::isSelectStmt(const char* stmt)
{
   return strncasecmp(stmt,(CHAR*)"SELECT",6) == 0 || 
          strncasecmp(stmt,(CHAR*)"SHOW",4) == 0 ||
          strncasecmp(stmt,(CHAR*)"DESCRIBE",8) == 0
   ;
}
/////////////////////////////////////////////////////////
// Function：	execSQL
// Description:
//	对用户提交的SQL语句进行处理.如果是查询语句,将查询结果
//	以链表的形式保存.只有在connDB()函数成功执行之后,才能
//	够执行成功.否则,产生未连接数据库错误.
// Args：
//	CHAR* 指向用户提交的SQL语句的字符指针.
// Return Values
//	INT 当返回1时,表明SQL语句成功执行;
//		当返回0时,表明SQL语句执行失败.
//////////////////////////////////////////////////////////
INT CDBMysql::execSQL1(const char* pcSQLStmt)
{
   int re;
	//如果SQL语句过长，直接返回错误信息。
	if (strlen(pcSQLStmt) >= MAX_STMT_LEN)
    {
    	strcpy(m_pcErrMsg , (CHAR*)"Too Long Statement(beyond 1024 bytes)");
      re = 0;
    }

    //如果没有连接到数据库，直接返回错误信息。
   else if (!m_bConnFlag)
    {
    	strcpy(m_pcErrMsg , (CHAR*)"Have not connected to the database or the connection lost now!");
      UniERROR(m_pcErrMsg);
      re = 0;
    }

    //在事务处理中，不允许使用COMMIT/ROLLBACK语句，必须调用Commit()和Rollback()接口。
   else if ((
	    (strncasecmp(pcSQLStmt, "COMMIT", 6) == 0)||
        (strncasecmp(pcSQLStmt, "ROLLBACK", 8) == 0)
        )&&(m_bTransaction)
       )
    {
    	strcpy(m_pcErrMsg , (CHAR*)"Can not commit or rollback work in a read-only transaction with a SQL statement;");
      re = 0;
    }
  else if (mysql_real_query(&m_struMYSQL, pcSQLStmt, strlen(pcSQLStmt)) == 0)
	{
		strncpy(m_pcSQLStmt,pcSQLStmt,MAX_STMT_LEN);
      if(isSelectStmt(pcSQLStmt))
		{
			m_iQueryFlag = 1;

			//Added by Wang Shufeng,2004,8.23
			//防止发生内存泄漏.
			if (m_pMysqlRes != NULL) mysql_free_result(m_pMysqlRes);
			m_pMysqlRes = NULL;
			m_pMysqlRes = mysql_store_result(&m_struMYSQL);
			freeResult(); //释放上次查询结果内存。
			m_isFirstlyGetResult = TRUE;
		}

		if (m_bTransaction)
		{
        if(m_iQueryFlag)
           m_iProcRowCount += mysql_num_rows(m_pMysqlRes);            
         else
				m_iProcRowCount += mysql_affected_rows(&m_struMYSQL);
			}
      re = 1;
	}
	else
	{
		strcpy(m_pcErrMsg , (CHAR*)mysql_error(&m_struMYSQL));
      re = 0;
   }
   return re;
}


PTSelectResult CDBMysql::getSelectResult()
{
    if(m_isFirstlyGetResult==FALSE) return m_pSelectResult; //一次查询后，可能多次调用getSelectResult(),
	                                                        //只取第一次的结果

	m_isFirstlyGetResult = FALSE;
	MYSQL_FIELD *field = mysql_fetch_fields(m_pMysqlRes);
	UINT num_rows   = mysql_num_rows(m_pMysqlRes);
	if (num_rows == 0)
	{
		//m_pSelectResult = NULL;
        strcpy(m_pcErrMsg,"No results return.");
		m_pSelectResult ->rowNum = 0;
        return m_pSelectResult;
	}

	UINT num_fields = mysql_num_fields(m_pMysqlRes);
    if(num_fields>MaxFieldNum)
    {
      UniERROR("CDBMysql::getSelectResult() --> ERROR: num_fields(%d) is more than MaxFieldNum(%d).",num_fields,MaxFieldNum);
	   num_fields = MaxFieldNum;
	}

	//UINT *lengths;
	size_t *lengths;
	MYSQL_ROW row;
    MYSQL_FIELD *fields;

	TRow    *pRowTemp;
	TRow    *pRowSwap;
	TField  fieldTemp;
	while ((row = mysql_fetch_row(m_pMysqlRes)))
	{

	   fields = mysql_fetch_fields(m_pMysqlRes);
	   //lengths = (UINT*)mysql_fetch_lengths(m_pMysqlRes);
	   lengths = (size_t*)mysql_fetch_lengths(m_pMysqlRes);
	   pRowTemp = new TRow;
	   for(int i = 0; i < num_fields; i++)
	   {
	   		CHAR* chTmp;
 		    INT iTmp = 0;
		    chTmp = new CHAR[lengths[i]+1];
		    memset(chTmp,0,lengths[i]+1);
         if(row[i] != NULL && lengths[i] != 0)
		    strncpy(chTmp,row[i],lengths[i]);
		    switch (fields[i].type)
   			{
	   			case MYSQL_TYPE_DECIMAL:
					if (lengths[i] == 0)
	   				{
	   					fieldTemp.isNull = TRUE;
	   				}
	   				else
	   				{
		   				fieldTemp.isNull = FALSE;
						fieldTemp.type = T_UINT;
						fieldTemp.value.uintValue  = strtoul(chTmp,0,0);
		   			}
	   				break;
	   			case MYSQL_TYPE_TINY:
	   			case MYSQL_TYPE_SHORT:
	   			case MYSQL_TYPE_LONG:
	   				if (lengths[i] == 0)
	   				{
	   					fieldTemp.isNull = TRUE;
	   				}
	   				else
	   				{
		   				fieldTemp.isNull = FALSE;
						fieldTemp.type = T_UINT;
						fieldTemp.value.uintValue  = strtoul(chTmp,0,0);
		   			}
	   				break;
	   			case MYSQL_TYPE_LONGLONG:
					if (lengths[i] == 0)
	   				{
	   					fieldTemp.isNull = TRUE;
	   				}
	   				else
	   				{
		   				fieldTemp.isNull = FALSE;
						fieldTemp.type = T_LONGLONG;
#ifdef WIN32
						fieldTemp.value.longlongValue  = atol(chTmp);
#else
						fieldTemp.value.longlongValue  = atoll(chTmp);
#endif
		   			}
					break;
	   			case MYSQL_TYPE_FLOAT:
				case MYSQL_TYPE_DOUBLE:
					if (lengths[i] == 0)
	   				{
	   					fieldTemp.isNull = TRUE;
	   				}
	   				else
	   				{
		   				fieldTemp.isNull = FALSE;
						fieldTemp.type = T_FLOAT;
						fieldTemp.value.floatValue = atof(chTmp);
		   			}
					break;
	   			default: //for string
					if(lengths[i]>MaxStrAIDLength)
					{
						m_pSelectResult -> rowNum = 0;
						sprintf(m_pcErrMsg,"length of field %s is too long[>%d]!",fields[i].name,lengths[i]);
						delete pRowTemp;
						delete[] chTmp;
						return m_pSelectResult;
					}
					fieldTemp.isNull = FALSE;
					fieldTemp.type = T_STRING;
					strncpy(fieldTemp.value.stringValue,chTmp,strlen(chTmp)+1);
	   				break;
	   			}

	   			//删除临时变量空间
	   			delete [] chTmp;
	   			pRowTemp->arrayField[i] = fieldTemp;
		   }//end of for


	      if (m_pSelectResult -> rowNum == 0)
		  {
	  		  m_pSelectResult-> rowNum  = num_rows;
	          m_pSelectResult-> fieldNum = num_fields;
			  m_pSelectResult->pRows = pRowTemp;
			  pRowTemp->next = NULL;
			  pRowSwap = pRowTemp;
			  for(int j=0;j<num_fields;j++)
			  {
				 strcpy(m_pSelectResult->fieldInfos[j].name,fields[j].name);
				 m_pSelectResult->fieldInfos[j].type = pRowTemp->arrayField [j].type;
				 m_pSelectResult->fieldInfos[j].len  = lengths[j];
			  }

		  }
	   	  else
		  {
	   	  	  pRowSwap ->next = pRowTemp;
			  pRowTemp->next  = NULL;
			  pRowSwap = pRowTemp;
		  }
	   }//end of while

       return m_pSelectResult;
}


/////////////////////////////////////////////////////////
// Function：	getNextRow
// Description:
// 	 查询数据库时,在执行完ExecSQL函数后执行GetNextRow,取得
//	 第一行数据.再次执行,取出下一行数据
// Args：
//	struct slctRslt** 输出参数,指向取出数据行的第一个数据单
//					  元的指针
// Return Values
//	INT 取出数据行返回1,否则返回0
//////////////////////////////////////////////////////////
INT CDBMysql::getNextRow(struct slctRslt** ppstruSlctRslt)
{
	if (!m_bConnFlag)
	{
		strcpy(m_pcErrMsg , (CHAR*)"Have not connected to DB now!");
		return 0;
	}

	if (m_iQueryFlag == 0)
	{
		strcpy(m_pcErrMsg , (CHAR*)"The ExecSQL operation is not executed,or is not successful,or is not a query operation!");
		return 0;
	}
	MYSQL_FIELD *field = mysql_fetch_fields(m_pMysqlRes);
	MYSQL_ROW row = mysql_fetch_row(m_pMysqlRes);
	if (row == NULL)
	{
		ppstruSlctRslt = NULL;
		return 0;
	}
	else
	{
	   UINT num_fields = mysql_num_fields(m_pMysqlRes);
      size_t *lengths;
      lengths = (size_t*)mysql_fetch_lengths(m_pMysqlRes);      
	   struct slctRslt* pSlctRslt = NULL;
	   struct slctRslt* pPreSlctRslt = NULL;
	   for(INT i = 0; i < num_fields; i++)
	   {
	   		pSlctRslt = new struct slctRslt;
	   		pSlctRslt->ResultLen = lengths[i];
	   		CHAR* chTmp;
	   		INT iTmp = 0;
	   		chTmp = new CHAR[lengths[i]+1];
	   		memset(chTmp,0,lengths[i]+1);
	   		strncpy(chTmp,row[i],lengths[i]);
	   		switch (field[i].type)
	   		{
	   		case MYSQL_TYPE_DECIMAL:
	   		case MYSQL_TYPE_TINY:
	   		case MYSQL_TYPE_SHORT:
	   		case MYSQL_TYPE_LONG:
	   			if (lengths[i] == 0)
	   			{
	   				pSlctRslt->ResultLen = 0;
	   				pSlctRslt->pvResult = NULL;
	   			}
	   			else
	   			{
		   			pSlctRslt->ResultLen = sizeof(INT);
		   			pSlctRslt->pvResult = malloc(sizeof(INT));
	    			//iTmp = atoi(chTmp);
	    			//memcpy(pSlctRslt->pvResult,
	    			//	   (void*)&iTmp,
	    			//	   pSlctRslt->ResultLen);
		   			*(INT*)pSlctRslt->pvResult = atoi(chTmp);
		   		}
	   			break;
	   		case MYSQL_TYPE_LONGLONG:
	   		case MYSQL_TYPE_FLOAT:
	   			if (lengths[i] == 0)
	   			{
	   				pSlctRslt->ResultLen = 0;
	   				pSlctRslt->pvResult = NULL;
	   			}
	   			else
	   			{
		   			pSlctRslt->ResultLen = sizeof(FLOAT);
		   			pSlctRslt->pvResult = malloc(sizeof(FLOAT));
		   			*(FLOAT*)pSlctRslt->pvResult = (FLOAT)atof(chTmp);
		   		}
	   			break;
	   		case MYSQL_TYPE_DOUBLE:
	   			if (lengths[i] == 0)
	   			{
	   				pSlctRslt->ResultLen = 0;
	   				pSlctRslt->pvResult = NULL;
	   			}
	   			else
	   			{
		   			pSlctRslt->ResultLen = sizeof(double);
		   			pSlctRslt->pvResult = malloc(sizeof(double));
		   			*(double*)pSlctRslt->pvResult = atof(chTmp);
		   		}
	   			break;
	   		default:
	   			pSlctRslt->ResultLen = strlen(chTmp);
	   			pSlctRslt->pvResult = malloc(strlen(chTmp)+1);
	   			memset(pSlctRslt->pvResult,0,strlen(chTmp)+1);
	   			strncpy((CHAR*)(pSlctRslt->pvResult),chTmp,strlen(chTmp));
	   			break;
	   		}

	   		//删除临时变量空间
	   		delete [] chTmp;
	   		pSlctRslt->pstruNext = NULL;
	   		if (pPreSlctRslt == NULL)
	   		{
	   			*ppstruSlctRslt = pSlctRslt;
	   			pPreSlctRslt = pSlctRslt;
	   		}
	   		else
	   		{
	   			pPreSlctRslt->pstruNext = pSlctRslt;
	   			pPreSlctRslt = pSlctRslt;
	   		}
	   }
	   return 1;
	}
}

/////////////////////////////////////////////////////////
// Function：	getRowCount
// Description:
// 	 当SQL语句为"SELECT"("select"),"INSERT"("insert"),
//   "UPDATE"("update"),"DELETE"("delete")类型时返回该
//   语句执行的行数
// Return Values
//	INT 返回执行语句的行数
//////////////////////////////////////////////////////////
INT CDBMysql::getRowCount()
{
   if(m_iQueryFlag)
      return mysql_num_rows(m_pMysqlRes);   
   if(m_bTransaction)
      return m_iProcRowCount;
   return mysql_affected_rows(&m_struMYSQL);
}


//	释放查询结果占用的内存空间
//	struct slctRslt* 指向要释放空间的行的行首结构的指针
void CDBMysql::freeRowMem(struct slctRslt* pstruSlctRslt)
{
	struct slctRslt* p = NULL;

	/*
	 *删除pstruSlctRslt指针执行的返回数据行。
	 */
	while (pstruSlctRslt != NULL)
	{
		p = pstruSlctRslt;
		pstruSlctRslt = pstruSlctRslt->pstruNext;
		free(p->pvResult);
		delete p;
		p = NULL;
	}
}

/////////////////////////////////////////////////////////
// Function：	getIntoPara
// Description:
//	将查询结果赋值给参数变量;输入参数个数可变.
//
// Args：
//	CHAR* pcParaType 指明pcParaType后面所跟的参数类型.'s'为
//		*CHAR类型,'d'为*INT类型,'u'为*UINT类型,'f'为
//		*FLOAT类型.例如,GetIntoPara("udsf",uPara,dPara,pcPara,
//		fPara).串"udsf"说明后面的参数从左到右分别为*UINT类型,
//		*INT类型,*CHAR类型和*float类型.

//
//		'c'为CHAR*类型，'C'为*BYTE类型,'h'为*SHORT类型,'H'为*USHORT
//		类型.这四种类型是以*INT类型为基础作强制类型转换而得到的,
//		因此不能保证取出的数据与数据库中的数据相一致.比如说,数据
//		库中的数据为16777214(INT),那么取出的数据分别为-2('c'),254('C'),
//		-2('h'),65534('H').
//				——————ADD BY WSH 2002.7.17

// Return Values
//	INT  当取出数据时返回1,没有数据时返回0;当输入的参数数目与
//		SELECT语句选择的列数不一致时,返回-1;当返回值超过NULLBASE
//		(目前定义为10)时,超出的部分为选择项空值的数目.例如,如果
//		返回值为15,则表示有(15-NULLBASE)=5个空值.另外,返回数据
//		为空值的表示方法为取各种数据类型的最大值(CHAR*例外).例如,INT类型的
//		数据返回值为INT_MAX时,表示该值在数据库中为空值;UINT类型的
//		数据返回值为UINT_MAX时,表示该值在数据库中为空值;FLOAT类型的
//		数据返回值为FLT_MAX时,表示该值在数据库中为空值;CHAR*类型的
//		数据返回值长度为0时,表示该值在数据库中为空值.
//
//		SHORT类型的数据返回值为SHRT_MAX时,表示该值在数据库中为空值;
//		USHORT类型的数据返回值为UNSHORT_MAX时,表示该值在数据库中为空值;
//		CHAR类型的数据返回值为CHAR_MAX时,表示该值在数据库中为空值;
//		BYTE类型的数据返回值为BYTE_MAX时,表示该值在数据库中为空值;
//				——————ADD BY WSH 2002.7.17
////////////////////////////////////////////////////////////
INT CDBMysql::getIntoPara(CHAR* pcParaType,...)
{
	if (!m_bConnFlag)
	{
		strcpy(m_pcErrMsg , (CHAR*)"Have not connected to DB now!");
		return 0;
	}
	if (m_iQueryFlag == 0)
	{
		strcpy(m_pcErrMsg , (CHAR*)"The ExecSQL operation is not executed,or is not successful,or is not a query operation!");
		return 0;
	}
	struct slctRslt *p,*q;
	p = NULL;
	q = NULL;
	CHAR *pc;
	pc = NULL;
	INT i = 0;
	INT nullDataCount = 0;
	INT iParaCount = strlen(pcParaType);
	UINT num_fields = mysql_num_fields(m_pMysqlRes);

	if (num_fields > iParaCount)
	{
		strcpy(m_pcErrMsg , (CHAR*)"Too few parameters");
		return -1;
	}

	if (num_fields < iParaCount)
	{
		strcpy(m_pcErrMsg , (CHAR*)"Too many parameters");
		return -1;
	}

	if (getNextRow(&p) == 0)
	{
		return 0;
	}

	q = p;
	va_list marker;
	va_start( marker, pcParaType);
	while ((q != NULL)&&(i < iParaCount))
	{
		switch (pcParaType[i])
		{
			/*
			 *2002.7.17 ADD BY WSF->
			 */
			/*
			 *CHAR类型数据处理
			 */
			case 'c' :
				if (q->ResultLen ==0 )
				{
					 *(va_arg( marker, CHAR*)) = CHAR_MAX;//以最大INT表示空数据
					 nullDataCount++;
				}
				else *(va_arg( marker, CHAR*)) = *((INT*)q->pvResult);    	//非空数据
    			break;
    		/*
			 *BYTE类型数据处理
			 */
    		case 'C' :
				if (q->ResultLen ==0 )
				{
					 *(va_arg( marker, BYTE*)) = BYTE_MAX;//以最大INT表示空数据
					 nullDataCount++;
				}
				else *(va_arg( marker, BYTE*)) = *((INT*)q->pvResult);    	//非空数据
    			break;
    		/*
			 *SHORT类型数据处理
			 */
    		case 'h' :
				if (q->ResultLen ==0 )
				{
					 *(va_arg( marker, SHORT*)) = SHRT_MAX;//以最大INT表示空数据
					 nullDataCount++;
				}
				else *(va_arg( marker, SHORT*)) = *((INT*)q->pvResult);    	//非空数据
    			break;
    		/*
			 *USHORT类型数据处理
			 */
    		case 'H' :
				if (q->ResultLen ==0 )
				{
					 *(va_arg( marker, USHORT*)) = USHRT_MAX;//以最大INT表示空数据
					 nullDataCount++;
				}
				else *(va_arg( marker, USHORT*)) = *((INT*)q->pvResult);    	//非空数据
    			break;
    		/*
			 *2002.7.17 ADD BY WSF<-
			 */
			/*
			 *INT类型数据处理
			 */
			case 'd' :
				if (q->ResultLen ==0 )
				{
					 *(va_arg( marker, INT*)) = INT_MAX;//以最大INT表示空数据
					 nullDataCount++;
				}
				else *(va_arg( marker, INT*)) = *((INT*)q->pvResult);    	//非空数据
    			break;
    		/*
			 *CHAR*类型数据处理
			 */
    		case 's' :
    			pc = va_arg( marker, CHAR*);
    			if (q->ResultLen == 0) nullDataCount++;
    			strncpy(pc,(CHAR*)q->pvResult,q->ResultLen);
    			pc[q->ResultLen] = '\0';
    			break;
    		/*
			 *UINT类型数据处理
			 */
    		case 'u' :
    			if (q->ResultLen ==0 )
    			{
    				*(va_arg( marker, INT*)) = UINT_MAX;//以最大UINT表示空数据
    				nullDataCount++;
    			}
				else *(va_arg( marker, UINT*)) = *((UINT*)q->pvResult);//非空数据
    			break;
    		/*
			 *FLOAT类型数据处理
			 */
    		case 'f' :
    			if (q->ResultLen ==0 )
    			{
    				*(va_arg( marker, FLOAT*)) = FLT_MAX;//以最大浮点数表示空数据
    				nullDataCount++;
    			}
				else *(va_arg( marker, FLOAT*)) = *((FLOAT*)q->pvResult);//非空数据
    			break;
		}
		i++;
		q = q->pstruNext;
	}

	freeRowMem(p);
	if (nullDataCount == 0)	return 1;
	else return(NULLBASE+nullDataCount);
}

//	开始一个事务处理。
// Args：
//	在MYSQL中，该参数没有实际用处，仅仅为了提供与ORACLE相同的接口。
void CDBMysql::beginTrans(BOOL bIsReadOnly)
{
	if (!m_bConnFlag)
	{
		return;
	}
	m_iProcRowCount = 0;
	mysql_autocommit(&m_struMYSQL,0);
	m_bTransaction = TRUE;
}

//	提交一个事务处理。
void CDBMysql::commit()
{
	if (!m_bConnFlag)
	{
		return;
	}
	m_bTransaction = FALSE;
	//m_bReadOnlyTrans = FALSE;
	m_iProcRowCount = 0;
	mysql_commit(&m_struMYSQL);
	mysql_autocommit(&m_struMYSQL,1);
}

//	回滚一个事务处理。
void CDBMysql::rollback()
{
	if (!m_bConnFlag)
	{
		return;
	}
	m_bTransaction = FALSE;
	//m_bReadOnlyTrans = FALSE;
	m_iProcRowCount = 0;
	mysql_rollback(&m_struMYSQL);
	mysql_autocommit(&m_struMYSQL,1);
}

//检查到数据库的连接是否正常。如果关闭，试图重连接
BOOL CDBMysql::ping()
{
	if(!alreadyInit)
	{
      UniERROR("ERROR in CDBMysql::ping()-->the db is not init yet. You must call CDB::instance() first!\n");
		return FALSE;
	}
	if(mysql_ping(&m_struMYSQL)==0)
		return TRUE;
	else
	{
		BOOL rt = FALSE;
		sprintf(m_pcErrMsg,"%s %s","Database connection lost.", (CHAR*)mysql_error(&m_struMYSQL));
      UniERROR(m_pcErrMsg);
		CHAR temp[256];
		sprintf(temp,"%s","Now trying to reconnect to the database......");
		if(reconnect())
		{
			strcat(temp," succeed.");
         UniINFO(temp);
			rt = TRUE;
		}
		else
		{
			strcat(temp," failed. ");
			strcat(temp,m_pcErrMsg);
         UniERROR(temp);
			rt = FALSE;
		}
		return rt;
	}
}

int CDBMysql::existTable(const char* tableName)
{
   int re = execSql("show tables");
   if(re != 1)
   {
      UniERROR("EXEC SQL: 'show tables' return %d", re);
      return 0;
   }
   TSelectResult* selectResult = getSelectResult();
   if(selectResult == NULL)
   {
      UniERROR("EXEC SQL: 'show tables' result is NULL");
      return 0;
   }
   if(selectResult->fieldNum != 1)
   {
      UniERROR("EXEC SQL: 'show tables' invalid field number %d", selectResult->fieldNum);
      return 0;
   }
   TRow* row = selectResult->pRows;
   while(row != NULL)
   {
      if(!row->arrayField[0].isNull && row->arrayField[0].type == T_STRING && strcasecmp(tableName, row->arrayField[0].value.stringValue) == 0)
         return 1;
      row = row->next;
   }
   return 0;
}

int CDBMysql::existTableField(const char* tableName, const char* fieldName)
{
   CStr sql = "describe ";
   sql << tableName;
   int re = execSql(sql.c_str());
   if(re != 1)
   {
      UniERROR("EXEC SQL: '%s' return %d", sql.c_str(), re);
      return 0;
   }
   TSelectResult* selectResult = getSelectResult();
   if(selectResult == NULL)
   {
      UniERROR("EXEC SQL: '%s' result is NULL", sql.c_str());
      return 0;
   }
   if(selectResult->fieldNum < 1)
   {
      UniERROR("EXEC SQL: '%s' invalid field number %d", sql.c_str(), selectResult->fieldNum);
      return 0;
   }
   TRow* row = selectResult->pRows;
   while(row != NULL)
   {
      if(!row->arrayField[0].isNull && row->arrayField[0].type == T_STRING && strcasecmp(fieldName, row->arrayField[0].value.stringValue) == 0)
         return 1;
      row = row->next;
   }
   return 0;
}
