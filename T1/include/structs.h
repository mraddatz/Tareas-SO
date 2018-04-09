#ifndef STRUCTS_H
#define STRUCTS_H

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

/** Estructura de una arraylist */
struct arraylist_list
{
    int count;
    int size;
    Process* pointer;
};

/** Estructura de un node de lista ligada */
struct linked_list_node
{
    Process* data;
    struct linked_list_node *next;
};

typedef struct linked_list_node LinkedListNode;

/** Estructura de una lista ligada */
struct linked_list
{
    LinkedListNode *root;
    LinkedListNode *last;
    int size;
    int quantum;
};

#endif
