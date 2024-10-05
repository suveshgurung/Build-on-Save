/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#include "bos.h"

/* global variables */

time_t oldmTime;     // last modified time of the file.
char *sourceFilePath = NULL;
char *sourceFileName = NULL;
char *filePath = NULL;
int filePathSize;
#ifdef _WIN32
DWORD processid;
#elif __unix__
pid_t processid;
#endif

/* function definitions */

void BOS_Init(char *fileName) {
    // Get the process ID.
    #ifdef _WIN32
    processid = GetCurrentProcessId();
    #elif __unix__
    processid = getpid(); 
    #endif

    // printf("Current Process ID : %d\n", processid);

    // Get the last modified date of the file.
    struct stat fileStat;
    #ifdef _WIN32
    /* TODO : dynamic memory for win32 as well */
    TCHAR filePathBuf[FILE_PATH_LEN];
    GetModuleFileName(NULL, filePathBuf, FILE_PATH_LEN);
    #elif __unix__
    filePathSize = 5;       // initially assume a size of 5.
    filePath = (char *)malloc(filePathSize);
    char *temp;

    do {
        if ((temp = getcwd(filePath, filePathSize)) == NULL) {
            if (errno == ERANGE) {      // Check if the buffer size is not enough.
                filePathSize++;
                filePath = (char *)realloc(filePath, filePathSize);
            } else {
                perror("[BOS_Init] getcwd, cannot get file path");
                free(filePath);
                fprintf(stderr, "BOS is not tracking further changes. Exiting BOS...\n") ;
                return;
                /* TODO : See if we have to exit from here */
                // exit(errno);
            }
        }
    } while ((errno == ERANGE) && (temp == NULL));
    #endif

     
    // set the global variable holding source file name.
    sourceFileName = (char *)realloc(sourceFileName, sizeof(fileName));
    strcpy(sourceFileName, fileName);

    int sourceFilePathSize = filePathSize + strlen(sourceFileName) + 2;     // +2 for '/' and '\0'
    sourceFilePath = (char *)realloc(sourceFilePath, sourceFilePathSize);
    snprintf(sourceFilePath, sourceFilePathSize, "%s/%s", filePath, sourceFileName);

    if (stat(sourceFilePath, &fileStat) == -1) {
        perror("[BOS_Init] stat");
        free(filePath);
        BOS_End();
        fprintf(stderr, "BOS is not tracking further changes. Exiting BOS...\n") ;
        return;
        // exit(errno);
    }
    oldmTime = fileStat.st_mtime;

    BOS_Create_Thread();
}

void BOS_Create_Thread() {
    pthread_t bosThread; 

    if (pthread_create(&bosThread, NULL, BOS_Check_Is_File_Saved, NULL)) {
        perror("[BOS_Create_Thread] pthread_create");
        BOS_End();
        fprintf(stderr, "BOS is not tracking further changes. Exiting BOS...\n") ;
        return;
        // exit(errno);
    }
}

void *BOS_Check_Is_File_Saved() {
    struct stat fileStat;
    time_t currmTime;

    while (1) {
        if (stat(sourceFilePath, &fileStat) == -1) {
            if (errno == ENOENT) {      // File temporarily not available (probably being modified).
                // sleep for 5 microseconds.
                #ifdef _WIN32
                Sleep(5 / 1000);
                #elif __unix__
                struct timespec ts;
                ts.tv_sec = 0;
                ts.tv_nsec = 5 * 1000 * 1000;
                nanosleep(&ts, NULL);
                #endif
                continue;
            } else {
                perror("[BOS_Check_Is_File_Saved] stat");
                BOS_End();
                fprintf(stderr, "BOS is not tracking further changes. Exiting BOS...\n") ;
                return NULL;
                // exit(errno);
            }
        }
        currmTime = fileStat.st_mtime;

        if (currmTime > oldmTime) {
            oldmTime = currmTime;

            char makeFilePath[filePathSize + 9];        // +9 for "Makefile" and '\0'
            snprintf(makeFilePath, filePathSize + 9, "%s/Makefile", filePath);

            if (access(makeFilePath, F_OK) == 0) {
                // use make to build.
                system("make");
            } else {
                // normally build
                int fileLen = strlen(sourceFileName);
                char command[38 + fileLen + fileLen - 2];
                char fileNameWithoutExtension[fileLen - 2];

                strcpy(fileNameWithoutExtension, sourceFileName);

                trimString(fileNameWithoutExtension, fileLen - 2, 2);
                snprintf(command, 38 + 2 * fileLen - 1, "gcc -Wall -Wextra -pedantic -o %s %s bos.c", fileNameWithoutExtension, sourceFileName);

                system(command);
            }

            // kill(processid, SIGKILL);
        }
    }

    return NULL;
}

void BOS_End() {
    free(sourceFilePath);
    free(sourceFileName);
    free(filePath);
}

void trimString(char *str, int begin, int len) {
    int strLen = strlen(str);

    if (begin + len > strLen) {
        len = strLen - begin;
    }

    memmove(str + begin, str + begin + len, len);
}
