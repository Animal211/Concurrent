// -----------------------------------------------------------
// NAME : Jacob McNeilly                    User ID: jrmcneil
// DUE DATE : 11/01/2019
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//         This includes the class definitions nessessary
// for our thread3s.
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

#include "ThreadClass.h"

extern int B[1000][1000];

// -----------------------------------------------------------
// FUNCTION Thread3 class:
//     Each thread takes a input i and j for indexing into
// 2d array.
// PARAMETER USAGE :
//    index i, and j for finding index [i,j] in array
// FUNCTION CALLED :
//    sprintf(), write(), exit()
// -----------------------------------------------------------

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
