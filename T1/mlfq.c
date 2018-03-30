#include <stdio.h>
#include <string.h>


struct Process {
    int PID;
    char nombre[256];
    char estado[20];
    int bursts[4];
};

typedef struct Process Proc;

void crear_proceso(char string[], int PID);

int str2int(char ch);

int main(int argc, char *argv[]){
    char l[] = "proc_1 4 5 6 3";
    crear_proceso(l, 5);
    return 0;
}

void crear_proceso(char string[], int PID){
    char *ch;
    int i;
    i = 0;

    Proc proceso;
    proceso.PID = PID;

    proceso.bursts[0]=3;
    proceso.bursts[3]=4;

    ch = strtok(string, " ");
    while (ch != NULL){
        if (i == 0) strcpy(proceso.nombre, ch);
        else {
            proceso.bursts[i-1] = str2int(*ch);

        };
        ch = strtok(NULL, " ");
        i++;
    }

    printf("%s con bursts %d %d %d %d y PID %d", proceso.nombre, proceso.bursts[0], 
    proceso.bursts[1], proceso.bursts[2], proceso.bursts[3], proceso.PID);
};

int str2int(char ch) {
    int num = ch - '0';
    return num;
}