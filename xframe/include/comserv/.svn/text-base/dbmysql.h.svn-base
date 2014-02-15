/*************************************************************************

 * Copyright (c)2005-2008, by Beijing TeleStar Network Technology Company Ltd.(MT2)
 * All rights reserved.

 * FileName：       dbmysql.h
 * System：         UniFrame 
 * SubSystem：      Common-DB
 * Author：         Long Xiangming
 * Date：           2005.10.06
 * Version：        1.0
 * Description：
    CDBMySQL定义；数据库操作定义。
	完成数据库操作。针对MySQL数据库
	 
 *
 * Last Modified:
     2005.10.06, 完成初始版本
        By Long Xiangming
**************************************************************************/
#ifndef __DBMYSQL_H
#define __DBMYSQL_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mysql.h>
#include "comtypedef.h"
#include "db.h"
class CDBMysql:public CDB
{
 public:   		
   		/*
   		 *构造函数
   		 */
   		 CDBMysql(void);
   		 
   		/*
         *析构函数
         */
        ~CDBMysql(void);

         /*
         *INT connDB(CHAR* uid,CHAR* pwd,CHAR* dbsv)
         *CHAR* uid:指向用户名的字符指针;
		 *CHAR* psw:指向用户口令的字符指;
		 *CHAR* dbsv:所要连接的数据库服务名.
         */
        INT  connDB(const CHAR* uid,const CHAR* pwd,const CHAR* dbsv = DEFAULT_DB);
 
		//--added by Long Xiangming. 2006.11.29
		//当MySQL服务器单独部署时，需要使用远程连接方式
		INT  connDB(const CHAR* uid,const CHAR* pwd,const CHAR* dbsv,const CHAR* host, INT port);
 
        void disConnDB(void);
 	
		/*
   		 *INT ExecSQL(CHAR *pcSQLStmt)
   		 * CHAR* pcSQLStmt参数为指向所要执行的SQL语句字符串的指针
   		 */
   INT execSQL1(const char* pcSQLStmt);

   int existTable(const char* tableName);
   int existTableField(const char* tableName, const char* fieldName);
   int isSelectStmt(const char* sql);

   /*
    * INT getNextRow(struct slctRslt** ppstruSlctRslt);
    * struct slctRslt** 输出参数,指向取出数据行的第一个数据单元的指针
    */

        INT getNextRow(struct slctRslt** ppstruSlctRslt);

        INT getRowCount();

		PTSelectResult getSelectResult();

        /*
         *void freeRowMem(struct slctRslt* pstruSlctRslt)
         *struct slctRslt* 指向要释放空间的行的行首结构的指针
         */
        void freeRowMem(struct slctRslt* pstruSlctRslt);

        /*
         * INT getIntoPara(CHAR* pcParaType,...);
         * CHAR* pcParaType 指明pcParaType后面所跟的参数类型.'s'为
		 * CHAR类型,'d'为*INT类型,'u'为*UINT类型,'f'为
		 * *FLOAT类型.例如,GetIntoPara("udsf",uPara,dPara,pcPara,
		 * fPara).串"udsf"说明后面的参数从左到右分别为*UINT类型,
		 * *INT类型,*CHAR类型和*float类型.
         */
        INT getIntoPara(CHAR* pcParaType,...);

        /*
         *开始一个事务处理。
         *bIsReadOnly为TRUE,表示该事务为只读事务。
         *只读事务可以保证在整个事务过程中数据保持一致性（其他进程
         *执行commit语句不会对该事务造成影响，但是该事务只能执行查询
         *语句，而不能对数据库对象进行修改。如果需要修改数据对象，又
 		 *要保证数据的一致性，需要执行SQL语句锁定该数据对象。）
         */
        void beginTrans(BOOL bIsReadOnly = FALSE);

        /*
         *提交一个事务处理。
         */
        void commit();        
       
        /*
         *回滚一个事务处理。
         */
        void rollback();  	

 
		/*
		 * 检查到服务器的连接是否正在工作。如果它关闭了，自动尝试一个再连接。
         * 如果空闲很长时间不使用数据库，数据库服务器可能将连接关闭。
		 * 所以在长时间运行时，需要检查服务器是否关闭了连接并且如有必要重新连接。 
		 * 在UniFrame中，此函数由Kernel自动调用，上层应用无须调用此函数。
		 
		 * return: TRUE: 连接正常或者重连成功； FALSE: 连接关闭且重连失败
		 */
		virtual BOOL ping();

   const char* type() { return "MYSQL"; }
private:
	    INT reconnect(); //重连数据库

   	    MYSQL m_struMYSQL;		
		struct DBInfo
		{
			CHAR DBName[256];
			CHAR DBAddr[256];
			INT	 DBPort;
		};
		typedef struct DBInfo DBInfo;
		//主机地址和端口从配置文件中读取。
		//与ORACLE的区别在于，在ORACLE中，这些信息在ORACLE配置文件tnsnames中进行配置
		//应用程序可以忽略。
		//改动思路是，在CDB初始化时，将数据库地址和端口信息读到内存缓冲区中（m_struDBInfo）。
		//连接时，根据数据库名称，从内存缓冲区中获取其对应的地址和端口
		DBInfo  m_struDBInfo;		
		
		//配置文件中数据库的数目。
		INT m_iDBCnt;
		
		MYSQL_RES * m_pMysqlRes; 
		
		BOOL  alreadyInit; //连接过一次后，此值置为TRUE.以便能调用reconnect
        
	 
};

#endif
