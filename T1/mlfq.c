#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <include/arraylist.h>
#include <include/linkedlist.h>
#include <include/mlfq.h>

int main(int argc, char *argv[]){
    char l[] = "proc_1 3 5 6 3";
    char l2[] = "proc_2 5 2 2 10 4 5";
    ArrayList* lista = arraylist_init();

    Process* proceso = crear_proceso(l, 1);
    Process* proceso2 = crear_proceso(l2, 2);
    
    arraylist_append(lista, proceso);
    arraylist_append(lista, proceso2);

    Process p1 = arraylist_get(lista, 0);
    Process p2 = arraylist_get(lista, 1);

    printf("%s con bursts %d %d %d y PID %d\n", p1.nombre, p1.bursts[0], 
    p1.bursts[1], p1.bursts[2], p1.PID);
    printf("%s con bursts %d %d %d %d %d y PID %d\n", proceso2->nombre, proceso2->bursts[0], 
    proceso2->bursts[1], proceso2->bursts[2], proceso2->bursts[3], proceso2->bursts[4],
    proceso2->PID);

    free(proceso);
    free(proceso2);
    arraylist_destroy(lista);
    return 0;
}

Process* crear_proceso(char string[], int PID){
    char *ch;
    int i = 0;
    int N;
    Process* proceso = malloc(sizeof(Process));
    proceso->PID = PID;

    ch = strtok(string, " ");
    while (ch != NULL){
        // Primer elemento es el nombre
        if (i == 0) {
            strcpy(proceso->nombre, ch);
        }
        // Segundo elemento es N
        else if (i == 1) {
            N = str2int(*ch);
            proceso->bursts = malloc(N * sizeof(int));
        }
        else {
            proceso->bursts[i-2] = str2int(*ch);

        };
        ch = strtok(NULL, " ");
        i++;
    }

    return proceso;
};

int str2int(char ch) {
    int num = ch - '0';
    return num;
}