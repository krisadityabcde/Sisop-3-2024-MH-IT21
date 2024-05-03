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

    char filename[256] = "";
    char type[20] = "";
    char name[100] = "";
    float rating, max_rating = 0.0;
    char highest_name[100] = "";
    while (*shm != '\0') {
        strcpy(filename, shm);
        shm += strlen(shm) + 1;

        if (strstr(filename, "trashcan.csv")) {
            strcpy(type, "Trash Can");
        } else if (strstr(filename, "parkinglot.csv")) {
            strcpy(type, "Parking Lot");
        }

        char filepath[512];
        sprintf(filepath, "/home/rafaelega24/SISOP/modul3/1/new-data/%s", filename);

        FILE *file = fopen(filepath, "r");
        if (file == NULL) {
            printf("Cannot open file: %s\n", filepath);
            continue;
        }

        fscanf(file, "%*s,%*s"); // skip header line
        strcpy(name, ""); // Reset name before reading file
        while (fscanf(file, "%[^,],%f", name, &rating) == 2) {
            if (rating > max_rating) {
                max_rating = rating;
                strcpy(highest_name, name); // Copy the name with highest rating
            }
        }

        fclose(file);
    }

    printf("Type: %s\nFilename: %s\n\n--------------------------------\n\nName: %s\nRating: %.1f\n", type, filename, highest_name, max_rating);

    shmdt(shm);
    //shmctl(shmid, IPC_RMID, NULL);
    return 0;
}
