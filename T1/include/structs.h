#ifndef STRUCTS_H
#define STRUCTS_H

// Process
typedef struct {
    int PID;
    int entry_time;
    char nombre[256];
    int exec_time;
    int cola;
    int burst_count;
    int* bursts;
    //0 es No ha entrado, 1 es READY, 2 es IN PROCESS, 3 es Terminado
    int estado;
    int cpu_turns;
    int bloqueos;
    int finish_time;
    int waiting_time;
    int response_time;
} Process;

// ArrayList
typedef struct {
    int count;
    int size;
    Process* pointer;
} ArrayList;

// LinkedListNode
typedef struct linked_list_node {
    Process* data;
    struct linked_list_node *next;
} LinkedListNode;


// LinkedList
typedef struct {
    LinkedListNode *root;
    LinkedListNode *last;
    int size;
    int quantum;
} LinkedList;

// MLFQ
typedef struct {
    ArrayList* procs;
    ArrayList* finished_procs;
    LinkedList* queues;
    Process* executing_proc;
    unsigned int num_queues;
    unsigned int num_procs;
    unsigned int timer;
} MLFQ;

#endif
