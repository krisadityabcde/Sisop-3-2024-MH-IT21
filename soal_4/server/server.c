#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <time.h>
#include <netinet/in.h>

#define PORT 8080

void handleRequest(char *buffer, char *response) {
    FILE *file = fopen("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv", "r");
    FILE *logFile = fopen("/home/rafaelega24/SISOP/modul3/4/change.log", "a");
    char line[1024];
    char command[1024];
    char parameter[1024];
    int found = 0;

    sscanf(buffer, "%s %[^\n]", command, parameter);

    if (strcmp(command, "tampilkan") == 0) {
    int count = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            char *token = strtok(line, ",");
            token = strtok(NULL, ",");
            token = strtok(NULL, ",");
            char numberedLine[1028];
            sprintf(numberedLine, "%d. %s", count, token);
            strcat(response, numberedLine);
            strcat(response, "\n");
            count++;
        }
    }
    } else if (strcmp(command, "hari") == 0) {
    rewind(file);
    int count = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            char *token = strtok(line, ",");
            if (strcmp(token, parameter) == 0) {
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
                char numberedLine[1028];
                sprintf(numberedLine, "%d. %s", count, token);
                strcat(response, numberedLine);
                strcat(response, "\n");
                found = 1;
                count++;
            }
        }
    }
    if (!found) {
        strcat(response, "Tidak ada anime pada hari tersebut\n");
    }
    } else if (strcmp(command, "genre") == 0) {
    rewind(file);
    int count = 1;
    while (fgets(line, sizeof(line), file) != NULL) {
        if (line[0] != '\n') {
            char *token = strtok(line, ",");
            token = strtok(NULL, ",");
            if (strcmp(token, parameter) == 0) {
                token = strtok(NULL, ",");
                char numberedLine[1028];
                sprintf(numberedLine, "%d. %s", count, token);
                strcat(response, numberedLine);
                strcat(response, "\n");
                found = 1;
                count++;
            }
        }
    }
    if (!found) {
        strcat(response, "Tidak ada anime dengan genre tersebut\n");
    }
    } else if (strcmp(command, "status") == 0) {
        rewind(file);
        while (fgets(line, sizeof(line), file) != NULL) {
            if (line[0] != '\n') {
                char *token = strtok(line, ",");
                token = strtok(NULL, ",");
                token = strtok(NULL, ",");
                if (strcmp(token, parameter) == 0) {
                    token = strtok(NULL, "\n");
                    strcat(response, token);
                    strcat(response, "\n");
                    found = 1;
                }
            }
        }
        if (!found) {
            strcat(response, "Tidak ada anime dengan status tersebut\n");
        }
    }else if (strcmp(command, "add") == 0) {
        rewind(file);
        FILE *temp = fopen("/home/rafaelega24/SISOP/modul3/4/temp.csv", "w");
        while (fgets(line, sizeof(line), file) != NULL) {
            fputs(line, temp);
        }
        fprintf(temp, "\n%s", parameter);
        fclose(temp);
        fclose(file);
        remove("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv");
        rename("/home/rafaelega24/SISOP/modul3/4/temp.csv", "/home/rafaelega24/SISOP/modul3/4/myanimelist.csv");
        file = fopen("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv", "r");

        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char dateStr[20];
        strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

        char *judul = strtok(parameter, ",");
        judul = strtok(NULL, ",");
        judul = strtok(NULL, ",");

        fprintf(logFile, "[%s] [ADD] %s ditambahkan.\n", dateStr, judul);
        strcat(response, "anime berhasil ditambahkan\n");

    } else if (strcmp(command, "edit") == 0) {
    rewind(file);
    FILE *temp = fopen("/home/rafaelega24/SISOP/modul3/4/temp.csv", "w");

    char *oldJudul = strtok(parameter, ",");
    char *newLine = strtok(NULL, "\n");

    while (fgets(line, sizeof(line), file) != NULL) {
        char *oldLine = strdup(line);
        char *token = strtok(line, ",");
        token = strtok(NULL, ",");
        token = strtok(NULL, ",");
        char *judul = token;

        if (strcmp(judul, oldJudul) != 0) {
            fputs(oldLine, temp);
        } else {
            fprintf(temp, "%s\n", newLine);

            time_t now = time(NULL);
            struct tm *t = localtime(&now);
            char dateStr[20];
            strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

            fprintf(logFile, "[%s] [EDIT] %s diubah menjadi %s.\n", dateStr, oldLine, newLine);
        }

        free(oldLine);
    }

    fclose(temp);
    fclose(file);
    remove("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv");
    rename("/home/rafaelega24/SISOP/modul3/4/temp.csv", "/home/rafaelega24/SISOP/modul3/4/myanimelist.csv");
    file = fopen("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv", "r");
    strcat(response, "anime berhasil diedit\n");
} else if (strcmp(command, "delete") == 0) {
        rewind(file);
        FILE *temp = fopen("/home/rafaelega24/SISOP/modul3/4/temp.csv", "w");
        char *judul = parameter;
        while (fgets(line, sizeof(line), file) != NULL) {
            char *oldLine = strdup(line);
            char *judulLine= strtok(line, ",");
            judulLine = strtok(NULL, ",");
            judulLine = strtok(NULL, ",");

            if (strcmp(judulLine, judul) != 0) {
                fputs(oldLine, temp);
            } else {
                time_t now = time(NULL);
                struct tm *t = localtime(&now);
                char dateStr[20];
                strftime(dateStr, sizeof(dateStr), "%d/%m/%y", t);

                fprintf(logFile, "[%s] [DEL] %s berhasil dihapus.\n", dateStr, judul);
            }
        }
        fclose(temp);
        fclose(file);
        remove("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv");
        rename("/home/rafaelega24/SISOP/modul3/4/temp.csv", "/home/rafaelega24/SISOP/modul3/4/myanimelist.csv");
        file = fopen("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv", "r");
        strcat(response, "anime berhasil dihapus\n");
    }else {
        strcat(response, "Invalid Command\n");
    }

    fclose(file);
    fclose(logFile);
}

int main() {
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    int addrlen = sizeof(addr_size);
    char buffer[1024];
    char response[4096];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Error binding socket\n");
        return -1;
    }

    if (listen(serverSocket, 1) < 0) {
        printf("Error listening\n");
        return -1;
    }

    while (1) {

        addr_size = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);
        if (clientSocket < 0) {
            printf("Error accepting connection\n");
            continue;
        }

        while (1) {
            memset(buffer, 0, sizeof(buffer));
            recv(clientSocket, buffer, sizeof(buffer), 0);

            printf("received: %s\n", buffer);
            if (strcmp(buffer, "exit") == 0) {
                printf("Closing connection\n");
                exit(0);
            }
            memset(response, 0, sizeof(response));
            handleRequest(buffer, response);
            send(clientSocket, response, strlen(response), 0);

            
        }
    close(clientSocket);
}
    close(serverSocket);
    return 0;
}
