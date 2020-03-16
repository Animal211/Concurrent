// -----------------------------------------------------------
// NAME : Jake McNeilly                      User ID: jrmcneil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT #4
// FILE NAME: thread.h
// PROGRAM PURPOSE :
//    The purpose of Prog4 is to help manage Santa's Workshop.
// Santa naps while the reindeer vacation and the elves work.
// Elves can wake up Santa when a group of 3 have questions and 
// the last reindeeer back from vacation wakes him up to deliver
// presents. After a given number of rounds of deliveries Santa
// retires.
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

#include "ThreadClass.h"

//Santa Constructor
class SantaThread : public Thread{
 public:
  SantaThread(int elf, int reindeer, int retire);

 private:
  void ThreadFunc();
  int i;
  int numElves;
  int numReindeer;
  int numDeliveries;
};

//Elf constructor
class ElfThread : public Thread{
 public:
  ElfThread(int number);

 private:
  void ThreadFunc();
  int name;
};

//Reindeer constructor
class ReindeerThread : public Thread{
 public:
  ReindeerThread(int name, int max); 

 private:
  void ThreadFunc();
  int reindeerName;
  int reindeerMax;
};

#endif
  
