/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include <fcntl.h>
#ifdef _WIN32
#include <windows.h>
#elif __unix__
#include <unistd.h>
#endif

/* defines */

#define FILE_PATH_LEN 10000

/* global variables */

extern time_t oldmTime;     // last modified time of the file.
#ifdef _WIN32
extern DWORD processid;
#elif __unix__
extern pid_t processid;
#endif

/* function declarations */

void BOS_Init(char *);
void BOS_Create_Thread();
void *BOS_Check_Is_File_Saved(void *);
