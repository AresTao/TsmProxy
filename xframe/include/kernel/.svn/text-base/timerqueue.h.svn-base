#ifndef __XFRAME_TIMERQUEUE_H_
#define __XFRAME_TIMERQUEUE_H_


#include <functional>
#include <queue>
#include <set>
#include <iosfwd>

#include "comtypedef.h"
#include "fifo.h"
#include "unihashtable.h"
#include "func.h"
#include "msg.h"
#include "timer.h"

const unsigned int XFRAME_TIMER_STEP_MAX=1000;  //1000 ms  
  
template <class T>
class TimerQueue
{
   public:
      virtual void processTimer(const T& timer)=0;
      virtual ~TimerQueue()
      {
         while (!mTimers.empty())
         {
         	if(mTimers.top().mPara!=NULL) delete mTimers.top().mPara;
            mTimers.pop();
         }
      }

      unsigned int msTillNextTimer()
      {
         if (!mTimers.empty())
         {
            UINT64 next = mTimers.top().getWhen();
            UINT64 now = getTimeMs();
            if (now > next) 
            {
               return 0;
            }
            else
            {
               UINT64 ret64 = next - now;
               if ( ret64 > UINT64(INT_MAX) )
               {
                  return XFRAME_TIMER_STEP_MAX;
               }
               else
               { 
                  int ret = int(ret64);
                  return ret;
               }
            }
         }
         else
         {
            return XFRAME_TIMER_STEP_MAX; 
         }
      }

      /// @brief gets the set of timers that have fired, and inserts TimerMsg into the state
      /// machine fifo and application messages into the TU fifo
      virtual unsigned int  process()
      {
         if (!mTimers.empty())
         {
            UINT64 now=getTimeMs();
            while (!mTimers.empty() && !(mTimers.top().getWhen() > now))
            {
               processTimer(mTimers.top());
               mTimers.pop();
            }

           // if(!mTimers.empty())
           // {
           //    //return mTimers.top().getWhen();
           //    return msTillNextTimer();
           // }
         }
         //return INT_MAX;
         return msTillNextTimer();
      }

      int size() const
      {
         return (int)mTimers.size();
      }

      bool empty() const
      {
         return mTimers.empty();
      }

      std::ostream& encode(std::ostream& str) const
      {
         if(mTimers.size() > 0)
         {
            return str << "TimerQueue[ size =" << mTimers.size() 
                       << " top=" << mTimers.top() << "]" ;
         }
         else
         {
            return str << "TimerQueue[ size = 0 ]";
         }
      }

   protected:
      typedef std::vector<T, std::allocator<T> > TimerVector;
      std::priority_queue<T, TimerVector, std::greater<T> > mTimers;
};

template <class T>
std::ostream& operator<<(std::ostream& str, const TimerQueue<T>& tq)
{
   return tq.encode(str);
}


class TaskTimerQueue : public TimerQueue<TTimer>
{
   public:
      TaskTimerQueue(Fifo<TMsg>& fifo);

	  //返回TimerKey ，返回0 代表添加失败
      UINT add(UINT timerMark, UINT timerDelay, void* timerPara);
	  void remove(UINT key);
      void stop(UINT key);
      virtual void processTimer(const TTimer& timer);
   private:
   	  UINT mTimerKey;		//用无符号整数做timerkey 应该够用了
      Fifo<TMsg>& mFifo;
	  CHashTable<UINT, BOOL> mTimerStoped;
};


#endif
