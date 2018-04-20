
#include "include/arraylist.h"
#include "include/linkedlist.h"
#include "include/mlfq.h"

static volatile int keepRunning = 1;
void intHandler(int dummy) {
    keepRunning = 0;
}

int main(int argc, char *argv[]){
    char* version = argv[1];
    char* buffer  = get_buffer(argv[2]);
    int quantum   = atoi(argv[3]);
    int queues    = atoi(argv[4]);
    int terminados = 0;

    int queue_signal;
    MLFQ* mlfq         = mlfq_init(buffer, quantum, queues);
    signal(SIGINT, intHandler);
    if (strcmp(version, "v1") == 0){
      printf("Ejecutando version1\n");
      while (keepRunning){
        check_entry_times(mlfq);
        if (mlfq->executing_proc == NULL){
            mlfq->timer++;
            //count_waitings(mlfq);
        }
        else{

        decrement_counters(mlfq, &queue_signal);
        count_waitings(mlfq);
        switch (queue_signal) {
            case FIN_SIGNAL :

                finish_process(mlfq);
                terminados++;
                if (mlfq->finished_procs->count == mlfq->procs->count) {
                    printf("COMPLETED\n\n\n");
                    print_final_stats(mlfq);
                    return 0;
                }
                queue_signal = 0;
                break;
            case DOWN_QUEUE :
                mlfq->executing_proc->bloqueos++;
                update_queue(mlfq, true);
                queue_signal = 0;
                break;
            case SAME_QUEUE :
                update_queue(mlfq, false); 
                queue_signal = 0; 
                break;    
        }
        //sleep(1);
        mlfq->timer++;
        if (mlfq->timer == 10000) break;
    }
      }
      printf("Salio del while\n" );
      print_final_stats(mlfq);
    }



     else if (strcmp(version, "v2") == 0){
        if (!argv[5]){
            printf("Falta argumento para el S\n");
            return 1;
        }
       int s = atoi(argv[5]);
       int time_to_reset = s;
      printf("Ejecutando version 2\n");


      while (true){
        check_entry_times(mlfq);
        if (mlfq->executing_proc == NULL){
            mlfq->timer++;
            //count_waitings(mlfq);
        }
        else {
        decrement_counters(mlfq, &queue_signal);
        count_waitings(mlfq);
        switch (queue_signal) {
            case FIN_SIGNAL :

                finish_process(mlfq);
                terminados++;
                if (mlfq->finished_procs->count == mlfq->procs->count) {
                    printf("COMPLETED\n\n\n");
                    print_final_stats(mlfq);
                    return 0;
                }
                queue_signal = 0;
                break;
            case DOWN_QUEUE :
                mlfq->executing_proc->bloqueos++;
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
        if (time_to_reset<=0){
            procesos_a_primera_cola(mlfq);
            time_to_reset=s;
        }
        time_to_reset--;

        if (mlfq->timer == 10000) break;
        }
      }
      print_final_stats(mlfq);
     }
    else if (strcmp(version, "v3") == 0){
        if (!argv[5]){
            printf("Falta argumento para el S\n");
            return 1;
        }
       int s = atoi(argv[5]);
       int time_to_reset = s;
      printf("Ejecutando version 3\n");

      ajustar_quantum_v3(mlfq);
      while (true){
        check_entry_times(mlfq);
        if (mlfq->executing_proc == NULL){
            mlfq->timer++;
        }
        else {
        decrement_counters(mlfq, &queue_signal);
        count_waitings(mlfq);
        switch (queue_signal) {
            case FIN_SIGNAL :

                finish_process(mlfq);
                terminados++;
                if (mlfq->finished_procs->count == mlfq->procs->count) {
                    printf("COMPLETED\n\n\n");
                    print_final_stats(mlfq);
                    return 0;
                }
                queue_signal = 0;
                break;
            case DOWN_QUEUE :
                mlfq->executing_proc->bloqueos++;
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
        if (time_to_reset<=0){
            procesos_a_primera_cola(mlfq);
            time_to_reset=s;
        }
        time_to_reset--;

        if (mlfq->timer == 10000) break;
        }
      }
      print_final_stats(mlfq);
     }

    

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
            proceso->entry_time = T-1;
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

    proceso->cpu_turns=0;
    proceso->bloqueos=0;
    proceso->response_time=-1;
    proceso->estado=0;

    return proceso;
};

//Process* seleccionar_proceso(){

//}
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
    //char* ch;
    //strcpy(ch, 'READY');
    if (p->estado==2){
        printf("Proceso %s paso de RUNNING a READY\n", p->nombre);
    }
    p->cola = num;
    p->estado=1;
    //strcpy(p->estado, ch);
    linkedlist_append(&(mlfq->queues[num]), p);
    if (set_time == true) p->exec_time = mlfq->queues[num].quantum;
    seleccionar_proceso(mlfq); // En caso de p tenga mejor prioridad que exec_proc
}

//Selecciona proceso a ejecutar
void seleccionar_proceso(MLFQ* mlfq){
    for (int i = 0; i < mlfq->num_queues; i++){
        if ((mlfq->queues[i]).size > 0){
            mlfq->executing_proc = linkedlist_get(&(mlfq->queues[i]), 0);
            if (mlfq->executing_proc->response_time==-1){
                mlfq->executing_proc->response_time=mlfq->timer+2;
            }
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
            printf("Entro proceso %s en tiempo %d\n", p->nombre, mlfq->timer+1);
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


// Decrementa los contadores de p y revisa si agota un burst o exec_time
void decrement_counters(MLFQ* mlfq, int* status){
    Process* p = mlfq->executing_proc;
    //Si antes su estado era ready imprimimos que paso de ready a Running
    if (p->estado == 1){
        printf("Scheduler eligio proceso %s para ejecutar\n", p->nombre);
        printf("Proceso %s paso de READY a RUNNING\n", p->nombre);
    }
    p->estado=2;
    p->exec_time--;
    p->cpu_turns++;
    int aux;
    //Esto es como un nuevo tick
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

    //printf("\n");
    //printf("PID: %d\nExec time: %d\nBurst: %d\nBurst count: %d\nCodigo: %d\n\n",
    //p->PID, p->exec_time, p->bursts[aux], p->burst_count, *status);
    return;
    
}

// Baja o reingresa de queue al proceso que se esta ejecutando
void update_queue(MLFQ* mlfq, bool downgrade) {

    int index = mlfq->executing_proc->cola;
    // Sacar al exec_proc de la cola en que esta
    Process* p = linkedlist_delete(&(mlfq->queues[index]), 0);
    printf("Proceso %s paso de RUNNING a READY\n", p->nombre);
    p->estado=1;
    if (downgrade) {
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
    int index = mlfq->executing_proc->cola;
    Process* p = linkedlist_delete(&(mlfq->queues[index]), 0);
    p->finish_time=mlfq->timer;
    p->estado=3;
    printf("El proceso %s ha terminado (FINISHED)\n", p->nombre);
    arraylist_append(mlfq->finished_procs, p);
    seleccionar_proceso(mlfq);
}

//Imprime datos finales solicitados
void print_final_stats(MLFQ* mlfq){
    printf("Procesos terminados: %i\n", mlfq->finished_procs->count);
    printf("Tiempo Total: %i\n", mlfq->timer+1);
    printf("\n");
    int p;

    for (p=0; p<mlfq->procs->count;p++){
        //printf("p\n" );
        Process* proceso;
        proceso = arraylist_get(mlfq->procs, p);
        printf("%s:\n", proceso->nombre);
        printf("Turnos de CPU: %d\n", proceso->cpu_turns);
        printf("Bloqueos: %d\n", proceso->bloqueos);
        printf("Turnaround time: %d\n", proceso->finish_time-proceso->entry_time);
        printf("Response time: %d\n", proceso->response_time-proceso->entry_time);
        printf("Waiting Time: %d\n", proceso->waiting_time);
        printf("\n");
    }


}

//Cuenta la cantidad de procesos en estado waiting
void count_waitings(MLFQ* mlfq){
    for (int p=0; p < mlfq->procs->count;p++){
        //printf("%i\n",p );
        Process* proceso = arraylist_get(mlfq->procs, p);
        //printf("EStado proceso %s es: %i\n", proceso->nombre, proceso->estado );
        if (proceso->estado == 1){
            proceso->waiting_time++;
        }
        if (!(proceso->estado==3)){
            proceso->finish_time++;
            //proceso->response_time++;
        }
}
}

//Traspasar todos los procesos a la primera prioridad
void procesos_a_primera_cola(MLFQ* mlfq){
    int i;
    for (i=1; i < mlfq->num_queues; i++){
        while (mlfq->queues[i].size > 0 ){
            Process* p = linkedlist_delete(&(mlfq->queues[i]), 0);
            p->cola = 0;
            p->exec_time = mlfq->queues[0].quantum;
            linkedlist_append(&(mlfq->queues[0]), p);

        }
    }

}

void ajustar_quantum_v3(MLFQ* mlfq){
    int i;
    int q = mlfq->queues[0].quantum;
    int queue_number = mlfq->num_queues;
    int new_quantum;
    int prioridad;
    for (i=1; i<queue_number; i++){
        prioridad = (queue_number - 1 - i);
        new_quantum = (queue_number-prioridad)*q;
        mlfq->queues[i].quantum = new_quantum;

    }
}



