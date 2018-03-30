#include <stdio.h>
#include <string.h>
#include <stdlib.h>


struct Process {
    int PID;
    char nombre[256];
    char estado[20];
    int *bursts;
};

typedef struct Process Proc;

void crear_proceso(char string[], int PID);

int str2int(char ch);

int main(int argc, char *argv[]){
    char l[] = "proc_1 3 5 6 3";
    crear_proceso(l, 5);
    return 0;
}

void crear_proceso(char string[], int PID){
    char *ch;
    int i = 0;
    int N;
    struct Process proceso;
    proceso.PID = PID;

    ch = strtok(string, " ");
    while (ch != NULL){
        // Primer elemento es el nombre
        if (i == 0) {
            printf("debug\n\n\n");
            strcpy(proceso.nombre, ch);
        }
        // Segundo elemento es N
        else if (i == 1) {
            N = str2int(*ch);
            proceso.bursts = malloc(N * sizeof(int));
        }
        else {
            proceso.bursts[i-2] = str2int(*ch);

        };
        ch = strtok(NULL, " ");
        i++;
    }

    printf("%s con bursts %d %d %d y PID %d", proceso.nombre, proceso.bursts[0], 
    proceso.bursts[1], proceso.bursts[2], proceso.PID);
};

int str2int(char ch) {
    int num = ch - '0';
    return num;
}