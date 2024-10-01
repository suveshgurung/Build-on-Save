#include "bos.h"
#include <pthread.h>

/* global variables */

pid_t processPid;

/* function definitions */

void BOS_Init(char *fileName) {
    char line[LEN];
    char buf[32];

    // get the process ID of the running file.
    snprintf(buf, sizeof(buf), "pidof ./%s", fileName);
    FILE *cmd = popen(buf, "r");

    fgets(line, LEN, cmd);
    pid_t pid = strtoul(line, NULL, 10);

    processPid = pid;
    printf("%d\n", processPid);

    BOS_Create_Thread();

    pclose(cmd);
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
