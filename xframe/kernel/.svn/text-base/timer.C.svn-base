#include "timer.h"

TTimer::TTimer(UINT timerMark, UINT timerKey, UINT timerDelay, void* timerPara) :
   mWhen(timerDelay + getTimeMs()),
   mTimerKey(timerKey),
   mTimerMark(timerMark),
   mTimerDelay(timerDelay),
   mPara(timerPara)
{}

std::ostream& 
TTimer::encode(std::ostream& str) const
{
   UINT64 now(getTimeMs());
   str << "TTimer[ when=" << mWhen << " rel=";
   if (mWhen < now)
   {
      str << "past";
   }
   else
   {
      str << (mWhen - now);
   }
   str << "]";
   return str;
}
