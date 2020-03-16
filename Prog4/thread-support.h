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

#ifndef _THREAD_SUPPORT_H
#define _THREAD_SUPPORT_H

#include "ThreadClass.h"

int AskQuestion(int elfNum, Semaphore *sleep);
void ReindeerBack(int num);
void WaitOthers(int num, int max, Semaphore *sleep, Semaphore *Sleigh);
void WaitSleigh(Semaphore *ReindeerWait);
void FlyOff(Semaphore *ToyDeliver);
int Sleep(int reinTotal, Semaphore *sleep);
int CheckRetired(int n);
void ReleaseReindeer(void);
void HelpElves(void);

#endif
