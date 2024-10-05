/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#elif __unix__
#include <unistd.h>
#endif

/* defines */

#define FILE_PATH_LEN 10000

/* global variables */

/* TODO : change these into a structure if possible */
extern time_t oldmTime;     // last modified time of the file.
extern char *sourceFilePath;
extern char *sourceFileName;
extern char *filePath;
extern int filePathSize;
#ifdef _WIN32
extern DWORD processid;
#define F_OK 0
#define access _access
#elif __unix__
extern pid_t processid;
#endif

/* function declarations */

void BOS_Init(char *);
void BOS_Create_Thread();
void *BOS_Check_Is_File_Saved();
void BOS_End();
void trimString(char *str, int begin, int len);
