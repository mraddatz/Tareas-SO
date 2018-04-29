#include <time.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//#include <signal.h>
// #include <sys/types.h>
#include <sys/wait.h>
void  parse(char *line, char **argv);
int primero_comilla(char *str);
int process_count(char *file);

typedef struct process
{
    char** argv;   
    int id;
    int status;
    int intentos;
    int waited;
    int exit_code;
    int link[2];
    char foo[4096];
} Process;

char* execute(Process*proc);
Process* process_creator(char** av);
void process_finish(Process*p);
Process** crear_cola_procesos(int amount, char *filename);
//int terminado(int cantidad_procesos, Process** cola_procesos);
#define die(e) do { fprintf(stderr, "%s\n", e); exit(EXIT_FAILURE); } while (0);

//File and number of simultaneus process
//argc cantidad de argumentos, argv 
int main(int argc, char *argv[]){
	FILE* file_pointer;
	file_pointer = fopen(argv[1], "r");
	int process_amount = process_count(argv[1]);
	int finished_process =0;
	clock_t begin = clock();
	struct timeval tvalBefore, tvalAfter;
	gettimeofday (&tvalBefore, NULL);
	int n = atoi(argv[2]);
	int actual_procces = 0;
	Process**cola_procesos = crear_cola_procesos(process_amount, argv[1]);
	//Process**procesos = (Process**) calloc((process_amount) , sizeof(Process*));
		//&variable: retorna puntero a esa variable
		//Linea se guarda en buffer
		char* buffer = NULL;
		size_t bufsize = 0;
		ssize_t length;
		length = getline(&buffer, &bufsize, file_pointer);
		int counter=0;
		if (strchr (buffer, '\n') != NULL ){buffer[strlen(buffer) - 1] = 0;}
		char**arg=(char**) calloc((64) , sizeof(char*));
		parse(buffer, arg);
		int mem_proceso =0;
		Process* proc =  cola_procesos[mem_proceso];
		int status;
		int i;
		int procesos_actuales = 0;
		int iteracion = 1;

		//length es -1 cuando no quedan lineas
		while (finished_process < process_amount){
		Process* proc =  cola_procesos[mem_proceso];
			if (procesos_actuales==n){
				int proceso_wait = wait(&status);
				for (i=0; i<process_amount; i++){
					Process* p=cola_procesos[i];
					if (p->id==proceso_wait){
						p->waited = 1;
						p->status = WEXITSTATUS(status);
						//printf("Actualizando status proceso: %d a %d\n", p->id, p->status);
					}
				}
				procesos_actuales--;
			}

			procesos_actuales++;
			proc->intentos++;
		 	execute(proc);
		 	int status;
		 	close(proc->link[1]);
			int nbytes = read(proc->link[0], proc->foo, sizeof(proc->foo));
			mem_proceso++;
			finished_process++;
		}//termino del while
		//Esperar al resto de los procesos
		for (i=0; i<process_amount; i++){
			Process* proc3=cola_procesos[i];
			//printf("Process id: %d\n", proc3->id);
			if (proc3->waited == 0){
				waitpid(proc3->id,&status,0);
				proc3->status=WEXITSTATUS(status);
			}
		}
		int resp;

		printf("Termino:\n");
		clock_t end = clock();
		double time_spent =(double)(end - begin) / CLOCKS_PER_SEC;
		gettimeofday (&tvalAfter, NULL);
		int e;
		float time_microseconds = ((tvalAfter.tv_sec - tvalBefore.tv_sec)*1000000L
           +tvalAfter.tv_usec) - tvalBefore.tv_usec;
		for(e=0; e<process_amount; e++){
			Process* proc2 = cola_procesos[e];
			printf("----PROCESO %i----\n", e+1);
			printf("Output: %s\n", proc2->foo);

			printf("ExitCode: %d\n", proc2->status);
		}
		printf("----Estadisticas Generales-----:\n");
		printf("Tiempo total sistema (Tiempo secuencial): %f\n", time_spent);
		printf("Time in microseconds (Tiempo paralelo): %f microseconds\n",
            time_microseconds/1000000
          ); // Added semicolon
		printf("M: %i\n", process_amount);
		printf("N: %i\n", n);

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


char*  execute(Process*proc)
{
     pid_t  pid;
     int    status;
	if (pipe(proc->link)==-1)
		die("pipe");
     if ((proc->id = fork()) < 0) {     /* fork a child process           */
          printf("*** ERROR: forking child process failed\n");
          die("fork")
          exit(1);
     }
     else if (proc->id == 0) {         /* for the child process:         */
     dup2 (proc->link[1], STDOUT_FILENO);
		 close(proc->link[0]);
		 close(proc->link[1]);
          if (execvp(*proc->argv, proc->argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               die("execvp");

               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          //while (wait(&status) != pid)       /* wait for completion  */
     	//printf("Soy unico padre\n" );;
     }

     char* retornar;
     retornar = "hola";

     //printf("Fin de execute:\n");
     return retornar;
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
	proc->waited=0;
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
		//printf("%s\n", buffer);
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
	char*lineas=(char*) calloc((100) , sizeof(char[100]));
	length = getline(&buffer, &bufsize, file_pointer);
	int c;
	c = 0;

	while (length > 0){
		if (strchr (buffer, '\n') != NULL ){buffer[strlen(buffer) - 1] = 0;}
		strcpy(&lineas[c] , buffer);
		char**argvn=(char**) calloc((64) , sizeof(char*));
		parse(&lineas[c], argvn);
		Process* proc= process_creator(argvn);

		//proc->id=counter;
		cola_procesos[counter] = proc;
		c=c+sizeof(char[100]);
		length = getline(&buffer, &bufsize, file_pointer);
		counter=counter+1;

	}
	return cola_procesos;
}


