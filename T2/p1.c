#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

char **arguments_array(char* string);
void  execute(char **argv);
void  parse(char *line, char **argv);
//File and number of simultaneus process
//argc cantidad de argumentos, argv 
int main(int argc, char *argv[]){
	FILE* file_pointer;
	file_pointer = fopen(argv[1], "r");
	int n = atoi(argv[2]);
	int actual_procces = 0;
	//for (n = 3 ; n>0; n--){
		//getline -> stdio.h
		//getline(&buffer,&size,stdin);
		//&variable: retorna puntero a esa variable
		//Linea se guarda en buffer
		char* buffer = NULL;
		size_t bufsize = 0;
		ssize_t length;
		length = getline(&buffer, &bufsize, file_pointer);
		while (length > 0){
		printf("%zi\n", length);	
		printf("%s\n", buffer);
		printf("Procesos acutuales: %i, procesos maximos: %i\n", actual_procces, n);
		if (actual_procces==n){
			printf("%s\n", "esperar alguno");
			//Espera a cualquier hijo que termine, luego continuo
			wait(NULL);
			actual_procces--;
		}
		actual_procces++;
		int child_pid = fork();
		 if (child_pid==0){
		 	printf("%s\n", "Soy Hijo!");

		 	char *argvn[64]; //Argumentos, linea en Buffer
		 	//Parse separa por espacios y lo deja en argv
		 	printf("El string que tengo que leer es %s\n", buffer);

		 	
			buffer[strlen(buffer) - 1] = 0;
		 	printf("El nuevo string que tengo que leer es %s\n", buffer);

		 	parse(buffer, argvn);

		 	       /*   parse the line               */
		 	//execvp recibe parametros al igual que la consola, primer argumento es el file y segundo los argumentos de este
		 	//int  main(int argc, char **argv)
		 	int element;
		 	for (element=0; element<5; element++){
	 			printf("%s\n", argvn[element]);

		 	}




		 	//printf("ejecutando %s %s %s\n", argvn[0], argvn[1], argvn[2]);
		 	execute(argvn);
		 	//	printf("Fallo la ejecucion de %s\n", buffer);
		 	//}
		 	break;
		 }



		  //char**ar=arguments_array(buffer);







		length = getline(&buffer, &bufsize, file_pointer);

		}
		

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
     while (*line != '\0') {       /* if not the end of line ....... */ 
          while (*line == ' ' || *line == '\t' || *line == '\n')
               *line++ = '\0';     /* replace white spaces with 0    */
          *argv++ = line;          /* save the argument position     */
          while (*line != '\0' && *line != ' ' && 
                 *line != '\t' && *line != '\n') 
               line++;             /* skip the argument until ...    */
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
     else if (pid == 0) {          /* for the child process:         */
          if (execvp(*argv, argv) < 0) {     /* execute the command  */
               printf("*** ERROR: exec failed\n");
               exit(1);
          }
     }
     else {                                  /* for the parent:      */
          while (wait(&status) != pid)       /* wait for completion  */
               ;
     }
}

