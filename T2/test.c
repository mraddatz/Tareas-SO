#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct process
{
    char** argv;   
    int id;
    int status;
    int intentos;
    int exit_code;
    int link[2];
    char foo[4096];
} Process;

Process* process_creator(char** av);
void  parse(char *line, char **argv);
int primero_comilla(char *str);
int process_count(char *file);

int main(int argc, char *argv[])
{	
	FILE* file_pointer;
	file_pointer = fopen(argv[1], "r");
	int amount = process_count(argv[1]);
	Process**cola_procesos = (Process**) calloc((amount) , sizeof(Process*));
	char* buffer = NULL;
	size_t bufsize = 0;
	ssize_t length;
	int counter=0;
	length = getline(&buffer, &bufsize, file_pointer);

	char*lineas=(char*) calloc((100) , sizeof(char[100]));


	char* line = (char*) calloc((bufsize) , sizeof(char));

	int c;
	c = 0;

	strcpy(line, buffer);

	while (length > 0){
		if (strchr (buffer, '\n') != NULL ){buffer[strlen(buffer) - 1] = 0;}
		strcpy(&lineas[c] , buffer);
		//printf("Lineaa: %s\n", lineas[c]);


		char**argvn=(char**) calloc((64) , sizeof(char*));
		int i;
		parse(&lineas[c], argvn);

		for (i=0; i<5; i++){
			printf("arg: %s\n", argvn[i]);
		}
		Process* proc= process_creator(argvn);
		printf("Printing arvn del proceso:\n");

		for (i=0; i<5; i++){
			printf("arg: %s\n", proc->argv[i]);
			//printf("direccion argvn %i\n", proc->argv);
		}
		proc->id=counter;
		cola_procesos[counter] = proc;
		printf("Printing arvn de todos los procesos con counter: %d\n", counter);
		int j;
		for (j=0; j<counter+1; j++){
			//printf("Argvn en direccion %d\n", *cola_procesos[j]->argv);
		for (i=0; i<5; i++){

			printf("arg proceso (%d) argv (%d): %s\n", j, i, cola_procesos[j]->argv[i]);
		}			
		}

		counter++;
		c=c+sizeof(char[100]);
		length = getline(&buffer, &bufsize, file_pointer);

		char* line = (char*) calloc((bufsize) , sizeof(char));
		//for (c=0; c<bufsize; c++){
		//printf("Lineaaa: %s\n", line);
		
		printf("Direcciones: \n");
		printf("Direccion linea: %d\n", &line);
		printf("Direccion Buffer: %d\n", &buffer);
		for (i=0; i<20;i++){
			printf("String Linea Arreglo: %c\n", lineas[i]);

		}
		//printf("String Linea Arreglo: %c\n", lineas[c]);
		printf("Lineaaa: %s\n", line);
		//strcpy(&lineas[c], buffer);
		strcpy(line, buffer);


	}

}

Process* process_creator(char**argv){
	Process *proc=(Process*) calloc(1,sizeof(Process));
	proc->argv=argv;
	proc->status=1;
	proc->intentos=0;
	proc->id=0;
	proc->exit_code=0;
  return proc;
}

void  parse(char *line, char **argv)
{	
	char**argumentos=(char**) calloc((48) , sizeof(char*));
	int i;
	for (i=0; i<48; i++){
		argumentos[i]=NULL;
	}
	int contador_argumentos=0;

	int comillas = 0;
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n'){          			//*line++ = '\0';
          			if (comillas%2==0){
          				*line++ = '\0';
          			}
          			else{
          				line++;
          			}
          		//}

          }
          if (comillas%2==0 && primero_comilla(line)){
          	line++;
          *argv++ = line;
          comillas++;          /* save the argument position     */
          } else if (comillas%2==0){
               	*argv++ = line;
               	printf("Holaaa\n");
               	//printf("%d - %c\n", &line, *line);
               	//argumentos[contador_argumentos]=

               }   /* replace white spaces with 0    */


          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n'){
           		char elemento = line[0];
				char comilla = '\"';
				if (elemento==comilla){
					*line++ = '\0';
					comillas++;
				}
				else{
					line++;
				}
          }
     }
}

int primero_comilla(char *str){
	char primer_elemento;
	primer_elemento = *str;
	char comilla = '\"';
	if (primer_elemento== comilla){
		return 1;
	}
	else{
		return 0;
	}
	
}

int process_count(char *file)
{
	FILE* file_pointer;
	file_pointer = fopen(file, "r");
	char* buffer = NULL;
	size_t bufsize = 0;
	ssize_t length;
	int counter=0;
	length = getline(&buffer, &bufsize, file_pointer);
	while (length > 0){
		printf("%s\n", buffer);
		counter++;
		length = getline(&buffer, &bufsize, file_pointer);

	}
	fclose(file_pointer);
	return counter;
}
