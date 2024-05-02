// paddock.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <time.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>

#define PORT 8080

extern char *selisih(float jarak);
extern char *bensin(float persenan);
extern char *ban(int sisa);
extern char *gantiBan(char *type);

void logMessage(char *source, char *command, char *additionalInfo) {
    FILE *logFile = fopen("/home/rafaelega24/SISOP/modul3/3/server/race.log", "a");
    if (logFile == NULL) {
        printf("Error opening log file\n");
        return;
    }

    time_t currentTime = time(NULL);
    struct tm *localTime = localtime(&currentTime);

    fprintf(logFile, "[%s] [%02d/%02d/%04d %02d:%02d:%02d]: [%s] [%s]\n",
            source,
            localTime->tm_mday, localTime->tm_mon + 1, localTime->tm_year + 1900,
            localTime->tm_hour, localTime->tm_min, localTime->tm_sec,
            command, additionalInfo);

    fclose(logFile);
}

void bikinDaemon() {
    pid_t pid;

    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    if (setsid() < 0) {
        exit(EXIT_FAILURE);
    }

    umask(0);

    chdir("/");

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);
}

int main() {
    bikinDaemon();
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addr_size;
    char buffer[1024];

    // Create the server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket < 0) {
        printf("Error creating socket\n");
        return -1;
    }

    // Configure server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket to the server address
    if (bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0) {
        printf("Error binding socket\n");
        return -1;
    }

    // Listen for incoming connections
    if (listen(serverSocket, 1) < 0) {
        printf("Error listening\n");
        return -1;
    }

    printf("Server is listening on port %d\n", PORT);

    while (1) {
        printf("Waiting for incoming connections...\n");

        addr_size = sizeof(clientAddr);
        clientSocket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_size);
        if (clientSocket < 0) {
            printf("Error accepting connection\n");
            continue;
        }

        printf("Connection accepted from %s:%d\n", inet_ntoa(clientAddr.sin_addr), ntohs(clientAddr.sin_port));

        while (1) {
            printf("Waiting for data from client...\n");
            memset(buffer, 0, sizeof(buffer));
            int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
            if (bytesReceived < 0) {
                printf("Error receiving data\n");
                break;
            } else if (bytesReceived == 0) {
                printf("Client disconnected\n");
                break;
            }

            printf("Received data: %s\n", buffer);

            char command[32];
            char value[32];
            sscanf(buffer, "%s %s", command, value);

            char *response;
            if (strcmp(command, "Gap") == 0) {
                float jarak = atof(value);
                response = selisih(jarak);
                logMessage("Driver", command, value);
                logMessage("Paddock", command, response);
            } else if (strcmp(command, "Fuel") == 0) {
                float persenan = atof(value);
                response = bensin(persenan);
                logMessage("Driver", command, value);
                logMessage("Paddock", command, response);
            } else if (strcmp(command, "Tire") == 0) {
                int sisa = atoi(value);
                response = ban(sisa);
                logMessage("Driver", command, value);
                logMessage("Paddock", command, response);
            } else if (strcmp(command, "TireChange") == 0) {
                response = gantiBan(value);
                logMessage("Driver", command, value);
                logMessage("Paddock", command, response);
            }else {
                response = "Invalid command";
                logMessage("Driver", command, response);
            }

            send(clientSocket, response, strlen(response), 0);
        }

        close(clientSocket);
    }

    close(serverSocket);
    return 0;
}
