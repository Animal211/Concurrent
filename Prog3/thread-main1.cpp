#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstring>
#include <stdio.h>
#include "thread.h"

int B[1000][1000]; /*2D arary with overkil space for holding arrays*/

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
    sprintf(buf,"%s%4d", buf, inputarray[i]);
    write(1,buf,strlen(buf));
  }
  sprintf(buf,"\n\n");
  write(1,buf,strlen(buf));

  //Sets B[0] to the array[]
  for( i = 0; i < arraySize; i++ )
    B[0][i] = inputarray[i];

  //Loop iterations for threading
  Thread3 *thread[k+1][arraySize];

  for( i = 1; i < k+1; i++ ) {
    sprintf(buf,"Run %d: \n", i); /*Print the results for the run after all threads return*/
    write(1,buf,strlen(buf));
    //goes into Thread3for each element
    for( j = 0; j < arraySize; j++ ){
      thread[i][j] = new Thread3(i, j);
      thread[i][j]->Begin();
    }

    for( j = 0; j < arraySize; j++ ) /*Wait for n threads where n is the size of the array*/
      thread[i][j]->Join();


    sprintf(buf,"Results after run %d: \n", i); /*Print the results for the run after all threads return*/
    write(1,buf,strlen(buf));
    for (j = 0; j < arraySize; j++)
      sprintf(buf,"%s%4d", buf, B[i][j]);
    write(1,buf,strlen(buf));
    sprintf(buf,"\n\n");
    write(1,buf,strlen(buf));
  }



  sprintf(buf,"Final results after run %d : \n", k); /*make final prints about results*/
  write(1,buf,strlen(buf));
  for (i = 0; i < arraySize; i++)
    sprintf(buf,"%s%4d", buf, B[k][i]);
  write(1,buf,strlen(buf));
  sprintf(buf,"\n");
  write(1,buf,strlen(buf));

  Exit();
  return 0;
}
