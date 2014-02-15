#include "testtask.h"
#include "framectrl.h"
#include "abstracttask.h"
#include "msgdef.h"
#include "db.h"

TestTask::TestTask(): TAbstractTask()
{
}
TAbstractTask* TestTask::clone()
{
    TAbstractTask * task = this;
    return task;
}

void TestTask:: procMsg(TUniNetMsg * msg)
{
    //打印消息
    TAbstractTask::procMsg(msg);
    int result = getDB(1)->execSQL("select * from user"); //test for get db
    TUniNetMsg * newMsg = new TUniNetMsg();
    newMsg->tAddr = msg->oAddr;
    sendMsg(newMsg);
    sleep(1);
}
