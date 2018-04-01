#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>

struct Process {
    int PID;
    char nombre[256];
    char estado[20];
    int bursts[];
};

typedef struct Process Proc;

void crear_proceso(char string[], int PID);

char* file2char(char filename[]);

int str2int(char ch);

int main(int argc, char **argv){
    char    *file_content;
    file_content = file2char(argv[1]);
    //file_content contiene un char con todo el contenido
    int i;
    //Proceso para separar por lineas (el mismo sirve para separar palabras, cambiar el "\n" por " ")
    char *ch;
    ch = strtok(file_content, "\n");
    while (ch){
        printf("%s\n", "new line");
        printf("%s\n", ch);
        //Acá, para cada linea haría el proceso
        crear_proceso(ch, i);
        i++;
        ch = strtok(NULL, "\n");
        printf("%s\n", ch);
    }

    return 0;

}

char* file2char(char filename[]){
    printf("Estoy %s\n", "Dentro de la funcion!");
    printf("nombre archivo: %s\n", filename);
    FILE    *infile;
    char    *buffer;
    long    numbytes;

    infile = fopen(filename, "r");
    if(infile == NULL)
    {
        return "hola";
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
    return "hi";

    //Copiamos el texto al buffer
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);


    printf("number of bytes: %ld\n", numbytes);
    printf("The file  contains this text\n\n%s", buffer);



    free(buffer);

    return buffer;
}

void crear_proceso(char string[], int PID){
    fflush(stdin);
    char *ch;
    int i;
    printf("creando proceso\n" );
    Proc proceso;
    printf("Largo proceso: %lu\n", sizeof(proceso));
    proceso.PID = PID;
    ch = strtok(string, " ");
    while (ch){
    	printf("Nuevo string %s\n", ch);
        if (i == 0) strcpy(proceso.nombre, ch);
        else if (i==1) printf("hola\n" );
        else {
            proceso.bursts[i-1] = str2int(*ch);
        }
        ch = strtok(NULL, " ");
        i++;
    }
    printf("hola2\n");
	//printf("%s con bursts %d %d %d %d y PID %d", proceso.nombre, proceso.bursts[0], proceso.bursts[1], proceso.bursts[2], proceso.bursts[3], proceso.PID);
}

int str2int(char ch) {
    int num = ch - '0';
    return num;
}