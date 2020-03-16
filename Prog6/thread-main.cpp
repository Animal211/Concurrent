// ----------------------------------------------------------- 
// NAME : Jake McNeilly                      User ID: jrmcneil 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread-main.cpp           
// PROGRAM PURPOSE :                                           
//  The purpose of this program is to do matrix multiplication
// on two matrices input through a input file. 
// -----------------------------------------------------------

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <stdlib.h>
#include <stdio.h>
#include "thread.h"
#include "ThreadClass.h"

using namespace std;
Mutex* Printer;
int **Matrix;


// -----------------------------------------------------------
// FUNCTION  main :
//     Through an input file this program reads in two matrices
// through stdin. The function creates grid, column, and row 
// threads to doo the multiplication while the main thread waits
//on all other threads to complete before giving final solution.
// PARAMETER USAGE :
//     No Parameters passed to this thread
// FUNCTION CALLED :
//    scanf(), sprintf(), malloc(), Begin(), Join(), print(), free()
// -----------------------------------------------------------
int main(int argc, char **argv) {
	Printer = new Mutex("Printer");
	int i; //i and j predeclared to save time redeclaring 50 times
	int j;
	int ID; //for saving threadid

	int matAR; //number of rows in mat a
	int matAC; //number of columns in mat a

	scanf("%d", &matAR); //read in num of columns and rows from file
	scanf("%d", &matAC);

	int matA[matAR][matAC]; //scan in the rest of matrix A
	for (i = 0; i < matAR; i++) {
		for (j = 0; j < matAC; j++) {
			scanf("%d", &matA[i][j]);
		}
	}

	int matBR; //number of rows in mat b
	int matBC; //number of columns in mat b

	scanf("%d", &matBR); //read in num of columns and rows from file
	scanf("%d", &matBC);

	int matB[matBC][matBR]; //scan in the res of matrix B
	for (i = 0; i < matBR; i++) {
		for (j = 0; j < matBC; j++) {
			scanf("%d", &matB[j][i]);
		}
	}


	if (matAC != matBR) { //if the number of A's columns does not equal number of B's rows than then the input is impossible
		cout << "Input Matrices cannot be multiplied. Please input compatible matrices.\n";
		return -1;
	}

	Matrix = (int **)malloc(matAR * sizeof(int *)); //preallocate the resources for holding the global matrix
	for (i = 0; i < matAR; i++) {
		Matrix[i] = (int *)malloc(matBC * sizeof(int));
	}

	int Channels[(matBC + 1)*(matAR + 1)][4]; //used for storing channels used by threads by index
	SynOneToOneChannel *channel[matBC*matAR * 2]; //construct the 2 channels for each matrix position for communicating
	for (i = 0; i < (matBC + 1)*(matAR + 1); i++) {
		for (j = 0; j < 4; j++) {
			Channels[i][j] = -1;
		}
	}

	int index = 0;
	int temp1 = matBC + 1;
	int temp2 = matBC + 2;

	for (i = 1; i <= matBC*matAR; i++) { //Create left->Right communication
		char ChannelName[1000]; //bufffer for holding channel names

		sprintf(ChannelName, "Channel: %d\n", index + 1);
		channel[index] = new SynOneToOneChannel(ChannelName, temp1, temp2); //creates a channel between left and rght index

		for (j = 0; j < 4; j++) { //add link to channels table
			if (Channels[temp1][j] == -1) {
				Channels[temp1][j] = index;
				break;
			}
		}

		for (j = 0; j < 4; j++) { //add link to channels table
			if (Channels[temp2][j] == -1) {
				Channels[temp2][j] = index;
				break;
			}
		}

		index++;
		if ((index % (matBC)) == 0) { //if end of row, jump forward t2 nto next row
			temp1 = temp1 + 2;
			temp2 = temp2 + 2;
			
		}
		else { //if not end of row move forward 1
			temp1 = temp2;
			temp2 = temp2 + 1;
		}
	}

	temp1 = 1;
	temp2 = matBC + 2;

	for (i = 1; i <= matBC*matAR; i++) {  //Create top -> Bottom communication
		char ChannelName[1000]; //bufffer for holding channel names

		sprintf(ChannelName, "Channel: %d \n", index + 1);
		channel[index] = new SynOneToOneChannel(ChannelName, temp1, temp2); //create a channel between top and bottom

		for (j = 0; j < 4; j++) { //add link to channels table
			if (Channels[temp1][j] == -1) {
				Channels[temp1][j] = index;
				break;
			}
		}
		for (j = 0; j < 4; j++) { //add link to channels table
			if (Channels[temp2][j] == -1) {
				Channels[temp2][j] = index;
				break;
			}
		}
		index++;
		if ((i % matAR) == 0) { //if end of column jump to next
			temp1 = temp1 - ((matBC + 1) * (matAR - 1)) + 1;
			temp2 = temp2 - ((matBC + 1) * (matAR - 1)) + 1;
		}
		else { //if not endo of column continue
			temp1 = temp1 + matBC + 1;
			temp2 = temp2 + matBC + 1;
		}
	}

	Grid *grids[matAR][matBC];  //create threads for grids

	ID = matBC + 2;
	for (i = 0; i < matAR; i++) { //for each of matA rows
		for (j = 0; j < matBC; j++) { //and for each of mat B's columns
			if (j + 1 == matBC && i + 1 == matAR) { //if in bottom right
				grids[i][j] = new Grid(i + 1, j + 1, ID, channel[Channels[ID][0]], NULL, channel[Channels[ID][1]], NULL);
			}
			else if (i + 1 == matAR) { //if on side
				grids[i][j] = new Grid(i + 1, j + 1, ID, channel[Channels[ID][0]], channel[Channels[ID][1]], channel[Channels[ID][2]], NULL);
			}
			else if (j + 1 == matBC) { //if on bottom
				grids[i][j] = new Grid(i + 1, j + 1, ID, channel[Channels[ID][0]], NULL, channel[Channels[ID][1]], channel[Channels[ID][2]]);
			}
			else { //all other cases in middle
				grids[i][j] = new Grid(i + 1, j + 1, ID, channel[Channels[ID][0]], channel[Channels[ID][1]], channel[Channels[ID][2]], channel[Channels[ID][3]]);
			}

			grids[i][j]->Begin(); //start the thread
			ID++; //increment the ID for the next
		}
		ID++;
	}

	RowThread *Row_Threads[matAR]; //create threads for rows
	for (i = 0, ID = matBC + 1; i < matAR; i++, ID += matBC + 1) {
		Row_Threads[i] = new RowThread(i + 1, ID, channel[Channels[ID][0]], matA[i], matAC);
		Row_Threads[i]->Begin();
	}

	ColumnThread *Column_Threads[matBC]; //create threads for columns
	for (i = 0, ID = 1; i < matBC; i++, ID++) {
		Column_Threads[i] = new ColumnThread(i + 1, ID, channel[Channels[ID][0]], matB[i], matBR);
		Column_Threads[i]->Begin();
	}

	for (i = 0; i < matAR; i++) { //wait on all grid threads to finish
		for (j = 0; j < matBC; j++) {
			grids[i][j]->Join();
		}
	}
	for (i = 0; i < matAR; i++) { //wait on all row threads to finish
		Row_Threads[i]->Join();
	}

	for (i = 0; i < matBC; i++) { //wait on all column threads to finish
		Column_Threads[i]->Join();
	}

	cout << endl << "*** From main ***" << endl; //start final output now that all threads finished (no need for mutex after)

	cout << "Matrix A: " << matAR << " rows and " << matAC << " Columns" << endl; //output matrix A's dimensions
	for (i = 0; i < matAR; i++) {
		for (j = 0; j < matAC; j++) {
			printf("%d ", matA[i][j]); //output matrix A data
		}
		cout << endl;
	}

	cout << endl << "Matrix B: " << matBR << " rows and " << matBC << " Columns" << endl; //output matrix B's dimensions
	for (i = 0; i < matBR; i++) {
		for (j = 0; j < matBC; j++) {
			printf("%d ", matB[j][i]); //output matrix B data
		}
		cout << endl;
	}

	cout << endl << "Matrix C = A*B: " << matAR << " rows and " << matBC << " Columns" << endl; //output final matrix dimensions
	for (i = 0; i < matAR; i++) {
		for (j = 0; j < matBC; j++) {
			printf("%d ", Matrix[i][j]); //output final matrix data
		}
		cout << endl;
	}

	free(Matrix); //free allocated matrix

	return 1;
}

