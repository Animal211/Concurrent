// -----------------------------------------------------------
// NAME : Jacob McNeilly                    User ID: jrmcneil
// DUE DATE : 11/01/2019
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//         This file contains all the class implementations
// of our threads (named Thread3 for program 3).
// -----------------------------------------------------------

#include <stdio.h>
#include <math.h>
#include <cstring>
#include "thread.h"

/*Construct thread3*/
Thread3::Thread3(int i, int j)
{
  iIndex = i; //initialize the threads i and j indexes with passed params
  jIndex = j;
  char buf[1000];
  sprintf(buf,"      Thread %d Created\n", j);
  write(1,buf,strlen(buf));
}

// -----------------------------------------------------------
// FUNCTION Thread3 Thread func():
//     Compute the value of a given spot in our array given index i
// and index j
// PARAMETER USAGE :
//    index i, and j for finding index [i,j] in array
// FUNCTION CALLED :
//    sprintf(), write(), exit()
// -----------------------------------------------------------

void Thread3::ThreadFunc() {/*contains all the execution code for the threads*/
  Thread::ThreadFunc(); /*must be the first line*/
  char buf[1000]; /*for printing*/

  if(jIndex - (int)pow(2, iIndex - 1) < 0) {
    sprintf(buf,"      Thread %d copies x[%d]\n", jIndex, jIndex);
    write(1,buf,strlen(buf));
    B[iIndex][jIndex] = B[iIndex-1][jIndex];
  }
  else{
    sprintf(buf, "      Thread %d computes x[%d] + x[%d-2^(%d-1)]\n", jIndex, jIndex, jIndex, iIndex);
    write(1,buf,strlen(buf));
    B[iIndex][jIndex] = B[iIndex - 1][jIndex] + B[iIndex - 1][jIndex - (int)pow(2, iIndex - 1)];
  }

  sprintf(buf,"      Thread %d exits\n", jIndex);
  write(1,buf,strlen(buf));

  Exit();
}
