// ----------------------------------------------------------- 
// NAME : Jake McNeilly                      User ID: jrmcneil 
// DUE DATE : 12/13/2019                                       
// PROGRAM ASSIGNMENT #6                                        
// FILE NAME : thread.h            
// PROGRAM PURPOSE :                                           
//  The purpose of this program is to do matrix multiplication
// on two matrices input through a input file. 
// -----------------------------------------------------------


#ifndef _THREAD_H
#define _THREAD_H

#include "ThreadClass.h"

//constructor for grid threads
class Grid : public Thread {
public:
	Grid(int i, int j, int ID, SynOneToOneChannel* left, SynOneToOneChannel* right, SynOneToOneChannel* up, SynOneToOneChannel* down);

private:
	void ThreadFunc();
	int total;
	int rowIndex
	int columnIndex;

	SynOneToOneChannel* Up;
	SynOneToOneChannel* Down;
	SynOneToOneChannel* Left;
	SynOneToOneChannel* Right;
};

//constructor for row threads
class RowThread : public Thread {
public:
	RowThread(int i, int ID, SynOneToOneChannel* right, int *array, int Size);

private:
	void ThreadFunc();
	int rowIndex;

	SynOneToOneChannel* Right;
	int *ptr;
	int size;
};

//constructor for column threads
class ColumnThread : public Thread {
public:
	ColumnThread(int j, int ID, SynOneToOneChannel* down, int* array, int Size);

private:
	void ThreadFunc();
	int columnIndex;

	SynOneToOneChannel* Down;
	int *ptr;
	int size;
};

#endif
