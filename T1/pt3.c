#include <stdio.h>
#include <string.h>


struct Process {
    int PID;
    char nombre[256];
    char estado[20];
    int bursts[];
};

typedef struct Process Proc;

void crear_proceso(char string[]);

int str2int(char ch);

int main(int argc, char *argv[]){
    char l[] = "proc_1 4 5 6 3";
    crear_proceso(l, 5);
    /* Proc proceso1;
    proceso1.PID = *argv[1] - '0';
    strcpy(proceso1.nombre, argv[2]);
    strcpy(proceso1.estado, "WAITING");

    printf("El proceso %s con PID %d esta en estado %s\n", proceso1.nombre, proceso1.PID, proceso1.estado); */
    return 0;
}

void crear_proceso(char string[], int PID){
    char *ch;
    int i;
    Proc proceso;
    proceso.PID = PID;
    ch = strtok(string, " ");
    while (ch){
        if (i == 0) strcpy(proceso.nombre, ch);
        else {
            proceso.bursts[i-1] = str2int(ch);
        }
        ch = strtok(NULL, " ");
        i++;
    }
    printf("%s con bursts %d %d %d %d", proceso.nombre, [proceso.bursts]);
};

int str2int(char ch) {
    int num = ch - "0";
    return num;
}