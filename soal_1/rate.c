#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 1234

int main() {
    key_t key = SHM_KEY;
    int shmid = shmget(key, sizeof(char) * 1024, IPC_CREAT | 0666);
    char *shm = shmat(shmid, NULL, 0);

    char filename[256];
    char trashcan_name[100] = "";
    char parkinglot_name[100] = "";
    float rating, trashcan_max_rating = 0.0, parkinglot_max_rating = 0.0;

    while (*shm != '\0') {
        strcpy(filename, shm);
        shm += strlen(shm) + 1;

        char filepath[512];
        sprintf(filepath, "/home/rafaelega24/SISOP/modul3/1/new-data/%s", filename);

        FILE *file = fopen(filepath, "r");
        if (file == NULL) {
            printf("Cannot open file: %s\n", filepath);
            continue;
        }

        fscanf(file, "%*s,%*s"); // skip header line
        char name[100];
        while (fscanf(file, "%[^,],%f", name, &rating) == 2) {
            if (strstr(filename, "trashcan.csv")) {
                if (rating > trashcan_max_rating) {
                    trashcan_max_rating = rating;
                    strcpy(trashcan_name, name);
                }
            } else if (strstr(filename, "parkinglot.csv")) {
                if (rating > parkinglot_max_rating) {
                    parkinglot_max_rating = rating;
                    strcpy(parkinglot_name, name);
                }
            }
        }

        fclose(file);
    }

    printf("Type: Trash Can\nFilename: %s\n\n--------------------------------\n\nName: %s\nRating: %.1f\n\n", filename, trashcan_name, trashcan_max_rating);
    printf("Type: Parking Lot\nFilename: %s\n\n--------------------------------\n\nName: %s\nRating: %.1f\n", filename, parkinglot_name, parkinglot_max_rating);

    shmdt(shm);
    shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
