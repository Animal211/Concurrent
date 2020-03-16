#include <stdio.h>
#include <math.h>
#include <cstring>
#include "thread.h"


Thread3::Thread3(int i, int j)
{
  iIndex = i; //initialize the threads i and j indexes with passed params
  jIndex = j;
  char buf[1000];
  sprintf(buf,"      Thread %d Created\n", j);
  write(1,buf,strlen(buf));
}

/*contains all the execution code for the threads*/
void Thread3::ThreadFunc() {
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
