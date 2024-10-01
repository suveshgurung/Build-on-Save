/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#include "bos.h"
#include <pthread.h>
#include <unistd.h>

/* global variables */

pid_t processid;

/* function definitions */

void BOS_Init() {
    #ifdef _WIN32
        processid = GetCurrentProcessId();
    #elif __unix__
        processid = getpid(); 
    #endif

    printf("Current Process ID : %d\n", processid);

    BOS_Create_Thread();
}

void BOS_Create_Thread() {
    pthread_t bosThread; 

    if (pthread_create(&bosThread, NULL, BOS_Check_Is_File_Saved, NULL)) {
        perror("pthread_create");
        exit(100);
    }
}

void *BOS_Check_Is_File_Saved(void *arg) {
    while (1) {
        printf("1234\n");
    }

    return NULL;
}
