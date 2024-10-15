/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#pragma once

#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#ifdef _WIN32
#include <windows.h>
#include <io.h>
#elif __unix__
#include <unistd.h>
#endif

/* defines */

#define FILE_PATH_LEN 10000

/* global variables */

typedef struct {
    time_t oldmTime;     // last modified time of the file.
    char *sourceFilePath;
    char *sourceFileName;
    char *filePath;
    int filePathSize;
} fileDetails;
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
