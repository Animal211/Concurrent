// -----------------------------------------------------------
// NAME : Jake McNeilly                      User ID: jrmcneil
// DUE DATE : 11/15/2019
// PROGRAM ASSIGNMENT #4
// FILE NAME: thread.cpp
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
#include "ThreadClass.h"
#include "thread.h"
#include "thread-support.h"

//Semaphores
static Semaphore Sleeping("Sleeping", 0); //for if santa is in bed. 0 == Awake, 1 == Asleep
static Semaphore Queue("Queue", 0); //for knowing if elves are in queue
static Semaphore Question("Question", 1); //for tracking when somone is using question method
static Semaphore Answer("Answer", 0); //for waiting on santa to wakeup and answer the questions
static Semaphore ReindeerWait("ReindeerWait", 0); //used for holding reindeer before hitching them up
static Semaphore ReindeerGone("ReindeerGone", 0); //number of reindeer away from workshop
static Semaphore Sleigh("Sleigh", 0); //if sleigh is ready or not
static Semaphore Delivering("Delivering", 0); //If santa/reindeer are out delivering toys
static Semaphore ReindeerAttached("ReindeerAttached", 0); //if the reindeer are hooked up to sleigh
static Semaphore ReindeerMutex("ReindeerMutex", 1); //mutex for ReindeerGone


//Constructor for the Santa thread
SantaThread::SantaThread(int elf, int reindeer, int retire) {
	char buf[1000]; //used for printing, 1000 is overkill to be safe

	//set variables with the input parameters
	numElves = elf;
	numReindeer = reindeer;
	numDeliveries = retire;

	//print santa is ready
	sprintf(buf, "Santa thread starts\n");
	write(1, &buf, strlen(buf));
}

// ----------------------------------------------------------- 
// FUNCTION  SantaThread ThreadFunc()                          
//     Controls the santa thread. That means signalling the elves,
// reindeer. Waits on either elves to answer their questions or 
// reindeer to return so he can deliver presents. After making
// a set number of deliveries then he retires and closes everything
// down.
// PARAMETER USAGE :                                           
//     No parameters passed to this thread.              
// FUNCTION CALLED :                                           
//              
// ----------------------------------------------------------- 
void SantaThread::ThreadFunc(){
	Thread::ThreadFunc();
	int i = 0; //used in for loops and needs to be declared outside of loop
	int deliveryNum = 1; //The number of the delivery being made (for tracking retirment when deliveryNum > Numofdeliveries
	char buf[1000]; //used for printing, 1000 is overkill to be safe

	CheckRetired(0); //from the start Santa is not retired

	Queue.Signal(); //from the start allow 3 elves to be able to ask questions
	Queue.Signal();
	Queue.Signal();

	//for each reindeer signal they can start
	for (i = 0; i < numReindeer; i++){
		ReindeerGone.Signal();
	}

	//Run in an infinite loop until program exits
	while (1){
		//see if reindeer or elves woke him
		if (Sleep(numReindeer, &Sleeping)) {
			//reindeer woke him
			for (i = 0; i < numReindeer; i++) {
				ReindeerWait.Signal();
			}
			sprintf(buf, "Santa is preparing sleighs\n");
			write(1, &buf, strlen(buf));
			for (i = 0; i < numReindeer; i++) {
				Sleigh.Signal();
			}

			//hook up the reindeer and leave
			for (i = 0; i < numReindeer; i++) {
				ReindeerAttached.Wait();
			}
			sprintf(buf, "The team flies off (%d)!\n", numDeliveries);
			write(1, &buf, strlen(buf));
			Delay();

			if (deliveryNum > numDeliveries) {
				CheckRetired(1); //Make Santa retired
				sprintf(buf, "After (%d) deliveries, Santa retires and is on vacation!\n", numDeliveries);
				write(1, &buf, strlen(buf));
				for (i = 0; i < numReindeer; i++) {
					Delivering.Signal();
				}
				Exit();
			}
			else { // There will be a next year so send reindeer on vacation
				sprintf(buf, "Santa Releases the Reindeer\n");
				write(1, buf, strlen(buf));
				ReleaseReindeer();
				for (i = 0; i < numReindeer; i++) {
					Delivering.Signal();
				}
				for (i = 0; i < numReindeer; i++) {
					ReindeerGone.Signal();
				}
				deliveryNum++;
			}

		//Elves woke him up not reindeer
		} else{ //Answer the elves questions
			sprintf(buf, "Santa is helping elves\n");
			write(1, &buf, strlen(buf));
		
			//Answer the question and rerelease the 3 elves
			Delay();
			HelpElves();
			Answer.Signal();
			Answer.Signal();
			Answer.Signal();
		}
	}
}

//Constructor for an Elf Thead
ElfThread::ElfThread(int number){
  char buf[1000]; //used for printing, 1000 is overkill to be safe

  name = number;
  sprintf(buf, "          Elf %d starts.\n", name + 1);
  write(1, buf, strlen(buf));
}

// -----------------------------------------------------------
// FUNCTION  ElfThread
//     
// PARAMETER USAGE :
//     
// FUNCTION CALLED :
//     
// -----------------------------------------------------------
void ElfThread::ThreadFunc() {
	Thread::ThreadFunc();
	char buf[1000]; //buff is used for printing, 1000 is overkill to be safe

	while (1) { //while(1) makes this a endless loop of elves having issues
		//work in the shop without a problem
		Delay();

		//We have a problem, check to see if Santa is retired and leave if he is
		if (CheckRetired(2)) { //send 2 to check if hes awake, if it returns true then hes retired and the elves can leave
			Exit();
		}
		else { //otherwise its still a problem 
			sprintf(buf, "          Elf %d has a problem\n", name + 1);
			write(1, &buf, strlen(buf));
		}

		Queue.Wait(); //wait until there arent 3 elves in queue
		Question.Wait(); //make sure one can use the question method

		//sif 3 elves have issues
		if (AskQuestion(name + 1, &Sleeping)) {
			//Wait for Santa to answer
			Answer.Wait();
			AskQuestion(-1, &Sleeping); //after getting answer resume working

										//empty the elf line and release being able to ask question
			Queue.Signal();
			Queue.Signal();
			Queue.Signal();
			Question.Signal();
		}
		else {//less that 3 elves having issues 
			Question.Signal();
			Answer.Wait();
		}
		//Question answered, go back to making toys
	}
	Exit();
}

//Constructor for Reindeer
ReindeerThread::ReindeerThread(int number, int Max){
  char buf[1000]; //used for printing, 1000 is overkill to be safe
  reindeerName = number;
  reindeerMax = Max;
  sprintf(buf, "     Reindeer %d starts.\n", number+1);
  write(1, &buf, strlen(buf));
}

// -----------------------------------------------------------
// FUNCTION  ReindeerThread
//     
// PARAMETER USAGE :
// 
// FUNCTION CALLED :
//     
// -----------------------------------------------------------
void ReindeerThread::ThreadFunc(){
	Thread::ThreadFunc();
	char buf[1000]; //used for printing, 1000 is overkill to be safe

	while (1){
		//If santa is retired then we can leave
		if (CheckRetired(2)){
			Exit();
		}

		//Otherwise they suntan on vacation for a bit
		Delay();
		ReindeerGone.Wait();

		//Return from vacation
		ReindeerMutex.Wait();
		ReindeerBack(reindeerName + 1);
		ReindeerMutex.Signal();

		WaitOthers(reindeerName + 1, reindeerMax, &Sleeping, &ReindeerWait); //Wait on the rest of the herd of reindeer
		WaitSleigh(&Sleigh); //once all here wait to be hooked up
		ReindeerAttached.Signal(); //notify all reindeer attached
		FlyOff(&Delivering); //Go deliver
	}
	Exit();
}





