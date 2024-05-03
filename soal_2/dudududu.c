#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <time.h>

void balikinJadiString(int result, char *intConverted) {
    char *ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};
    char *tens[] = {"", "", "dua puluh", "tiga puluh", "empat puluh", "lima puluh", "enam puluh", "tujuh puluh", "delapan puluh", "sembilan puluh"};
    char *teens[] = {"sepuluh", "sebelas", "dua belas", "tiga belas", "empat belas", "lima belas", "enam belas", "tujuh belas", "delapan belas", "sembilan belas"};

    int num = result;
    int tens_digit = num / 10;
    int ones_digit = num % 10;

    if (num < 10) strcpy(intConverted, ones[num]);
    else if (num >= 10 && num < 20) strcpy(intConverted, teens[ones_digit]);
    else if (num >= 20 && num < 100) sprintf(intConverted, "%s %s", tens[tens_digit], ones[ones_digit]);
    else strcpy(intConverted, "Terlalu besar");
}

int gantiKeAngka(char *str) {
    char *ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};
    int i;
    for (i = 0; i < 10; i++) {
        if (strcmp(str, ones[i]) == 0)
            return i;
    }
    return -1;
}

void buatLog(char *type, char *pesan) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    FILE *log = fopen("/home/rafaelega24/SISOP/modul3/2/histori.log", "a");
    if (log == NULL) {
        fprintf(stderr, "Error opening file");
        return;
    }

    fprintf(log, "[%02d/%02d/%04d %02d:%02d:%02d] [%s] %s\n",
            tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900, tm.tm_hour, tm.tm_min, tm.tm_sec,
            type, pesan);

    fclose(log);
}

int main(int argc, char *argv[]) {
    int fd1[2];
    int fd2[2];

    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        fprintf(stderr, "Pipe Failed");
        return 1;
    }

    char input_str1[100], input_str2[100];
    scanf("%s %s", input_str1, input_str2);

    pid_t p = fork();

    if (p < 0) {
        fprintf(stderr, "fork Failed");
        return 1;
    } else if (p > 0) {
        int num1 = gantiKeAngka(input_str1);
        int num2 = gantiKeAngka(input_str2);

        int result;
        char operation[20];
        char type[10];
        if (strcmp(argv[1], "-kali") == 0) {
            result = num1 * num2;
            strcpy(operation, "perkalian");
            strcpy(type, "KALI");
        } else if (strcmp(argv[1], "-tambah") == 0) {
            result = num1 + num2;
            strcpy(operation, "penjumlahan");
            strcpy(type, "TAMBAH");
        } else if (strcmp(argv[1], "-kurang") == 0) {
            result = num1 - num2;
            strcpy(operation, "pengurangan");
            strcpy(type, "KURANG");
        } else if (strcmp(argv[1], "-bagi") == 0) {
            if (num2 == 0) {
                printf("ERROR pada pembagian, tidak dapat membagi dengan 0.\n");
            } else {
                result = num1 / num2;
                strcpy(operation, "pembagian");
                strcpy(type, "BAGI");
            }
        }

        close(fd1[0]);
        write(fd1[1], &result, sizeof(int));
        write(fd1[1], operation, strlen(operation) + 1);
        close(fd1[1]);

        wait(NULL);

        close(fd2[1]);
        char result_str[100];
        read(fd2[0], result_str, sizeof(result_str));
        close(fd2[0]);

        buatLog(type, result_str);
    } else {
        int result;
        char operation[20];
        char result_str[200];
        char intConverted[100];

        close(fd1[1]);
        read(fd1[0], &result, sizeof(int));
        read(fd1[0], operation, sizeof(operation));
        close(fd1[0]);

        if (result < 0) {
            strcpy(result_str, "ERROR");
        } else {
            int len = snprintf(result_str, sizeof(result_str), "hasil %s %s dan %s adalah ", operation, input_str1, input_str2);
            balikinJadiString(result, intConverted);
            strncat(result_str, intConverted, sizeof(result_str) - len - 1); // Memastikan tidak terjadi overflow
            strncat(result_str, ".", sizeof(result_str) - strlen(result_str) - 1);
        }

        printf("%s\n", result_str);

        close(fd2[0]);
        write(fd2[1], result_str, strlen(result_str) + 1);
        close(fd2[1]);

        exit(0);
    }
}
