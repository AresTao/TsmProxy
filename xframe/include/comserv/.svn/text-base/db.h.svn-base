/*************************************************************************

 * Copyright (c)2005-2008, by Beijing TeleStar Network Technology Company Ltd.(MT2)
 * All rights reserved.

 * FileName：       db.h
 * System：         UniFrame
 * SubSystem：      Facility-DB
 * Author：         Long Xiangming
 * Date：           2005.10.06
 * Version：        1.0
 * Description：
    CDB定义；数据库操作定义。
	完成数据库操作。

 *
 * Last Modified:
     2005.10.06, 完成初始版本
        By Long Xiangming
	2011-2-12，修订，删掉单例，支持多实例; 增加mConnectionID参数，区分每一个建立的连接
		By Li jinglin
**************************************************************************/
#ifndef __DB_H
#define __DB_H

#include "comtypedef.h"
#include "info.h"

const INT  MaxSQLLength			= 8192;

#define DEFAULT_DB			(CHAR*)"unidb"//缺省数据库SID
#define MAX_ITEMS         	100			//在查询语句中可以一次查询的最多列数
#define MAX_VNAME_LEN     	4096		//每个字段的最大bytes
#define MAX_INAME_LEN     	4096		//选择描述符的最大bytes
#define NULLBASE			10			//GetIntoPara()方法当取出空值时，返回值相加的基数；
										//例如，如果有4个空值，那么返回值就为NULLBASE+4;
#define MAX_STMT_LEN		MaxSQLLength	//限制SQL语句的最大长度。
#define MAX_UID_LEN			128			//数据库用户名的最大bytes
#define MAX_DBSV_LEN		128			//数据库服务名的最大bytes
#define MAX_PWD_LEN			32			//数据库口令的最大bytes
#define MAX_THREAD_LNK		3			//数据库连接最大线程数

/*
 *定义各种数据类型的最大值。在从数据库表中取出的数据
 *如果等于该数据类型的最大值时，认为改数据为空值。（
 *针对GetIntoPara()方法而言）
 */
#ifndef UINT_MAX
#define UINT_MAX			4294967295	//UINT类型数据的空值表示
#endif
#ifndef CHAR_MAX
#define CHAR_MAX			127			//CHAR类型数据的空值表示
#endif
#ifndef BYTE_MAX
#define BYTE_MAX			255			//BYTE类型数据的空值表示
#endif
#ifndef SHRT_MAX
#define SHRT_MAX			32767		//SHORT类型数据的空值表示
#endif
#ifndef USHRT_MAX
#define USHRT_MAX			65535		//USHORT类型数据的空值表示
#endif
#ifndef INT_MAX
#define INT_MAX				2147483647	//INT类型数据的空值表示
#endif
#ifndef FLT_MAX
#define FLT_MAX				3.402823466e+38F	//FLOAT类型数据的空值表示
#endif

enum TFieldDataType
{
   T_INT,
   T_UINT,
   T_LONGLONG,
   T_FLOAT,
   T_STRING
};

typedef struct Field
{
	TFieldDataType	type;
	BOOL    		isNull;
	union
	{
		I32			    intValue;
		UINT            uintValue;
		I64 			longlongValue;
		FLOAT           floatValue;
#define MaxStrAIDLength 4000
		char			stringValue[MaxStrAIDLength];
	}value;

	Field()
	{
		isNull = FALSE;
		value.intValue = 0;
		value.stringValue[0]=0; //added by lxm.2008.12.08
	}
}TField;

typedef struct FieldInfo
{
	char			 name[20];
	TFieldDataType	 type;
	int 			 len;
}TFieldInfo;

const I32 MaxFieldNum = 80;//20;
typedef struct Row
{
	struct Field	 arrayField[MaxFieldNum];
	struct Row		 *next;
}TRow;

_CLASSDEF(TSelectResult);
struct TSelectResult
{
	int 			 rowNum; //modify short to int. by Long Xiangming. 2007.11.16
	short  			 fieldNum;
	struct Row		 *pRows;
	struct FieldInfo fieldInfos[MaxFieldNum];
};

// SelectResult 与 slctRslt 功能同. 今后主要采用SelectResult.
// slctRslt只是为了与以前的软交换(UniNet 3.X 以前）兼容, 今后将逐渐废弃。

struct slctRslt
{
    void* pvResult;						//查询结果单元
    INT ResultLen;						//结果长度
    struct slctRslt* pstruNext;			//下一列
};

_CLASSDEF(CDB);
class CDB
{
 public:
 //move to comtypedef.h
		enum DBType
		{
			DB_MYSQL = 0,
			DB_ORACLE = 1,
			DB_SQLITE = 2
		};

		CDB(void);
		INT getConnectionID() {return mConnectionID;};

		virtual ~CDB(void);
		/*
		 *INT ConnDB(CHAR* uid,CHAR* pwd,CHAR* dbsv)
		 *CHAR* uid:指向用户名的字符指针;
		 *CHAR* psw:指向用户口令的字符指;
		 *CHAR* dbsv:所要连接的数据库服务名.
		*/
        virtual INT connDB(const CHAR* uid,const CHAR* pwd,const CHAR* dbsv = DEFAULT_DB)=0;
		//host: 数据库服务器主机地址； port: 数据库服务器端口号
		//此方法用于数据库服务器单独部署为一台机器时。
		//MySQL可能需要使用这种方式。(Oracle即使单独部署，通过配置文件db项即可指定为远程连接，无需在connDB中显式指定host)
		virtual INT connDB(const CHAR* uid,const CHAR* pwd,const CHAR* dbsv,const CHAR* host, INT port)=0;

		virtual void disConnDB(void)=0;

		/************************************************
		*INT ExecSQL(CHAR *pcSQLStmt)
		* CHAR* pcSQLStmt参数为指向所要执行的SQL语句字符串的指针
		*************************************************/
		virtual INT execSQL(const char *pcSQLStmt);
   virtual INT execSQL1(const char* pcSQLStmt)=0;
   virtual int execSql(const char* sql) { return execSQL((char*)sql); }
   virtual int existTable(const char* tableName) { return 1; };
   virtual int existTableField(const char* tableName, const char* fieldName) { return 1; };
   virtual int isSelectStmt(const char* sql) = 0;
   int count(const char* tableName, const char* condition = NULL);
   int selectInt(const char* sql, int& result);
   int selectStr(const char* sql, CStr& result);
   int selectIntList(const char* sql, CList<int>& result);
   int selectStrList(const char* sql, CList<CStr>& result);
		/********************************************
 		*INT GetNextRow(struct slctRslt** ppstruSlctRslt);
		*struct slctRslt** 输出参数,指向取出数据行的第一个数据单元的指针
		*************************************************/
		virtual INT getNextRow(struct slctRslt** ppstruSlctRslt)=0;

		/* 获取返回行的数量  */
		virtual INT getRowCount()=0;

		/* 获取返回值 */
		virtual PTSelectResult getSelectResult()=0;

		/* 获取执行结果的错误提示 */
        virtual CHAR* getErrMsg();

        /*
         *void FreeRowMem(struct slctRslt* pstruSlctRslt)
         *struct slctRslt* 指向要释放空间的行的行首结构的指针
         */
        virtual void freeRowMem(struct slctRslt* pstruSlctRslt);

        /*
         * INT GetIntoPara(CHAR* pcParaType,...);
         * CHAR* pcParaType 指明pcParaType后面所跟的参数类型.'s'为
		 * CHAR类型,'d'为*INT类型,'u'为*UINT类型,'f'为
		 * *FLOAT类型.例如,GetIntoPara("udsf",uPara,dPara,pcPara,
		 * fPara).串"udsf"说明后面的参数从左到右分别为*UINT类型,
		 * *INT类型,*CHAR类型和*float类型.
         */
        virtual INT getIntoPara(CHAR* pcParaType,...)=0;

        /*
         *开始一个事务处理。
         *bIsReadOnly为TRUE,表示该事务为只读事务。
         *只读事务可以保证在整个事务过程中数据保持一致性（其他进程
         *执行commit语句不会对该事务造成影响，但是该事务只能执行查询
         *语句，而不能对数据库对象进行修改。如果需要修改数据对象，又
 		 *要保证数据的一致性，需要执行SQL语句锁定该数据对象。）
         */
        virtual void beginTrans(BOOL bIsReadOnly = FALSE)=0;

        /*
         *提交一个事务处理。
         */
        virtual void commit()=0;

        /*
         *回滚一个事务处理。
         */
        virtual void rollback()=0;

 		/*
		 * 检查到服务器的连接是否正在工作。如果它关闭了，自动尝试一个再连接。

         * 如果空闲很长时间不使用数据库，数据库服务器可能将连接关闭。
		 * 所以在长时间运行时，需要检查服务器是否关闭了连接并且如有必要重新连接。
		 * 在UniFrame中，此函数由Kernel自动调用，上层应用无须调用此函数。

		 * return: TRUE: 连接正常或者重连成功； FALSE: 连接关闭且重连失败
		 */
		virtual BOOL ping()=0;
   virtual const char* type() = 0;
   virtual void printState(CStr& str, int indent);

   protected:
	    void freeResult(); 	//释放m_pSelectResult内pRows的内存
							//此方法在每次查询前被自动调用
		INT mConnectionID;
	    I32  m_iDBCnt;
		CHAR m_pcDBsv[256];
        CHAR m_pcUsr[256];
        CHAR m_pcPwd[256];

		CHAR m_pcSQLStmt[MaxSQLLength];
        INT  m_iProcRowCount;					//用于事务处理中累计处理行数
        CHAR m_pcErrMsg[255];					//用于保存出错信息
        BOOL m_bTransaction;					//标志是否为事务处理
        BOOL m_bConnFlag;						//标志数据库连接状态
		INT  m_iQueryFlag;

	    PTSelectResult m_pSelectResult;
		BOOL m_isFirstlyGetResult;              //每次查询后置true

   Field* selectOneField(const char* sql);
   int getFieldInt(Field* field, int& value);
};
/* 初始化数据库
*  输入参数：
*		DBType dbType
*		CHAR* dbUser  数据库用户名
*		CHAR* dbPasswd 数据库用户密码
*		CHAR* dbName   数据库名（sid：oracle）（sqlite是本地数据库前缀，真正的数据库名后面会为每个连接建立一个独立的库）
*		CHAR* dbHost，INT dbPort   数据库地址 + 端口（一般MySQL使用，Oracle需要安装客户端配置sid, SQLite是本地数据库）
*******************************************************/
extern CDB* initDB(CDB::DBType dbType, const CHAR* dbUser, const CHAR* dbPasswd, const CHAR* dbName, const CHAR* dbHost=NULL, INT dbPort=0);

#endif

