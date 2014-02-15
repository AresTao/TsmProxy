// the code of this file is copy from resip

#ifndef _FDSET_H
#define _FDSET_H

#include <assert.h>
#include "comtypedef_vchar.h"

class CFdSet
{
public:
   CFdSet() : size(0), numReady(0)
   {
      FD_ZERO(&read);
      FD_ZERO(&write);
      FD_ZERO(&except);
   }

   int select(struct timeval& tv)
   {
      return numReady = ::select(size, &read, &write, &except, &tv);
   }

   int selectMilliSeconds(unsigned long ms)
   {
      struct timeval tv;
      tv.tv_sec = (ms/1000);
      tv.tv_usec = (ms%1000)*1000;
      return select(tv);
   }

   int selectMicroSeconds(unsigned long ms)
   {
      struct timeval tv;
      tv.tv_sec = (ms/1000000);
      tv.tv_usec = (ms%1000000);
      return select(tv);
   }

   bool readyToRead(int fd)
   {
      return (FD_ISSET(fd, &read) != 0);
   }

   bool readyToWrite(int fd)
   {
      return (FD_ISSET(fd, &write) != 0);
   }

   bool hasException(int fd)
   {
      return (FD_ISSET(fd,&except) != 0);
   }

   void setRead(int fd)
   {
      assert( FD_SETSIZE >= 8 );
#ifndef WIN32 // windows fd are not int's and don't start at 0 - this won't work in windows
      assert( fd < (int)FD_SETSIZE ); // redefineing FD_SETSIZE will not work 
#else
      assert(read.fd_count < FD_SETSIZE); // Ensure there is room to add new FD
#endif
      FD_SET(fd, &read);
      size = ( int(fd+1) > size) ? int(fd+1) : size;
   }

   void setWrite(int fd)
   {
#ifndef WIN32 // windows fd are not int's and don't start at 0 - this won't work in windows
      assert( fd < (int)FD_SETSIZE ); // redefinitn FD_SETSIZE will not work 
#else
      assert(write.fd_count < FD_SETSIZE); // Ensure there is room to add new FD
#endif
      FD_SET(fd, &write);
      size = ( int(fd+1) > size) ? int(fd+1) : size;
   }

   void setExcept(int fd)
   {
#ifndef WIN32 // windows fd are not int's and don't start at 0 - this won't work in windows
      assert( fd < (int)FD_SETSIZE ); // redefinitn FD_SETSIZE will not work 
#else
      assert(except.fd_count < FD_SETSIZE); // Ensure there is room to add new FD
#endif
      FD_SET(fd,&except);
      size = ( int(fd+1) > size) ? int(fd+1) : size;
   }


   void clear(int fd)
   {
      FD_CLR(fd, &read);
      FD_CLR(fd, &write);
      FD_CLR(fd, &except);
   }

   void reset()
   {
      size = 0;
      numReady = 0;
      FD_ZERO(&read);
      FD_ZERO(&write);
      FD_ZERO(&except);
   }

   // Make this stuff public for async dns/ares to use
   fd_set read;
   fd_set write;
   fd_set except;
   int size;
   int numReady;  // set after each select call

   int count1(fd_set* fdSet)
   {
      int count = 0;
      int i;
      for(i=0; i<size; i++)
      {
         if(FD_ISSET(i, fdSet))
            count++;
      }
      return count;
   }
   int count()
   {
      return count1(&read) + count1(&write) + count1(&except);
   }
   void printState1(fd_set* fdSet)
   {
      int i;
      for(i=0; i<size; i++)
      {
         if(FD_ISSET(i, fdSet))
            printf("%d ", i);
      }
   }
   void printState()
   {
      int i;
      if(count1(&read))
      {
         printf("read[ ");
         printState1(&read);
         printf("]\n");
      }
      if(count1(&write))
      {
         printf("write[ ");
         printState1(&write);
         printf("]\n");
      }
      if(count1(&except))
      {
         printf("except[ ");
         printState1(&except);
         printf("]\n");
      }
   }
};
#endif
