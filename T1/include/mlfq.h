#ifndef MLFQ_H
#define MLFQ_H

typedef struct {
    int PID;
    char nombre[256];
    char estado[20];
    int *bursts;
} Process;

Process* crear_proceso(char string[], int PID);

int str2int(char ch);

#endif
