/* Date : September 30, 2024
* Author : Suvesh Gurung
*/


#include "bos.h"

/* global variables */

time_t oldmTime;     // last modified time of the file.
char *sourceFilePath = NULL;
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

    // printf("Current Process ID : %d\n", processid);

    // Get the last modified date of the file.
    struct stat fileStat;
    #ifdef _WIN32
    /* TODO : dynamic memory for win32 as well */
    TCHAR filePathBuf[FILE_PATH_LEN];
    GetModuleFileName(NULL, filePathBuf, FILE_PATH_LEN);
    #elif __unix__
    int filePathSize = 5;       // initially assume a size of 5.
    char *filePath = (char *)malloc(filePathSize);
    char *temp;

    do {
        if ((temp = getcwd(filePath, filePathSize)) == NULL) {
            if (errno == ERANGE) {
                filePathSize++;
                filePath = (char *)realloc(filePath, filePathSize);
            } else {
                perror(" [BOS_Init] getcwd");
                exit(errno);
            }
        }
    } while ((errno == ERANGE) && (temp == NULL));
    #endif

    int sourceFilePathSize = filePathSize + strlen(sourceFileName) + 2;     // +2 for '/' and '\0'
    sourceFilePath = (char *)realloc(sourceFilePath, sourceFilePathSize);
    snprintf(sourceFilePath, sourceFilePathSize, "%s/%s", filePath, sourceFileName);

    // printf("%s\n", sourceFilePath);

    if (stat(sourceFilePath, &fileStat) == -1) {
        perror(" [BOS_Init] stat");
        BOS_End();
        exit(errno);
    }
    oldmTime = fileStat.st_mtim.tv_sec;

    // printf("%ld\n", oldmTime);

    free(filePath);
    BOS_Create_Thread();
}

void BOS_Create_Thread() {
    pthread_t bosThread; 

    if (pthread_create(&bosThread, NULL, BOS_Check_Is_File_Saved, NULL)) {
        perror(" [BOS_Create_Thread] pthread_create");
        BOS_End();
        exit(errno);
    }
}

void *BOS_Check_Is_File_Saved(void *arg) {
    struct stat fileStat;
    time_t currmTime;

    while (1) {
        if (stat(sourceFilePath, &fileStat) == -1) {
            if (errno == ENOENT) {
                // File temporarily not available (probably being modified).
                // sleep for 5 microseconds.
                #ifdef _WIN32
                Sleep(5 / 1000);
                #elif __unix__
                usleep(5 * 1000);
                #endif
                continue;
            } else {
                perror(" [BOS_Check_Is_File_Saved] stat");
                BOS_End();
                exit(errno);
            }
        }
        currmTime = fileStat.st_mtim.tv_sec;

        if (currmTime > oldmTime) {
            oldmTime = currmTime;
            printf("File is modified\n");
        }
    }

    return NULL;
}

void BOS_End() {
    free(sourceFilePath);
}
