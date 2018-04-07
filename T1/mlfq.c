
#include "include/arraylist.h"
//#include "include/linkedlist.h"
#include "include/mlfq.h"

int main(int argc, char *argv[]){
    char* buffer = get_buffer(argv[1]);
    int queues = atoi(argv[2]);
    // char l[] = "proc_1 3 5 6 3";
    // char l2[] = "proc_2 5 2 2 10 4 5";
    ArrayList* lista = (ArrayList*)get_procesos(buffer);
    LinkedList* queues_list[queues];
    int i ;
    for (i=0; i<queues; i++){
        LinkedList* cola = linkedlist_init();
        queues_list[i] = cola;    
    };
    // Process* proceso = crear_proceso(l, 1);
    // Process* proceso2 = crear_proceso(l2, 2);
    
    // arraylist_append(lista, proceso);
    // arraylist_append(lista, proceso2);

    Process p1 = arraylist_get(lista, 0);
    Process p2 = arraylist_get(lista, 1);

    printf("%s con bursts %d %d %d y PID %d\n", p1.nombre, p1.bursts[0], 
    p1.bursts[1], p1.bursts[2], p1.PID);
    printf("%s con bursts %d %d %d %d y PID %d\n", p2.nombre, p2.bursts[1], 
    p2.bursts[1], p2.bursts[2], p2.bursts[3], p2.PID);

    //Process p4 = arraylist_get(lista, 3);
    //printf("%s\n con bursts %d %d y PID", p4.nombre, p4.bursts[]);

    entra_proceso(1, *queues_list);

    printf("%i\n", queues_list[0]->size);
    printf("%i\n", queues_list[1]->size);

    //printf("%i\n", linkedlist_get(queues_list[0], 0));

    // free(proceso);
    // free(proceso2);
    arraylist_destroy(lista);
    return 0;
}

Process* crear_proceso(char string[], int PID){
    char* ch;
    char* aux = string;
    int i = 0;
    int N;
    Process* proceso = malloc(sizeof(Process));
    proceso->PID = PID;
    while( (ch = strsep(&aux," ")) != NULL ){
        // Primer elemento es el nombre
        if (i == 0) {
            strcpy(proceso->nombre, ch);
        }
        // Segundo elemento es N
        else if (i == 1) {
            N = atoi(ch);
            proceso->bursts = malloc(N * sizeof(int));
        }
        else {
            proceso->bursts[i-2] = atoi(ch);

        };
        i++;
    }

    return proceso;
};


char* get_buffer(char filename[]){
    FILE    *infile;
    char    *buffer;
    long    numbytes;

    infile = fopen(filename, "r");
    if(infile == NULL)
    {
        return "Error";
    }
    //Lee numero de bytes del archivo
    fseek(infile, 0L, SEEK_END);
    numbytes = ftell(infile);

    //resetear el indicador de posision del file al principio del file
    fseek(infile, 0L, SEEK_SET);

    //Tomar la memoria necesaria para que el buffer contenga al texto
    buffer = (char*)calloc(numbytes, sizeof(char));

    //controlamos memory error
    if(buffer == NULL)
    return "Error";

    //Copiamos el texto al buffer
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);

    return buffer;
    // printf("number of bytes: %ld\n", numbytes);
    // printf("The file  contains this text\n\n%s", buffer);
}

void* get_procesos(char* buffer){
    //Algoritmo para separar por lineas (el mismo sirve para separar palabras, 
    //cambiar el "\n" por " ")
    char *ch;
    ArrayList* lista = arraylist_init();
    int pid = 1;
    while( (ch = strsep(&buffer,"\n")) != NULL ){
        Process* p = crear_proceso(ch, pid);
        arraylist_append(lista, p);
        //Acá, para cada linea haría el proceso
        pid++;
    }

    free(buffer);
    return lista;
}

void entra_proceso(int id, LinkedList* colas){
    printf("%s\n", "Entro proceso");
    linkedlist_append(&colas[0], id);
}

void 

