
#include "env.h"
#include "info.h"
#include "db.h"


int LogLevelFactory(const char * Level)
{
    if(strcmp(Level,"Debug")==0)
        return Debug;
    else if(strcmp(Level,"Info")==0)
        return Info;
    else if(strcmp(Level,"Error")==0)
        return Err;
}
int LogTypeFactory(const char * type)
{
    if(strcmp(type,"Cout")==0)
        return Cout;
    else if(strcmp(type,"File")==0)
        return File;
    else if(strcmp(type,"Both")==0)
        return Both;
}

int TaskTypeFactory(const char * type)
{
	if(strcmp(type, "psa")==0)
		return objtype_Psa;
	else if(strcmp(type, "task")==0)
		return objtype_Task;
	else if(strcmp(type, "extend")==0)
		return objtype_Extended;
}


void TTimerRsc::list(CStr& result)
{
	result<<"Timer List:\n";
	for(int i=0;i<__TASK_TIMER_NUM;i++)
	{
		if(mTimerMarkList[i]>0)
		{
			result.fCat(" timerID = %d,", mTimerList[mTimerMarkList[i]].timerId);
			result  <<  " timerName =" << mTimerList[mTimerMarkList[i]].timerName <<",";
			result.fCat(" timerDelay = %d,",mTimerList[mTimerMarkList[i]].timerDelay);
			result.fCat(" resendNum = %d\n", mTimerList[mTimerMarkList[i]].resendNum);
		}
	}
}

TTimeMarkExt& TTimerRsc::operator[](const TTimeMark timerId)
{
	if(timerId>=0 && timerId< __TASK_TIMER_NUM)
		return mTimerList[timerId];
	else
		return mTimerList[0];
}

inline void TTimerRsc::clear()
{
	for(int i=0;i<__TASK_TIMER_NUM; i++)
	{
		mTimerList[i].timerId=0;
		mTimerList[i].timerDelay=0;
		mTimerList[i].resendNum=0;
		mTimerList[i].timerName[0]=0;

		mTimerMarkList[i]=0;
	}
}


TDBEnv::TDBEnv()
{
	clear();
}

TDBEnv::~TDBEnv()
{
	clear();
}

TDBMarkExt& TDBEnv::operator[](const INT dbID)
{
	if(dbID>=0 && dbID< __TASK_DB_NUM)
		return mDBList[dbID];
	else
		return mDBList[0];
}

inline void TDBEnv::clear()
{
	for(int i=0;i<__TASK_DB_NUM;i++)
	{
		mDBList[i].dbId=0;
		mDBList[i].dbType=0;
		mDBList[i].dbPort=0;
		mDBList[i].dbUser.clear();
		mDBList[i].dbPass.clear();
		mDBList[i].dbName.clear();
		mDBList[i].dbHost.clear();

		mDBMarkList[i]=0;
	}
}

UINT TDBEnv::load(TiXmlElement* dblistnode)
{
	if(dblistnode==NULL)
	{
		UniERROR("TDBEnv: error load db config!(get db config)");
		return 0;
	}

	clear();

	TiXmlElement*	db=NULL;
	INT    			id=0;
	INT				pos=0;

	TiXmlHandle handle(dblistnode);
	db=handle.FirstChild("db").Element();
	while(db)
	{
		if(!db->Attribute("dbID", &id)) id=0;
		if(id>0 && id<__TASK_DB_NUM)
		{
			if(mDBList[id].dbId>0)
			{
				UniERROR("TDBEnv: Duplicate db config, db id=%d, dbType=%d, dbName=%s, dbUser=%s, dbHost=%s, dbPort=%d!", id, mDBList[id].dbType, mDBList[id].dbName.c_str(), mDBList[id].dbUser.c_str(), mDBList[id].dbHost.c_str(), mDBList[id].dbPort);
			}
			else
			{
				mDBList[id].dbId=id;
				if(!db->Attribute("dbType", &(mDBList[id].dbType))) mDBList[id].dbType=0;
				mDBList[id].dbUser=db->Attribute("dbUser");
				mDBList[id].dbPass=db->Attribute("dbPass");
				mDBList[id].dbName=db->Attribute("dbName");
				mDBList[id].dbHost=db->Attribute("dbHost");
				if(!db->Attribute("dbPort", &(mDBList[id].dbPort))) mDBList[id].dbPort=0;

				mDBMarkList[pos]=id;
				pos++;
			}
		}
		else
		{
			UniERROR("TDBEnv: Error db config, db id=%d!", id);
		}

		db=db->NextSiblingElement();
	}

	return pos;
}

void TDBEnv::list(CStr& result)
{
	result << "DBList: \n";

	for(int i=0;i<__TASK_DB_NUM;i++)
	{
		if(mDBMarkList[i]>0)
		{
			result.fCat(" dbId     = %d \n", mDBList[mDBMarkList[i]].dbId);
			result.fCat("   dbType = %d \n", mDBList[mDBMarkList[i]].dbType);
			result  <<  "   dbUser = "<<mDBList[mDBMarkList[i]].dbUser<<"\n";
			result  <<  "   dbPass = "<<mDBList[mDBMarkList[i]].dbPass<<"\n";
			result  <<  "   dbName = "<<mDBList[mDBMarkList[i]].dbName<<"\n";
			result  <<  "   dbHost = "<<mDBList[mDBMarkList[i]].dbHost<<"\n";
			result.fCat("   dbPort = %d\n", mDBList[mDBMarkList[i]].dbPort);
		}
	}
}

TDBRsc::TDBRsc()
{
	for(int i=0;i<__TASK_DB_CONN_NUM;i++)
	{
		mDBList[i].dbId=0;
		mpDB[i]=NULL;
	}
}

TDBRsc::~TDBRsc()
{
	clear();
}

inline void TDBRsc::clear()
{
	for(int i=0;i<__TASK_DB_CONN_NUM;i++)
	{
		mDBList[i].dbId=0;
		if(mpDB[i]!=NULL)
		{
			mpDB[i]->disConnDB();
			delete mpDB[i];
			mpDB[i]=NULL;
		}
	}
}

CDB* TDBRsc::operator[](const INT dbID)
{
	if(dbID>=0 && dbID< __TASK_DB_CONN_NUM)
		return mpDB[dbID];
	else
		return NULL;
}

BOOL  TDBRsc::conn(const INT dbidx)
{
	if(dbidx>0 && mDBList[dbidx].dbId>0)
	{
		if(mpDB[dbidx]!=NULL)
		{
			mpDB[dbidx]->disConnDB();
			delete mpDB[dbidx];
			mpDB[dbidx]=NULL;
        }
        mpDB[dbidx]=initDB((CDB::DBType)mDBList[dbidx].dbType, mDBList[dbidx].dbUser.c_str(), mDBList[dbidx].dbPass.c_str(), mDBList[dbidx].dbName.c_str(),mDBList[dbidx].dbHost.c_str(), mDBList[dbidx].dbPort);
        if(mpDB[dbidx]!=NULL)
        {
            UniINFO("TDBRsc: New db rsc ok, dbID=%d", mDBList[dbidx].dbId);
            return TRUE;
        }
        else
        {
            UniINFO("TDBRsc: New db rsc error, dbID=%d", mDBList[dbidx].dbId);
        }
        return TRUE;
	}
	return FALSE;
}

BOOL  TDBRsc::conn()
{
	for(int i=1;i<__TASK_DB_CONN_NUM;i++)
	{
		conn(i);
	}
	return TRUE;
}

BOOL  TDBRsc::disconn(const INT dbidx)
{
	if(dbidx>0 && mDBList[dbidx].dbId>0)
	{
		if(mpDB[dbidx]!=NULL)
		{
			mpDB[dbidx]->disConnDB();
			delete mpDB[dbidx];
			mpDB[dbidx]=NULL;
		}
		return TRUE;
	}
	return FALSE;
}


BOOL TTaskEnv::onLoad()
{
	UniINFO("Try to load Task env....");
	TiXmlHandle handle(root);
	TiXmlElement* tasklist=NULL;
	tasklist=handle.FirstChild("tasklist").Element();
	if(tasklist)
	{
		TiXmlHandle tlhandle(tasklist);
		task=NULL;
		INT taskID=0;
		task=tlhandle.FirstChild("task").Element();
		while(task)
		{
			if(!task->Attribute("taskID", &taskID)) taskID=0;
			if(taskID==mTaskID)
			{
				TiXmlHandle taskhandle(task);
				log=NULL;
				timerrsc=NULL;
				dbrsc=NULL;
				extend=NULL;
				log=taskhandle.FirstChild("log").Element();
				timerrsc=taskhandle.FirstChild("timerrsc").Element();
				dbrsc=taskhandle.FirstChild("dbrsc").Element();
				extend=taskhandle.FirstChild("extend").Element();
				return TRUE;
			}
			task=task->NextSiblingElement();
		}
	}

	return FALSE;
}

void TTaskEnv::onList(CStr& result)
{
	result	<<	"TTaskEnv env:\n";
	result.fCat("  logType		   = %d\n", getLogType());
	result.fCat("  logLevel 	   = %d\n", getLogLevel());
	CStr taskName;
	INT taskType;
	getTaskInfo(taskType, taskName);
	result	<<	" Task env:\n";
	result.fCat("  TaskID 		   = %d\n", mTaskID);
	result.fCat("  TaskType		   = %d\n", taskType);
	result.fCat("  TaskName		   = %s\n", taskName.c_str());



}


int TTaskEnv::getLogType()
{
	if(log)
	{
		const char * tmp;
		if(tmp= log->Attribute("logType")) mLogType = LogTypeFactory(tmp);
	}
	return mLogType;

}


int TTaskEnv::getLogLevel()
{
	if(log)
	{
		const char * tmp;
		if(tmp = log->Attribute("logLevel")) mLogLevel = LogLevelFactory(tmp);
	}
	return mLogLevel;
}

BOOL TTaskEnv::getTaskInfo(INT& taskType, CStr& taskName)
{
	if(!task) return FALSE;
	taskName=task->Attribute("taskName");
	const char * tmp;
	if(tmp = task->Attribute("taskType")) taskType=TaskTypeFactory(tmp);
	else taskType=TaskTypeFactory("task");
	return TRUE;
}


BOOL TTaskEnv::getDBRsc(TDBRsc& dbinfo)
{
	dbinfo.clear();

	TiXmlElement*	db=NULL;
	INT    			id=0;
	INT				dbID=0;

	TiXmlHandle handle(dbrsc);
	db=handle.FirstChild("db").Element();
	while(db)
	{
		if(!db->Attribute("index", &id)) id=0;
		if(!db->Attribute("dbID", &dbID)) dbID=0;

		if(id>0 && id<__TASK_DB_CONN_NUM && dbID>0 )
		{
			dbinfo.mDBList[id]=mDBEnv[dbID];
			if(dbinfo.mDBList[id].dbId>0)
			{
				UniINFO("TDBRsc: load db rsc ok, dbindex=%d, dbID=%d", id, dbID);
			}
		}
		db = db->NextSiblingElement();
	}
    return true;
}


BOOL TKernalEnv::onLoad()
{
	UniINFO("Try to load Kernal env...");
	TiXmlHandle handle(root);
	TiXmlElement* kernal=NULL;
	kernal=handle.FirstChild("kernal").Element();
	if(kernal)
	{
		if(!kernal->Attribute("appID", &mAppID)) mAppID=0;
		mAppName=kernal->Attribute("appName");
		mHostIP=kernal->Attribute("hostIP");

		TiXmlHandle kernalhandle(kernal);
		sc=NULL;
		sc=kernalhandle.FirstChild("sc").Element();
		proclist=NULL;
		proclist=kernalhandle.FirstChild("process").Element();
		log=NULL;
		log=kernalhandle.FirstChild("log").Element();

		timerrsc=NULL;
		timerrsc=kernalhandle.FirstChild("timerrsc").Element();
	}
	tasklist=NULL;
	tasklist=handle.FirstChild("tasklist").Element();

	return TRUE;
}


void TKernalEnv::onList(CStr& result)
{
   result  <<  "TKernalEnv env:\n";
   result.fCat("  logType         = %d\n", getLogType());
   result.fCat("  logLevel        = %d\n", getLogLevel());
   CStr scip;
   UINT scport;
   UINT hb;
   getSCInfo( scip, scport, hb);
   result  <<  " SC env:\n";
   result.fCat("  SCIP            = %s\n", scip.c_str());
   result.fCat("  SCPORT          = %d\n", scport);
   result.fCat("  HB              = %d\n", hb);
   CList<TProcRsc> list;
   getProcList(list);
   TProcRsc p;
   result  <<  " Process env:\n";
   for(int i=0; i<list.size(); i++)
   {
	list.pop_front(p);
   		result.fCat("  TaskIP = %d ; TaskName = %s ; TaskType = %d ; ThreadNum = %d ; TaskDll = %s \n", p.taskID, p.taskName.c_str(), p.taskType, p.threadNum, p.taskDll.c_str());
   }
}

int TKernalEnv::getLogType()
{
	if(log)
	{
		const char * tmp;
		if(tmp= log->Attribute("logType")) mLogType = LogTypeFactory(tmp);
	}
	return mLogType;
}

int TKernalEnv::getLogLevel()
{
	if(log)
	{
		const char * tmp;
		if(tmp = log->Attribute("logLevel")) mLogLevel = LogLevelFactory(tmp);
	}
	return mLogLevel;
}

BOOL TKernalEnv::getProcList(CList<TProcRsc>& list)
{
	if(tasklist==NULL || proclist==NULL) return FALSE;
	CHashTable<UINT, TProcRsc> tl;
	INT    id;
	TiXmlHandle handle(tasklist);
	TiXmlElement *task=NULL;
	task=handle.FirstChild("task").Element();
	while(task)
	{
		if(!task->Attribute("taskID", &id)) id=0;
		if(id>0)
		{
			TProcRsc p;
			p.taskID=id;
			p.taskName=task->Attribute("taskName");
			const char * tmp;
			if(tmp = task->Attribute("taskType")) p.taskType=TaskTypeFactory(tmp);
			else p.taskType=TaskTypeFactory("task");
			p.taskDll=task->Attribute("taskDll");

			tl.put(id,p);
		}
		task=task->NextSiblingElement();
	}

	TiXmlHandle handle2(proclist);
	TiXmlElement *proc=NULL;
	proc=handle2.FirstChild("task").Element();
	while(proc)
	{
		if(!proc->Attribute("taskID", &id)) id=0;
		if(id>0)
		{
			TProcRsc p;
			tl.get(id,p);
			if(!proc->Attribute("threadNum", &p.threadNum)) p.threadNum=1;

			list.push_back(p);
		}
		proc=proc->NextSiblingElement();
	}
	return TRUE;

}


BOOL TKernalEnv::getSCInfo(CStr& scip, UINT& scport, UINT& hb)
{
	if(sc==NULL) return FALSE;
	if(sc->Attribute("scIP") != NULL)
		scip=sc->Attribute("scIP");
	if(!sc->Attribute("scPort", (int *)&scport)) scport=9000;
	if(!sc->Attribute("hbInterval", (int *)&hb)) hb=10000;
	return TRUE;
}


TEnv::TEnv()
{
	root=NULL;
	config=NULL;

	mLogType=Cout;
	mLogLevel=Debug;
	mThreadName="";
}

TEnv::~TEnv()
{
	if(config!=NULL) delete config;
}

UINT TEnv::load()
{
	char* uniDir=getenv("UNIDIR");
	if(NULL==uniDir)
		uniDir=".";

	CStr mConfigFile;
	mConfigFile<<uniDir;
	mConfigFile<<"/etc/";
	if(appName)
	{
		mConfigFile<<appName;
		mConfigFile<<"_config.xml";
	}
	else
	{
		mConfigFile<<"config.xml";
	}

	int ret=0;

	config=new TiXmlDocument(mConfigFile.c_str());      //配置文件对象
	if(config==NULL || !config->LoadFile())
	{
		UniERROR("TEnv: can not open config file: %s. try load default...\n", mConfigFile.c_str());
		if(config) delete config;

		config=NULL;
		mConfigFile.clear();
		mConfigFile<<uniDir;
		mConfigFile<<"/etc/config.xml";
		config=new TiXmlDocument(mConfigFile.c_str());
		if(config==NULL || !config->LoadFile())
		{
			UniERROR("TEnv: can not open config file: %s....\n", mConfigFile.c_str());
			return FALSE;
		}
	}
	UniINFO("TEnv: Open config file %s ...",  mConfigFile.c_str());

	root = NULL;
	root=config->RootElement();
	if(root==NULL)
	{
		UniERROR("TEnv: no information in file %s", mConfigFile.c_str());
		return FALSE;
	}
	UniINFO(" Try to load common config...");
	if(!loadCommonEnv(root)) return FALSE;

	if(!onLoad()) return FALSE;

	return TRUE;
}

void TEnv::list(CStr& result)
{
   result  <<  "TEnv env:\n";
   result.fCat("  logType         = %d\n", mLogType);
   result.fCat("  logLevel        = %d\n", mLogLevel);
   result  <<  " TDBEnv env:\n";
   mDBEnv.list(result);

   onList(result);
}

BOOL TEnv::loadCommonEnv(TiXmlElement* root)
{
	TiXmlHandle handle(root);
	TiXmlElement* common=NULL;
	common=handle.FirstChild("common").Element();
	if(common)
	{
		TiXmlHandle comhandle(common);

		TiXmlElement* log=NULL;
		log=comhandle.FirstChild("log").Element();
		if(log)
		{
		    const char * tmp;
            if(tmp= log->Attribute("logType")) mLogType = LogTypeFactory(tmp);
            if(tmp = log->Attribute("logLevel")) mLogLevel = LogLevelFactory(tmp);
		}

		TiXmlElement* dblist=NULL;
		dblist=comhandle.FirstChild("dblist").Element();
		if(dblist)
			mDBEnv.load(dblist);

	}
//
//	CStr result;
//	list(result);
//	UniINFO("TEnv: Load common env ok:\n %s", result.c_str());
	return TRUE;
}


BOOL TEnv::getTimerRsc(TTimerRsc& tm)
{
	tm.clear();

	TTimeMark	 id=0;
	INT 		 delay=0;
	INT 		 pos=0;

	if(timerrsc==NULL) return FALSE;

	TiXmlHandle handle(timerrsc);
	TiXmlElement *timer=handle.FirstChild("timer").Element();
	while(timer)
	{
		if(!timer->Attribute("id", (int *)&id)) id=0;
		if(!timer->Attribute("delay", &delay)) delay=0;
		if(id>0 && id< __TASK_TIMER_NUM && delay>0 )
		{
			if(tm.mTimerList[id].timerId>0)
			{
				UniERROR("TTaskEnv: Duplicate timer config, timer id=%d, delay=%d!", id, delay);
			}
			else
			{
				tm.mTimerList[id].timerId=id;
				tm.mTimerList[id].timerDelay=delay;
				if(!timer->Attribute("resendnum", &(tm.mTimerList[id].resendNum))) tm.mTimerList[id].resendNum=0;
				memset(tm.mTimerList[id].timerName, 0, MAX_TIMER_NAME);
				if(timer->Attribute("name"))
				strncpy(tm.mTimerList[id].timerName, timer->Attribute("name"), MAX_TIMER_NAME-1);
				tm.mTimerMarkList[pos]=id;
				pos++;
			}
		}
		else
		{
			UniERROR("TTaskEnv: Error timer config, timer id=%d, delay=%d!", id, delay);
		}

		timer=timer->NextSiblingElement();
	}

	return pos;
}




