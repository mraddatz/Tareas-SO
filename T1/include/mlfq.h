#ifndef MLFQ_H
#define MLFQ_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>

typedef struct {
    int PID;
    int entry_time;
    char nombre[256];
    char estado[20];
    int *bursts;
} Process;

// Crea y retorna un proceso a partir del string que lo define
Process* crear_proceso(char string[], int PID);

// Convierte un numero en formato char a int
int str2int(char ch);

// Retorna el buffer completo del archivo
char* get_buffer(char filename[]);

// Retorna lista de procesos a partir del buffer del archivo
void* get_procesos(char* buffer);

#endif
