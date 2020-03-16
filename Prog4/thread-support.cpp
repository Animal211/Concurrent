// -----------------------------------------------------------
// NAME : Jake McNeilly                      User ID: jrmcneil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT #4
// FILE NAME: thread-support.cpp
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

#include "thread-support.h"
#include "ThreadClass.h"

static int santaRetired = 0; //Used to set how many rounds until retirment for santa
static int elfCount = 0; //number of elves in queue
static int elves[3]; //holds the elves waiting to ask question
static int reindeer = 0; //number of reindeer at workshop

// ----------------------------------------------------------- 
// FUNCTION  AskQuestion:                        
//		Used for handling when an elf encounter an issue. It
// tracks how many elves have issues to bache them into groups
// of three to wake Santa up.
// PARAMETER USAGE : 
//		name: elf number/name asking question
//		sleep: For waking up santa
// FUNCTION CALLED :                                           
//      sprintf(), write()
// ----------------------------------------------------------- 
int AskQuestion(int name, Semaphore *sleep){
  char buf[1000]; //buf is used for printing, 1000 is overkill to be safe
  
  if (name == -1){ //Santa answered, get back to work
      sprintf(buf, "          Elves %d, %d, %d return to work\n", elves[0], elves[1], elves[2]);
      write(1, &buf, strlen(buf));
      return 1;
    }
  
  elves[elfCount] = name;
  elfCount++;

  //Check how many elves waiting to wake up Santa.
  if (elfCount < 3){ //not enough, keep waiting
      return 0;
    }
  else if (elfCount == 3){ //Enough elves to wake up Santa
      sleep -> Signal();
      sprintf(buf, "          Elves %d, %d, %d wake up the Santa\n", elves[0], elves[1], elves[2]);
      write(1, &buf, strlen(buf));
      return 1;
    }
}

// ----------------------------------------------------------- 
// FUNCTION  ReindeerBack:                        
//		Keeps track of when reindeer get back from vacation
// and more importantly how many.
// PARAMETER USAGE :                                           
//      name: name/number of returning reindeer
// FUNCTION CALLED :                                           
//		sprintf(), write()
// ----------------------------------------------------------- 
void ReindeerBack(int name){
  char buf[1000]; //buf is used for printing, 1000 is overkill to be safe
  reindeer++; //increment number of reindeer at workshop
  sprintf(buf, "     Reindeer %d returns\n", name);
  write(1, &buf, strlen(buf));
}

// ----------------------------------------------------------- 
// FUNCTION  WaitOthers:                        
//		Hold the reindeers that return to the workshop until
// all ther reindeer also return. The last reindeer wakes Santa.
// PARAMETER USAGE :                                           
//		name: name of reindeer
//		max: number of total reindeer
//		sleep: used for waking santa
//		reindeerwait: used for holding reindeer before hitching them up
// FUNCTION CALLED :                                           
//		sprintf(), wrinte(), Signal()
// ----------------------------------------------------------- 
void WaitOthers(int name, int max, Semaphore *sleep, Semaphore *ReindeerWait){
  char buf[1000]; //buf is used for printing, 1000 is overkill to be safe
  
  //if its the last reindeer signal Santa
  if (reindeer >= max) {
      sleep -> Signal();
      sprintf(buf, "     The last reindeer %d wakes up Santa\n", name);
      write(1, &buf, strlen(buf));
    }
  ReindeerWait -> Wait();
}

// ----------------------------------------------------------- 
// FUNCTION  WaitSleigh:                        
//		Holds the reindeer while getting hooked up to sleigh.
// PARAMETER USAGE :                                           
//		Sleigh: used to mark if sleigh is ready
// FUNCTION CALLED :                                           
//		Wait();
// ----------------------------------------------------------- 
void WaitSleigh(Semaphore *Sleigh){
  Sleigh -> Wait(); //wait to he hooked
}

// ----------------------------------------------------------- 
// FUNCTION  FlyOff:                        
//		Holds reindeer/sleigh until Santa gives the signal to leave.
// PARAMETER USAGE :                                           
//		Deliver: semephore to group before delivering
// FUNCTION CALLED :                                           
//		wait();
// ----------------------------------------------------------- 
void FlyOff(Semaphore *Deliver){
  Deliver -> Wait(); 
}

// ----------------------------------------------------------- 
// FUNCTION  Sleep:                        
//		Keeps santa sleeping unless the reindeer or elves wake
// him up.
// PARAMETER USAGE :                                           
//		max: total number of reindeer expected
//		sleep: used for waking santa
// FUNCTION CALLED :                                           
//		wait()
// ----------------------------------------------------------- 
int Sleep(int max, Semaphore *sleep){
  sleep -> Wait();
  if (reindeer >= max){
      return 1;
    }
  if (elfCount == 3){
      return 0;
    }
}


// ----------------------------------------------------------- 
// FUNCTION  CheckRetired:                        
//		Used to check if santa is retired as well as setting it.
// PARAMETER USAGE :                                           
//    temp: 0 if not retired, 1 if retired, 2 if checking number
// FUNCTION CALLED :                                           
//		N/A
// ----------------------------------------------------------- 
int CheckRetired(int temp){
  if (temp != 2){
    santaRetired = temp;
    }
  return santaRetired;
}
 
// ----------------------------------------------------------- 
// FUNCTION  ReleaseReindeer:                        
//		Used for releasing the reindeer back on vacation at the 
// end of the year if Santa isn't retiring. It sets the counted
// reindeer at the shop to zero.
// PARAMETER USAGE :                                           
//		N/A
// FUNCTION CALLED :                                           
//		N/A   
// ----------------------------------------------------------- 
void ReleaseReindeer(){
  reindeer = 0;
}

// ----------------------------------------------------------- 
// FUNCTION  HelpElves:                        
//		Used when Santa answers the elves questions. He answers
// then sets the number of elves in queue to 0.
// PARAMETER USAGE :                                           
//		N/A
// FUNCTION CALLED :                                           
//		sprintf(), write()     
// ----------------------------------------------------------- 
void HelpElves(){
  char buf[1000]; //buf is used for printing, 1000 is overkill to be safe
  sprintf(buf, "Santa answers the question posted by elves %d, %d, %d\n", elves[0], elves[1], elves[2]);
  write(1, &buf, strlen(buf));
  elfCount = 0;
}
