#ifndef STRUCTS_H
#define STRUCTS_H

// Process
typedef struct {
    int PID;
    int entry_time;
    char nombre[256];
    char estado[20];
    int exec_time;
    int cola;
    int* bursts;
    int burst_count;
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


// LnkedList
typedef struct {
    LinkedListNode *root;
    LinkedListNode *last;
    int size;
    int quantum;
} LinkedList;

// MLFQ
typedef struct {
    ArrayList* processes;
    LinkedList* queues;
    Process* executing_proc;
    int timer;
} MLFQ;

#endif
