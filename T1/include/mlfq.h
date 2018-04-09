#ifndef MLFQ_H
#define MLFQ_H

#define FIN_SIGNAL 1 // Finished signal
#define DOWN_QUEUE 2 // Decrement queue
#define SAME_QUEUE 3 // Same queue

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <assert.h>
#include <errno.h>
#include "include/structs.h"

// Inicializa y retorna la estructura mlfq a partir de los parametros
MLFQ* mlfq_init(char* buffer, int quantum, int queues);

// Crea y retorna un proceso a partir del string que lo define
Process* crear_proceso(char string[], int PID);

// Retorna el buffer completo del archivo
char* get_buffer(char filename[]);

// Retorna lista de procesos a partir del buffer del archivo
ArrayList* get_procesos(char* buffer);

//Mete id de proceso en la cola (LinkedList)
void entra_proceso(Process* p, LinkedList* colas, int num, bool set_time);

//Mete id de proceso en la cola (LinkedList)
void baja_prioridad(Process* p, LinkedList* colas);

//Selecciona proceso a ejecutar
void seleccionar_proceso(MLFQ* mlfq);

// Recorre la lista y ve quienes ya estan listos para entrar a la Queue
void check_entry_times(MLFQ* mlfq);

void getAllButFirstAndLast(const char *input, char *output);

// Decrementa los contadores de p y revisa si agota un burst o exec_time
void decrement_counters(Process* p, int* status);

// Baja o reingresa de queue al proceso que se esta ejecutando
void update_queue(MLFQ* mlfq, bool downgrade);

#endif
