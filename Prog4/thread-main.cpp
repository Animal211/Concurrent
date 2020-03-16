// -----------------------------------------------------------
// NAME : Jake McNeilly                      User ID: jrmcneil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT #4
// FILE NAME: thread-main.cpp
// PROGRAM PURPOSE :
//    The purpose of Prog4 is to help manage Santa's Workshop.
// Santa naps while the reindeer vacation and the elves work.
// Elves can wake up Santa when a group of 3 have questions and 
// the last reindeeer back from vacation wakes him up to deliver
// presents. After a given number of rounds of deliveries Santa
// retires.
// -----------------------------------------------------------

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "thread.h"
#include "ThreadClass.h"

// ----------------------------------------------------------- 
// FUNCTION  Main:                        
//		The function of main is ti start the program by parsing
// in the number of elves, reindeer, and deliveries. It uses
// these to construct stana, elf, and reindeer threads.
// PARAMETER USAGE :                                           
//		argv: Used for passing numbers of elves, reindeer and deliveries
// FUNCTION CALLED :                                           
//		sprintf(), write(), Begin(), Join(), Exit()
// ----------------------------------------------------------- 
int main (int argc, char *argv[]){
	int numofElves = 0; //number of elves in the workshop
	int numofReindeer = 0; //number of reindeer to pull the sleigh
	int numofDeliveries = 0; //number of deliveries to be made before retirement

	char buf[1000]; //buffer used for print statments. 1000 is overkill
	int i = 0; //i is used for for loops and cannot be declared inside loops

	//make sure there are 3 inputs after program name
  if (argc != 4){
      sprintf(buf, "Error, must supply 3 numbers: Number of elves, Number of reindeer, Number of deliveries.", argv[0]);
      write(1, &buf, strlen(buf));
      exit(1);
  }
  else{
	  //read in the supplied arguments
	  numofElves = atoi(argv[1]);
	  numofReindeer = atoi(argv[2]);
	  numofDeliveries = atoi(argv[3]);

	  //check if they suppied zeros, if so appy default values
	  if (numofElves == 0) {
		  numofElves = 7;
	  }
	  if (numofReindeer == 0) {
		  numofReindeer = 9;
	  }
	  if (numofDeliveries == 0) {
		  numofDeliveries = 5;
	  }
  }

  //Create the single santa thread
  SantaThread *santa;
  santa = new SantaThread(numofElves, numofReindeer, numofDeliveries);
  santa -> Begin();

  //create elf threads up to numofElves
  ElfThread *elves[numofElves];
  for (i = 0; i < numofElves; i++)
    {
      elves[i] = new ElfThread(i);
      elves[i] -> Begin();
    }

  //create elf threads up to numofreindeer
  ReindeerThread *reindeer[numofReindeer];
  for (i = 0; i < numofReindeer; i++)
    {
      reindeer[i] = new ReindeerThread(i, numofReindeer); 
      reindeer[i] -> Begin();
    }

 
  santa -> Join(); // once santa thread is done kill the program, the factory shuts down
  Exit();
  return 0;
}
