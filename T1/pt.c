#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <errno.h>


int main(int argc, char **argv){
    printf("nombre archivo: %s\n", argv[1]);
    FILE    *infile;
    char    *buffer;
    long    numbytes;

    infile = fopen(argv[1], "r");
    if(infile == NULL)
    {
        return 1;
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
    return 1;

    //Copiamos el texto al buffer
    fread(buffer, sizeof(char), numbytes, infile);
    fclose(infile);


    printf("number of bytes: %ld\n", numbytes);
    printf("The file  contains this text\n\n%s", buffer);

    //Proceso para separar por lineas (el mismo sirve para separar palabras, cambiar el "\n" por " ")
    char *ch;
    ch = strtok(buffer, "\n");
    while (ch){
        printf("%s\n", "new line");
        printf("%s ", ch);
        //Acá, para cada linea haría el proceso
        ch = strtok(NULL, "\n");
    }


    free(buffer);
    return 0;

}



