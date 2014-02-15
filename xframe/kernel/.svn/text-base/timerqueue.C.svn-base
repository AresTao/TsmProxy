
#include <cassert>
#include <limits.h>

#include "timerqueue.h"

using namespace std;


TaskTimerQueue::TaskTimerQueue(Fifo<TMsg>& fifo)
   : mFifo(fifo),mTimerKey(0)
{
}

UINT
TaskTimerQueue::add(UINT timerMark, UINT timerDelay, void* timerPara)
{
	mTimerKey=mTimerKey+1;
	if(!mTimerKey) mTimerKey=1;
	TTimer timer(timerMark, mTimerKey, timerDelay, timerPara);
	BOOL keyState=TRUE;
    mTimers.push(timer);
    mTimerStoped.put(mTimerKey, keyState);
    return mTimerKey;
}

void
TaskTimerQueue::stop(UINT key)
{
	BOOL keyState=FALSE;
	if(mTimerStoped.get(key,keyState))
	{
		if(keyState)
		{
			keyState=FALSE;
			mTimerStoped.put(key, keyState);
		}
	}
}

void TaskTimerQueue:: remove(UINT key){
    mTimerStoped.remove(key);
}
void
TaskTimerQueue::processTimer(const TTimer& timer)
{
	BOOL keyState=FALSE;
	//哈希表中有且定时器有效，代表曾经添加过该定时器，且未取消，则设置超时
	if(mTimerStoped.get(timer.getTimerKey(),keyState))
	{
		if(keyState)
		   mFifo.add(new TTimeOutMsg(timer.mTimerMark,
                              timer.mTimerKey,
                              timer.mTimerDelay,
                              timer.mPara));
		mTimerStoped.remove(timer.getTimerKey());
	}
	else
		if(timer.mPara!=NULL) delete timer.mPara;
}

