/* Date : September 30, 2024
* Author : Suvesh Gurung
*/

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* defines */

#define LEN 100000

/* global variables */

extern pid_t processPid;

/* function declarations */

void BOS_Init(char *);
void BOS_Create_Thread();
void *BOS_Check_Is_File_Saved(void *);
