#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/stat.h>
#include <dirent.h>

#define SHM_KEY 1234

int main() {
    key_t key = SHM_KEY;
    int shmid = shmget(key, sizeof(char) * 1024, IPC_CREAT | 0666);
    char *shm = shmat(shmid, NULL, 0);

    DIR *dir;
    struct dirent *entry;
    struct stat statbuf;
    char filename[512];

    dir = opendir("/home/rafaelega24/SISOP/modul3/1/new-data");
    if (dir == NULL) {
        printf("Cannot open directory\n");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        sprintf(filename, "/home/rafaelega24/SISOP/modul3/1/new-data/%s", entry->d_name);
        if (stat(filename, &statbuf) == 0 && S_ISREG(statbuf.st_mode)) {
            if (strstr(entry->d_name, "trashcan.csv") || strstr(entry->d_name, "parkinglot.csv")) {
                strcpy(shm, entry->d_name);
                shm += strlen(entry->d_name) + 1;
            } else {
                remove(filename);
            }
        }
    }

    *shm = '\0';
    closedir(dir);
    shmdt(shm);
    return 0;
}
