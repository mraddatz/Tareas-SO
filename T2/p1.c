#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <signal.h>
// #include <sys/types.h>
#include <sys/wait.h>

char **arguments_array(char* string);
void  execute(char **argv);
void  parse(char *line, char **argv);
int primero_comilla(char *str);
int process_count(char *file);

typedef struct process
{
    char** argv;   
    int id;
    int status;
    int intentos;
    int exit_code;
} Process;

Process* process_creator(char** av);
void process_finish(Process*p);
Process** crear_cola_procesos(int amount, char *filename);

//File and number of simultaneus process
//argc cantidad de argumentos, argv 
int main(int argc, char *argv[]){
	FILE* file_pointer;
	file_pointer = fopen(argv[1], "r");
	int process_amount = process_count(argv[1]);
	int finished_process =0;
	printf("Cantidad de procesos: %i\n", process_amount);
	int n = atoi(argv[2]);
	int actual_procces = 0;
	//Process**cola_procesos = crear_cola_procesos(process_amount, argv[1]);

	Process**procesos = (Process**) calloc((process_amount) , sizeof(Process*));

	//printf("proceso 0: %s\n", cola_procesos[0]->argv[0]);
		//&variable: retorna puntero a esa variable
		//Linea se guarda en buffer
		char* buffer = NULL;
		size_t bufsize = 0;
		ssize_t length;
		length = getline(&buffer, &bufsize, file_pointer);
		int counter=0;

		while (length > -1){
		if (strchr (buffer, '\n') != NULL ){buffer[strlen(buffer) - 1] = 0;}
		char**arg=(char**) calloc((64) , sizeof(char*));
		printf("Antes de parse:\n");
		int e;
		for (e=0; e<5; e++){
			printf("%s\n", arg[e]);
		}	
		parse(buffer, arg);
		printf("DEspues de parse:\n");
		char* string;
		for (e=0; e<5; e++){
			strcpy(string, arg[e]);
			printf("%s\n", string);

		}

		//char**arg_copiado;
		//strcpy(arg_copiado,arg);
		procesos[counter]=process_creator(arg);

		printf("Printing arvn de todos los procesos con counter: %d\n", counter);
		int j;
		int i;
		for (j=0; j<counter+1; j++){
			//printf("Argvn en direccion %d\n", *cola_procesos[j]->argv);
		for (i=0; i<5; i++){

			printf("arg proceso (%d) argv (%d): %s\n", j, i, procesos[j]->argv[i]);
		}			
		}

		counter++;
		length = getline(&buffer, &bufsize, file_pointer);

		}

		int i;
		int j;

		//for (i=0; i<process_amount; i++){
		//	for (j=0; j<5; j++){
		//		printf("%s\n", "unoo");
		//		printf("%s\n",procesos[i]->argv[j]);
		//	}
		//}


		//length es -1 cuando no quedan lineas
		while (finished_process < process_amount){
			//if (strchr (buffer, '\n') != NULL ){buffer[strlen(buffer) - 1] = 0;}
			//if (actual_procces==n){
			//	printf("Esta Esperando\n" );
			//	sleep(10);
			//	wait(NULL);
			//	actual_procces--;
			//	printf("Dejo de esperar\n");
			//}
			//else{
			//	actual_procces++;
			
		 	//char *argvn[64]; //Argumentos, linea en Buffer
		 	//Parse separa por espacios y lo deja en argv


		 	//parse(buffer, argvn);
			//Process** proc= cola_procesos[finished_process];
			//printf("Argumento del proceso%s\n", proc->argv[0]);
			//printf("process argv: %c\n", *proc->argv[0]);

		 	//proc1.argv=&argvn;
		 	//printf("Estado Inicial: %d\n", proc->status);
		 	//execute(proc->argv);
		 	//wait(&proc->status);
		 	//printf("status: %i\n", proc->status);
		 	//printf("Estado Final: %d\n", proc->status);
		 	//wait(NULL);
		 	//}


			finished_process++;
		 //Lee una nueva linea y vuelve al while
		//length = getline(&buffer, &bufsize, file_pointer);
		}
		
		
		printf("Termino\n");
}


char** arguments_array(char* string){

	printf("%s\n", "ENTROOO");

	//char**arguments=(char* *) malloc((2) * sizeof(char*));
	char**arguments=(char* *) malloc((2)*sizeof(char*));
	//arguments = parse(string);	
	//char* filenme = 'ls'
	//char* argument = '-a'
	//arguments[0] = filename
	//arguments[1] = argument

	return arguments;



}

void  parse(char *line, char **argv)
{	
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
               }   /* replace white spaces with 0    */


          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n'){
               //line++;
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
     //*argv = '\0';                 /* mark the end of argument list  */
}


void  execute(char **argv)
{
     pid_t  pid;
     int    status;

     if ((pid = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          exit(1);
     }
     else if (pid == 0) {         /* for the child process:         */
     printf("---------Inicio Proceso Hijo-----------\n" );
     printf("comando: %s\n", *argv);
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          //while (wait(&status) != pid)       /* wait for completion  */
     	printf("Soy unico padre\n" );;
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

int tiene_comillas(char *str){
	if (strchr (str, '\"') != NULL ){return 1;}
	else{return 0;}
	
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

Process** crear_cola_procesos(int amount, char *filename)
{
	Process**cola_procesos = (Process**) calloc((amount) , sizeof(Process*));
	char* cola_argvs[amount][64];
	FILE* file_pointer;
	file_pointer = fopen(filename, "r");
	char* buffer = NULL;
	size_t bufsize = 0;
	ssize_t length;
	int counter=0;
	length = getline(&buffer, &bufsize, file_pointer);
	while (length > 0){
		if (strchr (buffer, '\n') != NULL ){buffer[strlen(buffer) - 1] = 0;}
		char**argvn=(char**) calloc((64) , sizeof(char*));
		printf("Creando proceso %d\n", counter);
		int i;
		parse(buffer, argvn);

		printf("Printing arvn del buffer:\n");
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
		length = getline(&buffer, &bufsize, file_pointer);
		

	}
	//free(buffer);
	//fclose(file_pointer);
		//int o;
		//for (o=counter; o>0; o--){
	//		printf("%d\n", cola_procesos[o]->id);
	//	}
	printf("Wenoo: %d\n", cola_procesos[1]->id);
	int e;
	//or (e=amount; e>0; e--){
	//	printf("string: %d\n", cola_procesos[e]->id);
	//	printf("argv%s\n", cola_procesos[e]->argv[0]);
	//}
	return cola_procesos;
}



