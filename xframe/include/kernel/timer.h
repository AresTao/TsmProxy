#ifndef __XFRAME_TIMER_H_
#define __XFRAME_TIMER_H_

#include <iostream>

#include "comtypedef.h"
#include "func.h"

class TTimer
{
   public:

      TTimer(UINT timerMark, UINT timerKey, UINT timerDelay, void* timerPara);

      ~TTimer(){}

      UINT64 getWhen() const {return mWhen;}
	  UINT   getTimerKey() const {return mTimerKey;}

      std::ostream& encode(std::ostream& str) const;

      inline bool operator<(const TTimer& rhs) const
      {
         return mWhen < rhs.mWhen;
      }

      inline bool operator>(const TTimer& rhs) const
      {
         return mWhen > rhs.mWhen;
      }

      void*  mPara;
      UINT64 mWhen;
      UINT   mTimerMark;
      UINT   mTimerKey;
      UINT   mTimerDelay;

};


inline std::ostream& operator<<(std::ostream& str, const TTimer& t)
{
   return t.encode(str);
}


#endif
