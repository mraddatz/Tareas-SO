
#include "include/arraylist.h"
#include "include/linkedlist.h"
#include "include/mlfq.h"

int main(int argc, char *argv[]){
    char* version = argv[1];
    char* buffer  = get_buffer(argv[2]);
    int quantum   = atoi(argv[3]);
    int queues    = atoi(argv[4]);

    MLFQ* mlfq = mlfq_init(buffer, quantum, queues);

    printf("%s\n", version);

    if (strcmp(version, "v1") == 0){
      printf("Ejecutando version1\n");
      while (true){
          printf("tick...\n");
          check_entry_times(mlfq);
          sleep(1);
          mlfq->timer++;
          if(mlfq->timer == 2) break;
      }
    }
    // else if (strcmp(version, "v2") == 0){
    //   char* s = argv[5];
    //   char mod[512];
    //   getAllButFirstAndLast(s, mod);
    //   int s_integer = atoi(mod);
    // }
    //else if (version=='v3'){
    //  printf("Ejecutando version3\n");

    //}

    Process* p = linkedlist_get(&(mlfq->queues[0]), 0);
    char* status = "";
    p->exec_time = 10;
    for (int i = 0; i < 15; i++) {
        decrement_counters(p, &status);
        printf("Exec time: %d\nPrimer burst: %d\nCodigo: %s\n",
        p->exec_time, p->bursts[0], status);
    }

    Process* proc;
    proc = arraylist_get(mlfq->processes, 4);
    entra_proceso(proc, mlfq->queues);
    arraylist_destroy(mlfq->processes);
    linkedlist_append(&(mlfq->queues[1]), proc);
    baja_prioridad(proc,mlfq->queues);
    printf("Id Proceso %i\n",proc->PID);

    return 0;
}

// Inicializa y retorna la estructura mlfq a partir de los parametros
MLFQ* mlfq_init(char* buffer, int quantum, int queues) {
    MLFQ* mlfq = malloc(sizeof(MLFQ));
    mlfq->processes = get_procesos(buffer);
    mlfq->queues = malloc(queues * sizeof(LinkedList));

    for (int i = 0; i < queues; i++){
        mlfq->queues[i] = *(linkedlist_init(quantum));
    };
    
    mlfq->timer = 0;

    return mlfq;
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
ArrayList* get_procesos(char* buffer){
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

void entra_proceso(Process* p, LinkedList* colas){
    printf("Entro proceso PID = %d\n", p->PID);
    //Cambiar quentum y cola de proceso
    p->cola=0;
    p->exec_time = colas[0].quantum;
    linkedlist_append(&(colas[0]), p);
}

// Mete a la queue a los procesos que les toque entrar
void check_entry_times(MLFQ* mlfq) {
    Process* p;

    for (int i=0; i < mlfq->processes->count; i++){
        p = arraylist_get(mlfq->processes, i);
        if (p->entry_time == mlfq->timer){
            entra_proceso(p, &(mlfq->queues[0]));
        }
    }
}

void baja_prioridad(Process* p, LinkedList* colas){
    //Cambiar quentum y cola de proceso
    int cola_actual = p->cola;
    p->cola = cola_actual+1;  
    
    linkedlist_delete(&(colas[cola_actual]), 0);
    linkedlist_append(&(colas[cola_actual+1]), p);

    p->exec_time = colas[cola_actual+1].quantum;
}

void getAllButFirstAndLast(const char *input, char *output) {
  int len = strlen(input);
  if(len > 0)
    strcpy(output, ++input);
  if(len > 1)
    output[len - 2] = '\0';
}

// Decrementa los contadores de p y revisa si agota un burst o exec_time
void decrement_counters(Process* p, char** status){
    for (int i=0; i < p->burst_count; i++) {
        if (p->bursts[i] != 0) {
            p->bursts[i]--;
            if (p->bursts[i] == 0) {
                *status = SAME_QUEUE;
                if (i == p->burst_count - 1) {
                    *status = FIN_SIGNAL;
                }
            }
            break;
        }
    }
    p->exec_time--;
    if (p->exec_time == 0) {
            *status = DECR_QUEUE;
        return;
    }
}
