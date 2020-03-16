#ifndef _THREAD_H
#define _THREAD_H

#include "ThreadClass.h"

extern int B[1000][1000];

class Thread3 : public Thread
{
 public:
  Thread3( int i, int j );

 private:
  void ThreadFunc();
  int iIndex;
  int jIndex;
};

#endif
