# Sisop-3-2024-MH-IT21
## Anggota Kelompok:
- Rafael Ega Krisaditya	(5027231025)
- Rama Owarianto Putra Suharjito	(5027231049)
- Danar Bagus Rasendriya	(5027231055)

## Soal 1
### 1a
Dalam `auth.c` pastikan file yang masuk ke folder `new-entry` adalah file `.csv` dan berakhiran  `trashcan` dan `parkinglot`. Jika bukan, program akan secara langsung akan delete file tersebut.
Contoh dari nama file yang akan diautentikasi:
- `belobog_trashcan.csv`
- `osaka_parkinglot.csv`

```c
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
```

### 1b
Format file csv sudah disesuaikan dengan kebutuhan soal dan dibuat secara manual.

### 1c
File csv yang lolos tahap autentikasi akan dikirim ke shared memory. Hal ini dilakukan menggunakan `strcpy(shm, entry->d_name);`.

### 1d
Dalam `rate.c`, proses akan mengambil data csv dari shared memory dan akan memberikan output Tempat Sampah dan Parkiran dengan Rating Terbaik dari data tersebut.
```c
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
```
Pada penggalan program ini, file akan diproses satu-persatu dalam sebuah while loop, yang mana setelah dibuka, isi dari file akan diambil apabila nilai yang baru lebih besar dari nilai yang tersimpan di variabel. Perlu diingat juga bahwa nilai pada file `trashcan` dan `parkinglot` disimpan ke dalam variabel yang berbeda karena permintaan soal untuk menampilkan nilai tertinggi dari masing-masing file tersebut.

Hasil yang telah didapatkan kemudian di print sebagai output dengan
```c
printf("Type: Trash Can\nFilename: %s\n\n--------------------------------\n\nName: %s\nRating: %.1f\n\n", filename, trashcan_name, trashcan_max_rating);
printf("Type: Parking Lot\nFilename: %s\n\n--------------------------------\n\nName: %s\nRating: %.1f\n", filename, parkinglot_name, parkinglot_max_rating);
```

### 1e
Pada `db.c`, proses bisa memindahkan file dari `new-data` ke folder `microservices/database`, **WAJIB MENGGUNAKAN SHARED MEMORY**.
```c
char source[512], dest[512];
        sprintf(source, "/home/rafaelega24/SISOP/modul3/1/new-data/%s", filename);
        sprintf(dest, "/home/rafaelega24/SISOP/modul3/1/microservices/database/%s", filename);

        if (strstr(filename, "trashcan.csv")) {
            strcpy(type, "Trash Can");
        } else {
            strcpy(type, "Parking Lot");
        }

        rename(source, dest);
```
Nama file yang sebelumnya disimpan di shared memory akan digunakan oleh `db.c` untuk memindahkan file menggunakan function `rename()`

### 1f
Log semua file yang masuk ke folder microservices/database ke dalam file db.log dengan contoh format sebagai berikut:
`[DD/MM/YY hh:mm:ss] [type] [filename]` ex : `[07/04/2024 08:34:50] [Trash Can] [belobog_trashcan.csv]`
```c
        time_t now = time(NULL);
        struct tm *t = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp), "%D %H:%M:%S", t);

        fprintf(logfile, "[%s] [%s] [%s]\n", timestamp, type, filename);
```

### Screenshot hasil pengerjaan

### Kendala

### Revisi

## Soal 2
### 2a
Sesuai request dari adiknya Max ingin nama programnya `dudududu.c`. Sebelum program `parent process` dan `child process`, ada input dari user berupa 2 string. Contoh input: tiga tujuh.
```c
char input_str1[100], input_str2[100];
scanf("%s %s", input_str1, input_str2);
```

### 2b
Pada `parent process`, program akan mengubah input menjadi angka dan melakukan perkalian dari angka yang telah diubah. Contoh: tiga tujuh menjadi 21.

Pada `main()`
```c
int num1 = gantiKeAngka(input_str1);
        int num2 = gantiKeAngka(input_str2);

        int result;
        char operation[20];
        char type[10];
        if (strcmp(argv[1], "-kali") == 0) {
            result = num1 * num2;
```

Pada function `gantiKeAngka()`
```c
int gantiKeAngka(char *str) {
    char *ones[] = {"", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};
    int i;
    for (i = 0; i < 10; i++) {
        if (strcmp(str, ones[i]) == 0)
            return i;
    }
    return -1;
}
```

### 2c
Pada `child process`, program akan mengubah hasil angka yang telah diperoleh dari `parent process` menjadi kalimat. Contoh: 21 menjadi “dua puluh satu”.
Hal ini dilaksanakan menggunakan function `balikinJadiString()`
```c
void balikinJadiString(int result, char *intConverted) {
    char *ones[] = {"nol", "satu", "dua", "tiga", "empat", "lima", "enam", "tujuh", "delapan", "sembilan"};
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
```

### 2d
Max ingin membuat program kalkulator juga dapat melakukan penjumlahan, pengurangan, dan pembagian.
Maka dari itu pada main ditambahkan menjadi seperti ini
```c
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
```

### 2e
Di programnya dia ingin (pada child process) kalimat akan di print dengan contoh format : 
1. perkalian	: “hasil perkalian tiga dan tujuh adalah dua puluh satu.”
2. penjumlahan	: “hasil penjumlahan tiga dan tujuh adalah sepuluh.”
3. pengurangan	: “hasil pengurangan tujuh dan tiga adalah empat.”
4. pembagian	: “hasil pembagian tujuh dan tiga adalah dua.”
```c
 int len = snprintf(result_str, sizeof(result_str), "hasil %s %s dan %s adalah ", operation, input_str1, input_str2);
            balikinJadiString(result, intConverted);
            strncat(result_str, intConverted, sizeof(result_str) - len - 1); // Memastikan tidak terjadi overflow
            strncat(result_str, ".", sizeof(result_str) - strlen(result_str) - 1);

        printf("%s\n", result_str);
```

### 2f
Max ingin hasil dari setiap perhitungan dicatat dalam sebuah log yang diberi nama `histori.log`. Pada `parent process`, lakukan pembuatan file log berdasarkan data yang dikirim dari `child process`.
Di main function saya menggunakan `buatLog(type, result_str);` untuk membuat log, untuk functionnya sendiri kurang lebih seperti ini
```c
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
```

### Screenshot hasil pengerjaan

### Kendala

### Revisi

## Soal 3
### 3a
Pada program `actions.c`, program akan berisi function function yang bisa di call oleh `paddock.c`
```c
//actions.c
#include <stdio.h>
#include <string.h>

char *selisih(float jarak) {
    if (jarak < 3.5) {
        return "Gogogo";
    } else if (jarak >= 3.5 && jarak <= 10) {
        return "Push";
    } else {
        return "Stay out of trouble";
    }
}

char *bensin(float persenan) {
    if (persenan > 80) {
        return "Push Push Push";
    } else if (persenan >= 50 && persenan <= 80) {
        return "You can go";
    } else {
        return "Conserve Fuel";
    }
}

char *ban(int sisa) {
    if (sisa > 80) {
        return "Go Push Go Push";
    } else if (sisa >= 50 && sisa <= 80) {
        return "Good Tire Wear";
    } else if (sisa >= 30 && sisa < 50) {
        return "Conserve Your Tire";
    } else {
        return "Box Box Box";
    }
}

char *gantiBan(char *type) {
    if (strcmp(type, "Soft") == 0) {
        return "Mediums Ready";
    } else if (strcmp(type, "Medium") == 0) {
        return "Box for Softs";
    } else {
        return "Invalid";
    }
}
```

```c
//sedikit bagian paddock.c
extern char *selisih(float jarak);
extern char *bensin(float persenan);
extern char *ban(int sisa);
extern char *gantiBan(char *type);
```

Sebenarnya halini juga bisa dilakukan ketika melakukan compile di akhir
```shell
gcc -c actions.c -o actions.o
gcc -c paddock.c -o paddock.o
gcc paddock.o actions.o -o paddock
```
Dengan ini, kita bisa compile tiap program menjadi sebuah `object` lalu kedua `object` akan dicompile menjadi `executable`

### 3b
Isi dari `actions.c` sudah dijelaskan pada soal `3a` sebelumnya

### 3c
Pada `paddock.c` program berjalan secara `daemon` di background, bisa terhubung dengan `driver.c` melalui `socket RPC`.
```c
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
```
Ini adalah function pada paddock untuk menjalankan secara daemon
```c
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
```
Sementara itu, ini adalah template yang digunakan untuk membaut socket RPC yang saya ambil dari modul

### 3d
Program `paddock.c` dapat call function yang berada di dalam `actions.c`. Hal ini sudah dijelaskan sebelumnya

### 3e
Program `paddock.c` tidak keluar/terminate saat terjadi error dan akan log semua percakapan antara `paddock.c` dan `driver.c` di dalam file `race.log`
```c
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
```
Ini adalah function untuk membuat `race.log`

### 3f
Program `driver.c` bisa terhubung dengan `paddock.c` dan bisa mengirimkan pesan dan menerima pesan serta menampilan pesan tersebut dari `paddock.c` sesuai dengan perintah atau function call yang diberikan.
```c
int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0) {
            printf("Error receiving data\n");
            break;
        }

        printf("Server response: %s\n", buffer);
```
Setelah menerima pesan dari `paddock.c` dengan `recv()`, pesan tersebut akan ditampilkan pada terminal `driver.c`

### 3g
Saya tidak tahu bagaimana cara untuk mengetes jika program ini bisa dijalankan di OS yang berbeda.

### 3h
Saya menggunakan metode CLI untuk program ini

### Screenshot hasil pengerjaan

### Kendala

### Revisi

## Soal 4
### 4a
Client dan server terhubung melalui socket. Saya sudah menginisiasi socket dengan metode yang sama seperti yang saya lakukan sebelumnya di soal no 3.

### 4b
client.c di dalam folder client dan server.c di dalam folder server. Sudah dilakukan.

### 4c
Client berfungsi sebagai pengirim pesan dan dapat menerima pesan dari server.
```c
 while (1) {
        printf("You: ");
        memset(buffer, 0, sizeof(buffer));
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';
        send(clientSocket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "exit") == 0) {
           exit(0);
        }

        memset(buffer, 0, sizeof(buffer));
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (bytesReceived < 0) {
            printf("Error receiving data\n");
            break;
        }

        printf("Server:\n%s\n", buffer);
    }
```
Client dapat mengirimkan pesan dan menerima pesan

### 4d
Server berfungsi sebagai penerima pesan dari client dan hanya menampilkan pesan perintah client saja.
```c
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
```
Server dapat menerima pesan dari client dan menampilkan perintah client saja

### 4e
Server digunakan untuk membaca `myanimelist.csv`. Dimana terjadi pengiriman data antara `client` ke `server` dan `server` ke `client`.
- Menampilkan seluruh judul
- Menampilkan judul berdasarkan genre
- Menampilkan judul berdasarkan hari
- Menampilkan status berdasarkan berdasarkan judul
- Menambahkan anime ke dalam file myanimelist.csv
- Melakukan edit anime berdasarkan judul
- Melakukan delete berdasarkan judul
- Selain command yang diberikan akan menampilkan tulisan “Invalid Command”

```c
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
```
Semua perintah yang dibutuhkan sudah tersedia dalam function `handleRequest()` dan dapat diinisiasi sesuai kebutuhannya masing-masing

### 4f
Karena Lewis juga ingin track anime yang ditambah, diubah, dan dihapus. Maka dia membuat server dapat mencatat anime yang dihapus dalam sebuah log yang diberi nama `change.log`.
```c
FILE *file = fopen("/home/rafaelega24/SISOP/modul3/4/myanimelist.csv", "r");
    FILE *logFile = fopen("/home/rafaelega24/SISOP/modul3/4/change.log", "a");
```
Pada bagian awal dari function `handleRequest()` sebenarnya ada file log yang diinisiasi, dapat dilihat juga pada program di poin sebelumnya setiap kali ada anime yang ditambah, diubah, dan dihapus, maka file log ini akan mencatat segala aktivitas yang terjadi sesuai dengan format log yang diinginkan.

### 4g
Koneksi antara `client` dan `server` tidak akan terputus jika ada kesalahan input dari client, cuma terputus jika user mengirim pesan `“exit”`. Program exit dilakukan pada sisi `client`.
```c
if (strcmp(buffer, "exit") == 0) {
           exit(0);
        }
```
Pada `client` jika ada input exit, maka program akan langsung keluar, namun sebenarnya perintah exit ini akan dikirimkan terlebih dahulu ke server, karena jika client tiba-tiba keluar, maka server tidak dapat dimatikan. Oleh karena itu pada server juga terdapat
```c
if (strcmp(buffer, "exit") == 0) {
                printf("Closing connection\n");
                exit(0);
            }
```

### 4h
Format direktori sudah sesuai

### Screenshot hasil pengerjaan

### Kendala

### Revisi
