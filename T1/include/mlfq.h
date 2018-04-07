#ifndef MLFQ_H
#define MLFQ_H

#define FIN_SIGNAL // Finished signal
#define DECR_QUEUE // Decrement queue
#define SAME_QUEUE // Same queue

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
    int burst_count;
    int* bursts;
    int exec_time;
} Process;

// Crea y retorna un proceso a partir del string que lo define
Process* crear_proceso(char string[], int PID);

// Retorna el buffer completo del archivo
char* get_buffer(char filename[]);

// Retorna lista de procesos a partir del buffer del archivo
void* get_procesos(char* buffer);

// Mete proceso en la queue (LinkedList)
void entra_proceso(Process* p, void* queue);

// Recorre la lista (ArrayList) y ve quienes ya estan listos para entrar a la Queue(LinkedList)
void check_entry_times(void* lista, int tick, void* Queue);

// Decrementa los contadores de p y revisa si agota un burst o exec_time
void decrement_counters(Process* p, char** status);

#endif
