// -----------------------------------------------------------
// NAME : Jacob McNeilly                    User ID: jrmcneil
// DUE DATE : 11/01/2019
// PROGRAM ASSIGNMENT #3
// FILE NAME : thread-main.cpp
// PROGRAM PURPOSE :
// Read in from a file an array of size n (given in file)
// Creates a child thread k=log2(n) times, each one
// computing differing gaps per run while filling out
// B[][]so that it can be used to generate the prefix sum out
// of given array. This file contains the main program.
// -----------------------------------------------------------

#include <iostream>
#include <math.h>
#include <cstring>
#include <stdio.h>
#include "thread.h"

int B[1000][1000]; /*2D arary with overkill space for holding arrays*/

// -----------------------------------------------------------
// FUNCTION main():
//     Runs the initial reading of the input file as well as
// outputing the final resuts. In between to get the resuts the
// method creates a array for storage and creates n threads for
// each run k.
// PARAMETER USAGE :
//    null
// FUNCTION CALLED :
//    log2(), sprintf(), write(), thread(), Begin(), Join()
// -----------------------------------------------------------

int main(int argc, char *argv[])
{
  char buf[1000]; /*used for print statments, 1000 to be safe*/

  int i = 0; /*i and j declared early bc cant declare in loops*/
  int j = 0;

  int *inputarray; /*temperary storage for reading in array from file*/
  int arraySize = 0; /*used for storing size read in from file*/
  int k = 0; /*k which will be computed using size*/

  scanf ("%d", &arraySize); /*read first element of fi, which is how many numbers in array*/
  k = log2(arraySize); /*calculate k from arraysize (given in proble desc)*/

  inputarray = new int[arraySize]; /*initialize the array using the new size*/
  for (i = 0; i < arraySize; i++){ /*for each num up to size read in one element from file*/
    scanf ("%d", &inputarray[i]);
  }

  sprintf(buf,"Concurrent Prefix Sum Computation \n \n"); /*After scanning from file notify of process start and output scanned size and array*/
  write(1,buf,strlen(buf));
  sprintf(buf,"Number of input data = %d \n", arraySize);
  write(1,buf,strlen(buf));
  sprintf(buf,"Input array: \n", arraySize);
  write(1,buf,strlen(buf));

  for (i = 0; i < arraySize; i++){
    sprintf(buf,"%4d",  inputarray[i]);
    write(1,buf,strlen(buf));
  }
  sprintf(buf,"\n\n");
  write(1,buf,strlen(buf));

  
  for( i = 0; i < arraySize; i++ ){ /*input the scanned in results in the first row*/
    B[0][i] = inputarray[i];
  }

  Thread3 *thisthread[k+1][arraySize]; /*Thread3s for k+1 runs*/

  for( i = 1; i < k+1; i++ ) {
    sprintf(buf,"Run %d: \n", i); /*Print the results for the run after all threads return*/
    write(1,buf,strlen(buf));
    for( j = 0; j < arraySize; j++ ){ /*for each thread give it its inputs and start*/ 
      thisthread[i][j] = new Thread3(i, j);
      thisthread[i][j]->Begin();
    }

    for( j = 0; j < arraySize; j++ ) { /*Waits for the begun threads to finish by joining*/
      thisthread[i][j]->Join();
    }

    sprintf(buf,"Results after run %d: \n", i); /*Print the results for the run after all threads return*/
    write(1,buf,strlen(buf));
    for (j = 0; j < arraySize; j++) {
      sprintf(buf,"%4d", B[i][j]);
      write(1,buf,strlen(buf));
    }
    sprintf(buf,"\n\n");
    write(1,buf,strlen(buf));
  }

  sprintf(buf,"Final results after run %d : \n", k); /*make final prints about results*/
  write(1,buf,strlen(buf));
  for (i = 0; i < arraySize; i++) {
    sprintf(buf,"%4d", B[k][i]);
    write(1,buf,strlen(buf));
  }
  sprintf(buf,"\n");
  write(1,buf,strlen(buf));

  Exit();
  return 0;
}
