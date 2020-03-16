// ----------------------------------------------------------- 
// NAME : Jake McNeilly                      User ID: jrmcneil 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread.cpp           
// PROGRAM PURPOSE :                                           
//  The purpose of this program is to do matrix multiplication
// on two matrices input through a input file. 
// -----------------------------------------------------------


#include <iostream>
#include "thread.h"
#include <stdio.h>

using namespace std;

extern Mutex* Printer;
extern int **Matrix;

// Constructor for grid threads
Grid::Grid(int i, int j, int ID, SynOneToOneChannel* left, SynOneToOneChannel* right, SynOneToOneChannel* up, SynOneToOneChannel* down) {
	UserDefinedThreadID = ID; // thread ID
	rowIndex = i;
	columnIndex = j;
	total = 0;  //For adding together numbers

	Up = up; //channel for top
	Down = down; //channel for bottom
	Left = left; //channel for left
	Right = right; //channel for right

	Printer->Lock();
	cout << "      Thread P[" << i << "," << j << "] started " << endl; //notify that the thread started
	Printer->Unlock();
}


// -----------------------------------------------------------
// FUNCTION  Grid :
//     Recieves the data from a matix positions up and left, it
// then multiplies them, stores them, and passes the new value
// forward to the bottom and right.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    sprintf(), Lock(), Unlock(), Receive(), Send(), exit()
// -----------------------------------------------------------
void Grid::ThreadFunc() {
	Thread::ThreadFunc();
	int EOD = -1; //used for sending end of data
	int receivedUp = 0; //used for saving the number from the up
	int receivedLeft = 0; //used for saving the number from the left
	char tempU[4] = ""; //buffer for the uup number, this way we can print eod if needed
	char tempL[4] = ""; //buffer for the left number, this way we can print eod if needed

	while (1) {
		Left->Receive(&receivedLeft, sizeof(int));
		Up->Receive(&receivedUp, sizeof(int));


		sprintf(tempU, "%d", receivedUp); //save passed numbers into temp
		sprintf(tempL, "%d", receivedUp);

		//if recieved turn out to be -1 turn into end of data
		if (receivedUp == -1) {
			sprintf(tempU, "EOD");
		}
		if (receivedLeft == -1) {
			sprintf(tempL, "EOD");
		}

		if (receivedUp == -1 && receivedLeft == -1) { //if recieved end of data from both channels get total
			Matrix[rowIndex - 1][columnIndex - 1] = total;
		}
		else { //otherwise there is more
			Printer->Lock();
			cout << "      Thread P[" << rowIndex << "," << columnIndex << "] received " << tempU << " from above and " << receivedLeft << " from left" << endl;
			Printer->Unlock();

			total += receivedUp*receivedLeft;
		}

		if (Down && Right) { //if have both neighbors send both
			Down->Send(&receivedUp, sizeof(int));
			Right->Send(&receivedLeft, sizeof(int));

			Printer->Lock();
			cout << "      Thread P[" << rowIndex << "," << columnIndex << "] sent " << tempU << " to below and " << tempL << " to right" << endl;
			Printer->Unlock();
		}
		else if (Down) { //if only down send down
			Down->Send(&receivedUp, sizeof(int));

			Printer->Lock();
			cout << "      Thread P[" << rowIndex << "," << columnIndex << "] sent " << tempU << " to below " << endl;
			Printer->Unlock();
		}
		else if (Right) { //if only a right send right
			Right->Send(&receivedLeft, sizeof(int));

			Printer->Lock();
			cout << "      Thread P[" << rowIndex << "," << columnIndex << "] sent " << tempL << " to right " << endl;
			Printer->Unlock();
		}

		if (receivedUp == -1 && receivedLeft == -1) { //if both were end of data time to save and stop
			Printer->Lock();
			cout << "      Thread P[" << rowIndex << "," << columnIndex << "] received EOD, saved result " << total << " and termainated" << endl;
			Printer->Unlock();

			Exit();
		}
	}
}

//constructor for column threads
ColumnThread::ColumnThread(int j, int ID, SynOneToOneChannel* down, int* array, int Size) {
	UserDefinedThreadID = ID; //thread ID 
	columnIndex = j;
	Down = down; //channel for below
	ptr = array; //pointer to passed array
	size = Size; //size of passed array

	Printer->Lock();
	cout << "   Column thread c[" << j << "] started" << endl;  //notif that thread has started
	Printer->Unlock();
}


// -----------------------------------------------------------
// FUNCTION  ColumnThread :
//     Sends the values from the column array one at a time until
// it reaches the end of data, at which point it sends EOD and quits.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//     Send(), Lock(), Unlock(), Exit()
// -----------------------------------------------------------
void ColumnThread::ThreadFunc() {
	Thread::ThreadFunc();
	int EOD = -1; //used for sending end of data

	for (int i = 0; i < size; i++) { //output array
		Down->Send(&ptr[i], sizeof(int));

		Printer->Lock();
		cout << "   Column thread c[" << columnIndex << "] sent " << ptr[i] << " to [1," << columnIndex << "]" << endl;
		Printer->Unlock();
	}

	Down->Send(&EOD, sizeof(int)); //once done signal complete end of data

	Printer->Lock();
	cout << "   Column thread c[" << columnIndex << "] sent EOD to [1," << columnIndex << "] and terminated" << endl;
	Printer->Unlock();


	Exit();
}

//constructor for row threads
RowThread::RowThread(int i, int ID, SynOneToOneChannel* right, int *array, int Size) {
	UserDefinedThreadID = ID; //thread ID
	rowIndex = i;
	Right = right; //channel to right

	ptr = array; //pointer to passed array
	size = Size; //size of passed array

	Printer->Lock();
	cout << "Row thread r[" << i << "] started" << endl; //notify that thread has started
	Printer->Unlock();
}


// -----------------------------------------------------------
// FUNCTION  RowThread :
//      Sends the values from the row array one at a time until
// it reaches the end of data, at which point it sends EOD and quits.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    Send(), Lock(), Unlock(), Exit()
// -----------------------------------------------------------
void RowThread::ThreadFunc() {
	Thread::ThreadFunc();
	int EOD = -1; //used for sending end of data

	for (int i = 0; i < size; i++) { //output array
		Right->Send(&ptr[i], sizeof(int));

		Printer->Lock();
		cout << "Row thread r[" << rowIndex << "] sent " << ptr[i] << " to [" << rowIndex << ",1]" << endl;
		Printer->Unlock();
	}

	Right->Send(&EOD, sizeof(int)); //once done signal complete end of data

	Printer->Lock();
	cout << "Row thread r[" << rowIndex << "] sent EOD to [" << rowIndex << ",1] and terminated" << endl;
	Printer->Unlock();

	Exit();
}
