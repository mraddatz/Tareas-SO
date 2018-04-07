
#include "include/arraylist.h"
//#include "include/linkedlist.h"
#include "include/mlfq.h"

int main(int argc, char *argv[]){
    char* buffer = get_buffer(argv[1]);
    ArrayList* lista = (ArrayList*)get_procesos(buffer);
    int tick = 0;
    int queues = atoi(argv[2]);
    LinkedList* queues_list[queues];
    int i ;
    for (i=0; i<queues; i++){
        LinkedList* cola = linkedlist_init();
        queues_list[i] = cola;    
    };

    while (true){
        printf("tick...\n");
        check_entry_times(lista, tick);
        sleep(1);
        tick++;
        if(tick == 10) break;
    }


    entra_proceso(1, *queues_list);
    arraylist_destroy(lista);
    return 0;
}

// Crea y retorna un proceso a partir del string que lo define
Process* crear_proceso(char string[], int PID){
    char* ch;
    char* aux = string;
    int i = 0;
    int N, T;
    Process* proceso = malloc(sizeof(Process));
    proceso->PID = PID;
    while( (ch = strsep(&aux," ")) != NULL ){
        // Primer elemento es el nombre
        if (i == 0) {
            strcpy(proceso->nombre, ch);
        }
        // Segundo elemento es T
        else if (i == 1) {
            T = atoi(ch);
            proceso->entry_time = T;
        }
        // Tercer elemento es N
        else if (i == 2) {
            N = atoi(ch);
            proceso->bursts = malloc(N * sizeof(int));
        }
        else {
            proceso->bursts[i-3] = atoi(ch);

        };
        i++;
    }

    return proceso;
};

// Retorna el buffer completo del archivo
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

// Retorna lista de procesos a partir del buffer del archivo
void* get_procesos(char* buffer){
    //Algoritmo para separar por lineas (el mismo sirve para separar palabras, 
    //cambiar el "\n" por " ")
    char *ch;
    ArrayList* lista = arraylist_init();
    int pid = 1;
    while( (ch = strsep(&buffer,"\n")) != NULL ){
        Process* p = crear_proceso(ch, pid);
        arraylist_append(lista, p);
        pid++;
    }

    free(buffer);
    return lista;
}

<<<<<<< HEAD
void entra_proceso(int id, LinkedList* colas){
    printf("%s\n", "Entro proceso");
    linkedlist_append(&colas[0], id);
}
=======
// Mete a la queue a los procesos que les toque entrar
void check_entry_times(void* lista, int tick) {
    Process* p;

    for (int i=0; i < ((ArrayList*)lista)->count; i++){
        p = arraylist_get(lista, i);
        if (p->entry_time == tick){
            printf("%s entra a la cola en T=%d (PID=%d)\n", p->nombre, tick, p->PID);
        }
    }
}
>>>>>>> matias
