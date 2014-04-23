#ifndef _SIPSTACKPSA_DIALOG
#define _SIPSTACKPSA_DIALOG

#include "cexpirebuffer.h"
#include "cmd5.h"
#include <ctime>

#include "msgdatadef.h"
#include "msgdef_sip.h"

class CSIPPsaDialogInfo
{
public:
	CSIPPsaDialogInfo() {};
	CStr			mDialogID_F;	//正向ID
	CStr			mDialogID_B;	//逆向ID
	TMsgAddress 	mAddr;			//toAddr
	void setDialogtAddr(TMsgAddress& taddr) { mAddr = taddr;};
};

class CSIPPsaDialog
{
private:
	//限制的最大记录数量 0：默认没有限制
	//销毁记录的策略 默认0：不自动销毁过期记录；1：自动销毁过期记录，当到达最大记录数量的时候，从最老的开始销毁
	//生存时间，单位秒，默认172800秒；当最后一次刷新的时间超过这一时间长度（currenttime-refreshtime > m_living_time），则过期
	CExpireBuffer<CStr, CSIPPsaDialogInfo>  mDialogMap_F;
	CExpireBuffer<CStr, CStr> 				mDialogMap_B;

public:
	CSIPPsaDialog(unsigned int max_count=0, bool destroy_old=false, unsigned int living_time=172800)
	{
		setMaxCount(max_count);
		setLivingTime(destroy_old);
		setDestroyPolicy(living_time);
	};

	void setMaxCount(unsigned int max_count=0) { mDialogMap_F.setMaxCount(max_count);  mDialogMap_B.setMaxCount(max_count);};
	void setLivingTime(unsigned int living_time=172800) { mDialogMap_F.setLivingTime(living_time);  mDialogMap_B.setLivingTime(living_time);};
	void setDestroyPolicy(bool destroy_old=false) { mDialogMap_F.setDestroyPolicy(destroy_old); mDialogMap_B.setDestroyPolicy(destroy_old);};

	//缓冲表能够保存的最大条目数量
	int getMaxSize() {return mDialogMap_F.getMaxSize();};
	BOOL getDestoryPolicy() {return mDialogMap_F.getDestoryPolicy();};
	int getLivingTime() {return mDialogMap_F.getLivingTime();};
	
	//查询对话
	BOOL QueryDialogTab(CStr& dialogid, CSIPPsaDialogInfo &dialoginfo);
	//判断是否存在对话
	BOOL ExistDialogTab(CStr& dialogid);
	//删除对话
	void RemoveDialogTab(CStr& dialogid);
	//插入对话
	BOOL InsertDialogTab(CStr& dialogid_F, CStr& dialogid_B);
	BOOL InsertDialogTab(CStr& dialogid_F, CStr& dialogid_B, TMsgAddress& addr);
	//更新对话状态
	BOOL UpdateDialogAddr(CStr& dialogid, TMsgAddress& tAddr);
	//更新对话的生存时间，一般在接收到消息的时候使用
	BOOL UpdateDialogExpire(CStr& dialogid, time_t currenttime=0);
	//删除过期对象
	INT CleanExpireDialog(time_t currenttime=0);

	INT Size();
};

#endif
