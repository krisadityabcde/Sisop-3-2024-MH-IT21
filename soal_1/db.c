#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <time.h>

#define SHM_KEY 1234

int main() {
    key_t key = SHM_KEY;
    int shmid = shmget(key, sizeof(char) * 1024, IPC_CREAT | 0666);
    char *shm = shmat(shmid, NULL, 0);

    char filename[256];
    char filedir[512];
    char type[20];
    char logfilename[256] = "/home/rafaelega24/SISOP/modul3/1/microservices/database/db.log";
    FILE *logfile = fopen(logfilename, "a");

    if (logfile == NULL) {
        printf("Cannot open log file: %s\n", logfilename);
        return 1;
    }

    while (*shm != '\0') {
        strcpy(filename, shm);
        shm += strlen(shm) + 1;

        char source[512], dest[512];
        sprintf(source, "/home/rafaelega24/SISOP/modul3/1/new-data/%s", filename);
        sprintf(dest, "/home/rafaelega24/SISOP/modul3/1/microservices/database/%s", filename);

        if (strstr(filename, "trashcan.csv")) {
            strcpy(type, "Trash Can");
        } else {
            strcpy(type, "Parking Lot");
        }

        rename(source, dest);

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%D %H:%M:%S", t);

        fprintf(logfile, "[%s] [%s] [%s]\n", timestamp, type, filename);
    }

    fclose(logfile);
    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
