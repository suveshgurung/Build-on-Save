/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#pragma once

/* defines and global variables */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifdef _WIN32
#include <windows.h>
extern DWORD processid;
#elif __unix__
#include <unistd.h>
extern pid_t processid;
#endif

/* function declarations */

void BOS_Init();
void BOS_Create_Thread();
void *BOS_Check_Is_File_Saved(void *);
