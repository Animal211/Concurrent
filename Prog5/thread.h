// -----------------------------------------------------------
// NAME : Jake McNeilly                     User ID: jrmcneil
// DUE DATE : 12/06/2019
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.h
// PROGRAM PURPOSE :
//    The purpose of this program to to have a boat thread
//which can take 3 passengers repeadily until its met its trip
//quota. The number of cannibles and missionaries riding the boat
//along with the number of required rounds aross the lake are
// supplied as arguments
// -----------------------------------------------------------

#ifndef _THREAD_H
#define _THREAD_H

#include "ThreadClass.h"

//cannibal constructor
class Cannibal_Thread : public Thread {
public:
	Cannibal_Thread(int cname);

private:
	void ThreadFunc();
	int cname;
	char startbuf[16];
};


//missionary constructor
class Missionary_Thread : public Thread {
public:
	Missionary_Thread(int mname);

private:
	void ThreadFunc();
	int mname;
	char startbuf[16];
};


//boat constructor
class Boat_Thread : public Thread {
public:
	Boat_Thread(int rounds, int people);

private:
	void ThreadFunc();
	int totalrounds;
	int counter;
	int names[3];
	char types[3];
};

#endif
