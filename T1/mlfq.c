
#include "include/arraylist.h"
#include "include/linkedlist.h"
#include "include/mlfq.h"

int main(int argc, char *argv[]){
    char* buffer     = get_buffer(argv[1]);
    int queues       = atoi(argv[2]);
    ArrayList* lista = (ArrayList*)get_procesos(buffer);
    int tick         = 0;

    LinkedList* queues_list[queues];
    int i;

    for (i=0; i<queues; i++){
        LinkedList* cola = linkedlist_init();
        queues_list[i] = cola;    
    };

    while (true){
        printf("tick...\n");
        check_entry_times(lista, tick, queues_list[0]);
        // sleep(1);
        tick++;
        if (tick == 10) break;
    }
    Process* p = linkedlist_get(queues_list[0], 0);
    char* status = "";
    p->exec_time = 10;
    for (int i = 0; i < 15; i++) {
        decrement_counters(p, &status);
        printf("Exec time: %d\nPrimer burst: %d\nCodigo: %s\n",
        p->exec_time, p->bursts[0], status);
    }

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
            proceso->burst_count = N;
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

// Mete proceso en la queue (LinkedList)
void entra_proceso(Process* p, void* queue){
    printf("%s entra a la cola (PID=%d)\n", p->nombre, p->PID);
    linkedlist_append((LinkedList*)queue, p);
    
}
// Recorre la lista (ArrayList) y ve quienes ya estan listos para entrar a la Queue(LinkedList)
void check_entry_times(void* lista, int tick, void* queue){
    Process* p;

    for (int i=0; i < ((ArrayList*)lista)->count; i++){
        p = arraylist_get(lista, i);
        if (p->entry_time == tick){
            entra_proceso(p, queue);
        }
    }
}

// Decrementa los contadores de p y revisa si agota un burst o exec_time
void decrement_counters(Process* p, char** status){
    for (int i=0; i < p->burst_count; i++) {
        if (p->bursts[i] != 0) {
            p->bursts[i]--;
            if (p->bursts[i] == 0) {
                *status = "SAME_QUEUE";
                if (i == p->burst_count - 1) {
                    *status = "FINISHED";
                }
            }
            break;
        }
    }
    p->exec_time--;
    if (p->exec_time == 0) {
            *status = "DECR_QUEUE";
        return;
    }
}