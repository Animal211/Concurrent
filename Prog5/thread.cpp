// -----------------------------------------------------------
// NAME : Jake McNeilly                     User ID: jrmcneil
// DUE DATE : 12/06/2019
// PROGRAM ASSIGNMENT #5
// FILE NAME : thread.cpp
// PROGRAM PURPOSE :
//    The purpose of this program to to have a boat thread
//which can take 3 passengers repeadily until its met its trip
//quota. The number of cannibles and missionaries riding the boat
//along with the number of required rounds aross the lake are
// supplied as arguments
// -----------------------------------------------------------

#include <iostream>
#include "thread.h"
#include "boat-monitor.h"
using namespace std;

extern Mutex* Printer;   //A mutex lock used to secure print statments
static Boat_Monitor *Boat;  //the monitor that will be used for the boat

//Constructor for the cannibal thread
Cannibal_Thread::Cannibal_Thread(int Cname){
	int i = 0;
	cname = Cname; //set name equal to passed id
	
	for (int i = 1; i < cname; i++) {
		startbuf[i - 1] = ' ';
	}
	startbuf[i-1] = '\0';
}


// -----------------------------------------------------------
// FUNCTION  Cannibal_Thread :
//     This function handles a cannibal thread. It starts by getting
// in line for the boat then waiting to get off at the other side.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    Lock(), Unlock(), Delay(), Exit(), srand(), rand()
// -----------------------------------------------------------
void Cannibal_Thread::ThreadFunc(){
    Thread::ThreadFunc();
    Printer->Lock();
    cout << startbuf << "Cannibal " << cname <<" starts" << endl; //print your thread name when thread starts
    Printer->Unlock();

    srand(time(0));
    int distance;

	while (1) {
		distance = rand() % 10000; //get random travel distance and travel it
		for (int i = 0; i != distance; i++) {
			Delay();
		}

		Printer->Lock();
		cout << startbuf << "Cannibal " << cname << " arrives" << endl; //annouce arrival of cannibal
		Printer->Unlock();

		if (Boat->CannibalArrives(cname) == -1) { //Get in line for boat
			Exit();
		}

		distance = rand() % 10000; //get random travel distance and travel it
		for (int i = 0; i != distance; i++) {
			Delay();
		}
	}
	Exit();
}


//Constructor for the missionary thread
Missionary_Thread::Missionary_Thread(int Mname){
	int i = 0;
	mname = Mname;
	
	for (int i = 1; i < mname; i++) {
		startbuf[i - 1] = ' ';
	}
	startbuf[i-1] = '\0';
}


// -----------------------------------------------------------
// FUNCTION  Missionary_Thread :
//    This function handles a missionary thread. It starts by getting
// in line for the boat then waiting to get off at the other side.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    Lock(), Unlock(), Delay(), Exit(), srand(), rand()
// -----------------------------------------------------------
void Missionary_Thread::ThreadFunc() {
	Thread::ThreadFunc();
	srand(time(0));

	Printer->Lock();
	cout << startbuf << "Missionary " << mname << " starts" << endl; //print your thread name when thread starts
	Printer->Unlock();

	int distance;

	while (1) {
		distance = rand() % 10000; //get random travel distance and travel it
		for (int i = 0; i != distance; i++) {
			Delay();
		}

		/*Prints missionary has arrived msg*/
		Printer->Lock();
		cout << startbuf << "Missionary " << mname << " arrives" << endl;
		Printer->Unlock();

		if (Boat->MissionaryArrives(mname) == -1) {     // register to cross the river
			Exit();
		}

		distance = rand() % 10000; //get random travel distance and travel it
		for (int i = 0; i != distance; i++) {
			Delay();
		}
	}
}


//Constructor for the boat thread
Boat_Thread::Boat_Thread(int rounds, int people) {
	totalrounds = rounds;
	Boat = new Boat_Monitor("Boat", rounds, people);
	counter = 1;
}


// -----------------------------------------------------------
// FUNCTION  Boat_Thread :
//     This function controls the boat thread, it starts and
// waits randomly befre arriving. Once arrived it loads, then transports
//passengers accross the lake before returning.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    srand(), rand(), Delay(), Lock(), Unlock(), BoatReady(), BoatDone()
// -----------------------------------------------------------
void Boat_Thread::ThreadFunc() {
	Thread::ThreadFunc();
	srand(time(0));
	int distance;
	int first = 1; //checks if its the first time checking for passengers
	int final = 0; //for knowing if final round

	Printer->Lock();
	cout << "***** BOAT thread starts" << endl; //announce boat thread when it starts
	Printer->Unlock();

	while (1) { //infinite loop
		distance = rand() % 10000; //get random travel distance and travel it
		for (int i = 0; i != distance; i++) {
			Delay();
		}

		if (first && !final) { //if it is your first but not only round, get boat ready
			first = 0; //it is no longer your first check

			Printer->Lock();
			cout << "*****  The boat is ready" << endl; //The boat is ready for a new round
			Printer->Unlock();
		}

		int havePassangers = Boat->BoatReady(names, types); //check to see if there is a good combination of passengers

		if (havePassangers == 1) { //if there is a good combinationof passengers
			Printer->Lock();
			cout << "***** Boat load (" << counter << "): Passanger list (" << types[0] << names[0] << ", " << types[1] << names[1] << ", " << types[2] << names[2] << ")" << endl; //load ready passengers on boat and announce them
			Printer->Unlock();

			distance = rand() % 10000; //get random travel distance and travel it
			for (int i = 0; i != distance; i++) {
				Delay();
			}

			Printer->Lock();
			cout << "***** Boat load(" << counter << "): Completed" << endl; //boat has now returned, anounce round as completed
			Printer->Unlock();

			final = Boat->BoatDone();  //let off passengers and check to see if its time to retire
			counter++; //successful trip, increment counter
			first = 1;

		}
		else if (havePassangers == -1) { //if boatready returns -1 it was the last round. Quit out
			Exit();
		}
	}
}
