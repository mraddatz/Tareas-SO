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
#include "include/structs.h"

// Crea y retorna un proceso a partir del string que lo define
Process* crear_proceso(char string[], int PID);

// Retorna el buffer completo del archivo
char* get_buffer(char filename[]);

// Retorna lista de procesos a partir del buffer del archivo
void* get_procesos(char* buffer);

//Mete id de proceso en la cola (LinkedList)
void entra_proceso(Process* p, LinkedList* colas);

//Mete id de proceso en la cola (LinkedList)
void baja_prioridad(Process* p, LinkedList* colas);

// Recorre la lista y ve quienes ya estan listos para entrar a la Queue
void check_entry_times(ArrayList* lista, int tick, LinkedList* queue);

void getAllButFirstAndLast(const char *input, char *output);

// Decrementa los contadores de p y revisa si agota un burst o exec_time
void decrement_counters(Process* p, char** status);

#endif
