
#include "include/mlfq.h"
#include "include/arraylist.h"
#include "include/linkedlist.h"

int main(int argc, char *argv[]){
    char* buffer = get_buffer(argv[1]);
    ArrayList* lista = (ArrayList*)get_procesos(buffer);
    int tick = 0;
    Process* p;

    Process* p1 = arraylist_get(lista, 0);
    Process* p2 = arraylist_get(lista, 1);

    // printf("%s con bursts %d %d %d y PID %d\n", p1.nombre, p1.bursts[0], 
    // p1.bursts[1], p1.bursts[2], p1.PID);
    // printf("%s con bursts %d %d %d %d y PID %d\n", p2.nombre, p2.bursts[1], 
    // p2.bursts[1], p2.bursts[2], p2.bursts[3], p2.PID);
    while (true){
        printf("tick...\n");
        for (int i=0; i < lista->count; i++){
            p = arraylist_get(lista, i);
            if (p->entry_time == tick){
                printf("%s entra a la cola en T=%d (PID=%d)\n", p->nombre, tick, p->PID);
            }
        }
        sleep(1);
        tick++;
        if(tick == 10) break;
    }

    arraylist_destroy(lista);
    return 0;
}

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
