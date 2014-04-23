#include "sipdialogmng.h"

BOOL CSIPPsaDialog::QueryDialogTab(CStr& dialogid, CSIPPsaDialogInfo &dialoginfo)
{
	CStr did;
	if(mDialogMap_B.find(dialogid, did))
		return mDialogMap_F.find(did, dialoginfo);
	else
		return mDialogMap_F.find(dialogid, dialoginfo);
}

BOOL CSIPPsaDialog::ExistDialogTab(CStr& dialogid) 
{ 
	if(mDialogMap_B.find(dialogid)) return TRUE;
	else return mDialogMap_F.find(dialogid);
}


BOOL CSIPPsaDialog::InsertDialogTab(CStr& dialogid_F, CStr& dialogid_B, TMsgAddress& addr)
{
	int error1;
	int error2;
	
	CSIPPsaDialogInfo info;
	info.mDialogID_F=dialogid_F;
	info.mDialogID_B=dialogid_B;
	info.mAddr=addr;
	
	error1 = mDialogMap_F.push(dialogid_F, info);
	error2 = mDialogMap_B.push(dialogid_B, dialogid_F);
	if(error1 && error2) return TRUE;
	else return FALSE;
}

BOOL CSIPPsaDialog::InsertDialogTab(CStr& dialogid_F, CStr& dialogid_B)
{
	TMsgAddress addr;
	return InsertDialogTab(dialogid_F, dialogid_B, addr);
}

void CSIPPsaDialog::RemoveDialogTab(CStr& dialogid)
{
	CSIPPsaDialogInfo dialoginfo;
	CStr did;
	if(mDialogMap_F.pop(dialogid, dialoginfo))
	{
		mDialogMap_B.pop(dialoginfo.mDialogID_B);
	}
	else if(mDialogMap_B.pop(dialogid, did))
	{
		mDialogMap_F.pop(did);
	}
}

BOOL CSIPPsaDialog::UpdateDialogAddr(CStr& dialogid, TMsgAddress& tAddr)
{
	if(mDialogMap_F.find(dialogid)) {
		mDialogMap_F[dialogid].setDialogtAddr(tAddr);
	}
	else if (mDialogMap_B.find(dialogid)) 
	{
		mDialogMap_F[mDialogMap_B[dialogid]].setDialogtAddr(tAddr);
	}
	else
		return FALSE;
	return TRUE;
}


BOOL CSIPPsaDialog::UpdateDialogExpire(CStr& dialogid, time_t currenttime)
{
	CSIPPsaDialogInfo dialoginfo;
	CStr did;
	if(mDialogMap_F.find(dialogid, dialoginfo))
	{
		mDialogMap_F.update(dialogid,currenttime);
		mDialogMap_B.update(dialoginfo.mDialogID_B,currenttime); 
	}
	else if(mDialogMap_B.find(dialogid, did))
	{
		mDialogMap_F.update(did,currenttime);
		mDialogMap_B.update(dialogid,currenttime); 
	}
	else return FALSE;	
	return TRUE;
}

INT CSIPPsaDialog::Size() 
{
	return mDialogMap_F.count();
}


INT  CSIPPsaDialog::CleanExpireDialog(time_t currenttime)
{
	mDialogMap_B.pop_old(currenttime);
	return mDialogMap_F.pop_old(currenttime);
}

