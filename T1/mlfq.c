
#include "include/arraylist.h"
#include "include/linkedlist.h"
#include "include/mlfq.h"


int main(int argc, char *argv[]){
    char* version = argv[1];
    char* buffer  = get_buffer(argv[2]);
    int quantum   = atoi(argv[3]);
    int queues    = atoi(argv[4]);
    int terminados = 0;

    int queue_signal;
    MLFQ* mlfq         = mlfq_init(buffer, quantum, queues);

    if (strcmp(version, "v1") == 0){
      printf("Ejecutando version1\n");
      while (true){
        printf("tick %d...\n", mlfq->timer);
        printf("Finished: %d\n", mlfq->finished_procs->count);
        check_entry_times(mlfq);
        if (mlfq->executing_proc == NULL) {
            printf("No encontro procesos..");
            break;
        }

        decrement_counters(mlfq, &queue_signal);
        switch (queue_signal) {
            case FIN_SIGNAL :
                finish_process(mlfq);
                terminados++;
                if (mlfq->finished_procs->count == mlfq->procs->count) {
                    printf("COMPLETED\n\n\n");
                    return 0;
                }
                queue_signal = 0;
                break;
            case DOWN_QUEUE :
                update_queue(mlfq, true);
                queue_signal = 0;
                break;
            case SAME_QUEUE :
                update_queue(mlfq, false); 
                queue_signal = 0; 
                break;    
        }
        // sleep(1);
        mlfq->timer++;
        if (mlfq->timer == 10000) break;
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

    // Process* proc;
    // proc = arraylist_get(mlfq->procs, 4);
    // entra_proceso(proc, mlfq->queues, 0, false);
    // arraylist_destroy(mlfq->procs);
    // linkedlist_append(&(mlfq->queues[1]), proc);
    // baja_prioridad(proc,mlfq->queues);
    // printf("Id Proceso %i\n",proc->PID);

    return 1;
}

// Inicializa y retorna la estructura mlfq a partir de los parametros
MLFQ* mlfq_init(char* buffer, int quantum, int queues) {
    MLFQ* mlfq = malloc(sizeof(MLFQ));
    mlfq->procs = get_procesos(buffer);
    mlfq->finished_procs = arraylist_init();
    mlfq->queues = malloc(queues * sizeof(LinkedList));

    for (int i = 0; i < queues; i++){
        mlfq->queues[i] = *(linkedlist_init(quantum));
    };
    
    mlfq->timer = 0;
    mlfq->num_queues = queues;
    seleccionar_proceso(mlfq);

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

// Mete proceso a la queue num
void entra_proceso(Process* p, MLFQ* mlfq, int num, bool set_time){
    //Cambiar quentum y cola de proceso
    p->cola = num;
    linkedlist_append(&(mlfq->queues[num]), p);
    if (set_time == true) p->exec_time = mlfq->queues[num].quantum;
    seleccionar_proceso(mlfq); // En caso de p tenga mejor prioridad que exec_proc
}

//Selecciona proceso a ejecutar
void seleccionar_proceso(MLFQ* mlfq){
    for (int i = 0; i < mlfq->num_queues; i++){
        if ((mlfq->queues[i]).size > 0){
            mlfq->executing_proc = linkedlist_get(&(mlfq->queues[i]), 0);
            return;
        }
    }
    // No encontro ninguno
    mlfq->executing_proc = NULL;
}

// Mete a la queue0 a los procesos que les toque entrar
void check_entry_times(MLFQ* mlfq) {
    Process* p;

    for (int i=0; i < mlfq->procs->count; i++){
        p = arraylist_get(mlfq->procs, i);
        if (p->entry_time == mlfq->timer){
            entra_proceso(p, mlfq, 0, true);
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
void decrement_counters(MLFQ* mlfq, int* status){
    Process* p = mlfq->executing_proc;
    p->exec_time--;
    int aux;
    for (int i = 0; i < p->burst_count; i++) {
        aux = i;
        if (p->bursts[i] != 0) {
            p->bursts[i]--;
            if (p->bursts[i] == 0) {
                *status = SAME_QUEUE;
                if (i == p->burst_count - 1) {
                    *status = FIN_SIGNAL;
                    return;
                }
            }
            break;
        }
    }

    if (p->exec_time == 0) {
        *status = DOWN_QUEUE;
    }

    printf("\n");
    printf("PID: %d\nExec time: %d\nBurst: %d\nBurst count: %d\nCodigo: %d\n\n",
    p->PID, p->exec_time, p->bursts[aux], p->burst_count, *status);
    return;
    
}

// Baja o reingresa de queue al proceso que se esta ejecutando
void update_queue(MLFQ* mlfq, bool downgrade) {
    int index = mlfq->executing_proc->cola;
    // Sacar al exec_proc de la cola en que esta
    Process* p = linkedlist_delete(&(mlfq->queues[index]), 0);
    if (downgrade == true) {
        if (index != mlfq->num_queues - 1) {
            // Caso en que no esta en la ultima cola, baja.
            // Si no, hace RR
            index++;
        }
    }
    entra_proceso(p, mlfq, index, downgrade);
}

// Saca a exec_proc de las colas y lo mete a la lista finished
void finish_process(MLFQ* mlfq) {
    printf("Termino uno\n\n");
    int index = mlfq->executing_proc->cola;
    Process* p = linkedlist_delete(&(mlfq->queues[index]), 0);
    arraylist_append(mlfq->finished_procs, p);
    printf("Queue: %d con %d procs\nFinished: %d\n", p->cola, mlfq->queues[index].size, mlfq->finished_procs->count);
    seleccionar_proceso(mlfq);
}
