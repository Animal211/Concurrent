
//-------------------------------------------------
// NAME : Jake McNeilly                     User ID: jrmcneil
// DUE DATE : 12/06/2019
// PROGRAM ASSIGNMENT #5
// FILE NAME : boat-monitor.cpp
// PROGRAM PURPOSE :
//    The purpose of this program to to have a boat thread
//which can take 3 passengers repeadily until its met its trip
//quota. The number of cannibles and missionaries riding the boat
//along with the number of required rounds aross the lake are
// supplied as arguments
// -----------------------------------------------------------

#include <iostream>
#include "ThreadClass.h"
#include "boat-monitor.h"

extern Mutex* Printer;


//Boat monitor constructor
Boat_Monitor::Boat_Monitor(char* Name,int boatrounds, int passengers ) : Monitor( Name, HOARE ), WaitingCannibals("WaitingCannibals"), WaitingMissionaries("WaitingMissionaries"), riding("riding"), inprogress("inprogress"){
	cannibalswaiting = 0; //Number of cannibals waiting to use boat
	missionarieswaiting = 0; //Number of missionaries waiting to use boat
	totalrounds = boatrounds; //Number of rounds to that need to be taken
	roundcounter = 0;//round counter
	boatretired = false; //when the boat is finished
	finishedthreads = 0; //the number of threads terminated
	totalthreads = passengers;  //the total number of threads running
}

// -----------------------------------------------------------
// FUNCTION  BoatReady :
//     This function is for when the boat is ready for new passengers
// It repeadily checks to see if adiquate combo of passengers is ready.
// PARAMETER USAGE :
//     names : the id's of passenger threads
//   types : array of m/c's of passgengers
// FUNCTION CALLED :
//    MonitorBegin(), Lock(), Unlock(), Signal(), MonitorEnd
// -----------------------------------------------------------
int  Boat_Monitor::BoatReady(int *names, char *types){
    MonitorBegin();  
    if(roundcounter == totalrounds){ //see if boat has made enough rounds to retire, if so start closing down

        boatretired = true; //Boat is now retired

		for (int i = 0; i < cannibalswaiting; i++) {
			WaitingCannibals.Signal();
		}

		for (int i = 0; i < missionarieswaiting; i++) {
			WaitingMissionaries.Signal();
		}

		if (finishedthreads != totalthreads) { //if it was last trip, wait on threads to stop
			inprogress.Wait();
		}

        Printer->Lock();
        cout << "MONITOR: "<< roundcounter <<" crosses have been made." <<endl << "MONITOR: This river cross is closed indefinitely for renovation." << endl; //Print final message of cross closure
        Printer->Unlock();

        MonitorEnd();
        return -1;
    }

    if( (cannibalswaiting >= 3) || (missionarieswaiting >=2 && cannibalswaiting >= 1) || (missionarieswaiting >=3)){ //see if good combination of passengers exist
        roundcounter++;
        if(cannibalswaiting >= 3){ //three cannibals
            cannibalswaiting -= 3;
            types[0] = 'c';  //put passengers into record
			types[1] = 'c';
			types[2] = 'c';
            passengerindex = 0;
            WaitingCannibals.Signal();
            passengerindex = 1;
            WaitingCannibals.Signal();
            passengerindex = 2;
            WaitingCannibals.Signal();

            Printer->Lock();
            cout << "MONITOR("<< roundcounter <<"): " << "three cannibals (" << passengernames[0] << ", " << passengernames[1] << ", " << passengernames[2] <<") are selected" <<endl;
            Printer->Unlock();
        }
        else if (missionarieswaiting >= 3){ //threee missionaries
            missionarieswaiting -=3;
            types[0] = 'm'; //put passengers into record
			types[1] = 'm';
			types[2] = 'm';
            passengerindex=0;
            WaitingMissionaries.Signal();
            passengerindex=1;
            WaitingMissionaries.Signal();
            passengerindex=2;
            WaitingMissionaries.Signal();

            Printer->Lock();
            cout << "MONITOR("<< roundcounter <<"): " << "three missionaries (" << passengernames[0] << ", " << passengernames[1] << ", " << passengernames[2] << ") are selected" <<endl;
            Printer->Unlock();
        }
        else if (missionarieswaiting >=2 && cannibalswaiting >= 1){ //one cannibal two missionaries
            cannibalswaiting --;
            missionarieswaiting-=2;
            types[0] = 'c';
			types[1] = 'm';
			types[2] = 'm';
            passengerindex=0; //put passengers into record
            WaitingCannibals.Signal();
            passengerindex=1;
            WaitingMissionaries.Signal();
            passengerindex=2;
            WaitingMissionaries.Signal();
 
            Printer->Lock();
            cout << "MONITOR("<< roundcounter <<"): "  << "one cannibal (" << passengernames[0] << ") and two missionaries (" << passengernames[1] << ", " << passengernames[2] <<") are selected" <<endl;
            Printer->Unlock();
        }
        names[0] = passengernames[0];
        names[1] = passengernames[1];
        names[2] = passengernames[2];
        MonitorEnd(); 
        return 1;
    }else{
        MonitorEnd(); 
        return 0;  
    }    
}   

// -----------------------------------------------------------
// FUNCTION  BoatDone :
//     This function is used for after te boat has crossed the lake.
// The function releases the passengers after reaching the other side.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    MonitorBegin(), Signal(), MonitorEnd()
// -----------------------------------------------------------
int  Boat_Monitor::BoatDone(){
    int returnValue =0;
    MonitorBegin();  

    riding.Signal();       //Signal all three pasangers to get off boat 
    riding.Signal();
    riding.Signal();
    if(roundcounter == totalrounds)
        returnValue=1;

    MonitorEnd();   
    return returnValue;
}  

// -----------------------------------------------------------
// FUNCTION  MissionaryArrives :
//     Used for holding missionaries to board the boat as well
// as cross on the boat.
// PARAMETER USAGE :
//     mname: the id of the missionary thread
// FUNCTION CALLED :
//   MonitorBegin(), Signal(), Wait(), MonitorEnd()
// -----------------------------------------------------------
int  Boat_Monitor::MissionaryArrives(int mname){
    MonitorBegin();
    if(boatretired){ //ends thread
         finishedthreads++;
         if(finishedthreads == totalthreads)
            inprogress.Signal();
         MonitorEnd();
         return -1;
    }

    missionarieswaiting++;      // increase waiting size
    WaitingMissionaries.Wait();     // waits until joining the boat

    if(boatretired){ //ends thread
        finishedthreads++;
        if(finishedthreads == totalthreads)
            inprogress.Signal();
        MonitorEnd();
        return -1;
    }

    passengernames[passengerindex] = mname;
    passengertype[passengerindex] = 'm';

    riding.Wait(); // Currently on the moving boat

    if(boatretired){ //ends thread
        finishedthreads++;
        if(finishedthreads == totalthreads)
            inprogress.Signal();
        MonitorEnd();
        return -1;
    }

    MonitorEnd();   
    return 1;
}  

// -----------------------------------------------------------
// FUNCTION  CannibalArrives :
//     Used for holding cannibals to board the boat as well
// as cross on the boat.
// PARAMETER USAGE :
//     cname : the id of the cannibal thread
// FUNCTION CALLED :
//    MonitorBegin(), Signal(), Wait(), MonitorEnd()
// -----------------------------------------------------------
int  Boat_Monitor::CannibalArrives(int cname){
    MonitorBegin();    
    if(boatretired){ //ends thread
        finishedthreads++;
        if(finishedthreads == totalthreads)
            inprogress.Signal();
        MonitorEnd();
        return -1;
    }
    cannibalswaiting++; // increase waiting size    
    WaitingCannibals.Wait(); // waits until joining the boat

    if(boatretired){ //ends thread
        finishedthreads++;
        if(finishedthreads == totalthreads)
            inprogress.Signal();
        MonitorEnd();
        return -1;
    }

    passengernames[passengerindex] = cname;
    passengertype[passengerindex] = 'c';

    riding.Wait(); // Currently on the moving boat

    if(boatretired){ //ends thread
        finishedthreads++;
        if(finishedthreads == totalthreads)
            inprogress.Signal();
        MonitorEnd();
        return -1;
    }
    MonitorEnd();
    return 1;
}
