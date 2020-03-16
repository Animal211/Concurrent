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
#include <cstdlib>
#include <stdlib.h>
#include "thread.h"
#include "ThreadClass.h"

Mutex* Printer;

// -----------------------------------------------------------
// FUNCTION  main :
//     This function deals with taking th input of numbers of
// cannibalss, missionaries, and boat rounds. It then creates
// threads for the boat, each cannibals and missionaries then
//waits for them to complete.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    a list of functions that are called by this one
// -----------------------------------------------------------
int main(int argc, char **argv) {
	Printer = new Mutex("Printer");

	int c = 8; //number of cannibles
	int m = 8; //number of missionaries
	int b = 5; //number of rounds before boat retires

	if (argc != 4) { //make sure correct number of args passed, if not quit
		Printer->Lock();
		cout << "This program requires 3 arguments: c (number of cannibals), m (number of missionaries), b (rounds the boat crosses the river)" << endl;
		Printer->Unlock();
		exit(1);
	}

	if (atoi(argv[1]) != 0) { //if input is 0 leave default, otherwise update
		c = atoi(argv[1]);
	}
	if (atoi(argv[2]) != 0) {
		m = atoi(argv[2]);
	}
	if (atoi(argv[3]) != 0) {
		b = atoi(argv[3]);
	}

	Cannibal_Thread* CANNIBLES[c]; //creates array for c cannibles
	Missionary_Thread* MISSIONARIES[m]; //creates array for m missionaries
	Boat_Thread* BOAT = new Boat_Thread(b, c + m); //creates boat thread
	BOAT->Begin();

	for (int i = 0; i < c; i++) { //create annible thhread c times
		CANNIBLES[i] = new Cannibal_Thread(i + 1);
		CANNIBLES[i]->Begin();
	}

	for (int i = 0; i < m; i++) { //creates missionary thread m times
		MISSIONARIES[i] = new Missionary_Thread(i + 1);
		MISSIONARIES[i]->Begin();
	}

	for (int i = 0; i < c; i++) { //wait for all cannible threads
		CANNIBLES[i]->Join();
	}

	for (int i = 0; i < m; i++) { //wait on all the missinary threads
		MISSIONARIES[i]->Join();
	}

	BOAT->Join(); //wait on boat thread

	return EXIT_SUCCESS;
}
