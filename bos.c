/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#include "bos.h"
#include <stdio.h>

/* global variables */

time_t oldmTime;     // last modified time of the file.
#ifdef _WIN32
DWORD processid;
#elif __unix__
pid_t processid;
#endif

/* function definitions */

void BOS_Init(char *sourceFileName) {
    // Get the process ID.
    #ifdef _WIN32
    processid = GetCurrentProcessId();
    #elif __unix__
    processid = getpid(); 
    #endif

    printf("Current Process ID : %d\n", processid);

    // Get the last modified date of the file.
    struct stat fileStat;
    #ifdef _WIN32
    TCHAR filePathBuf[FILE_PATH_LEN];
    GetModuleFileName(NULL, filePathBuf, FILE_PATH_LEN);
    #elif __unix__
    char filePath[FILE_PATH_LEN] = {'\0'};
    if (getcwd(filePath, sizeof(filePath)) == NULL) {
        perror(" [BOS_Init] getcwd");
        exit(errno);
    }
    #endif

    char sourceFilePath[FILE_PATH_LEN] = {'\0'};
    snprintf(sourceFilePath, FILE_PATH_LEN, "%s/%s", filePath, sourceFileName);
    printf("%s\n", sourceFilePath);
    // TODO : See how to get the path of the file.
    if (stat(sourceFilePath, &fileStat) == -1) {
        perror(" [BOS_Init] stat");
        exit(errno);
    }
    oldmTime = fileStat.st_mtim.tv_sec;

    BOS_Create_Thread();
}

void BOS_Create_Thread() {
    pthread_t bosThread; 

    if (pthread_create(&bosThread, NULL, BOS_Check_Is_File_Saved, NULL)) {
        perror(" [BOS_Create_Thread] pthread_create");
        exit(errno);
    }
}

void *BOS_Check_Is_File_Saved(void *arg) {
    // while (1) {
    //     printf("1234\n");
    // }

    return NULL;
}
