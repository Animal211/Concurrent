// -----------------------------------------------------------
// NAME : Jake McNeilly                     User ID: jrmcneil
// DUE DATE : 12/06/2019
// PROGRAM ASSIGNMENT #5
// FILE NAME : boat-monitor.h
// PROGRAM PURPOSE :
//    The purpose of this program to to have a boat thread
//which can take 3 passengers repeadily until its met its trip
//quota. The number of cannibles and missionaries riding the boat
//along with the number of required rounds aross the lake are
// supplied as arguments
// -----------------------------------------------------------


#include "ThreadClass.h"
#include<iostream>

//Boat monitor constructor
class Boat_Monitor : public Monitor{

     public:
        Boat_Monitor(char* Name, int boatrounds,int passengers);
        int BoatReady(int *names,char *types);
        int BoatDone();
        int MissionaryArrives(int mname);
        int CannibalArrives(int cname);
       
     private:
        Condition WaitingCannibals;
        Condition WaitingMissionaries;
		Condition riding;
		Condition inprogress;

		int totalrounds; //number of trips to take
		int roundcounter; //number of rounds arcross the lake
		int cannibalswaiting;
        int missionarieswaiting;
        int passengerindex; //used for incrementing through passenger lists
        int passengernames[3]; //ids of passenger threads
        char passengertype[3]; //saves which passengers are cannables or missionaries
        bool boatretired;
        int finishedthreads; //number of closed threads
        int totalthreads; //total number of cannable and missionary threads
};