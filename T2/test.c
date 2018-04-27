#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
	int counter=0;

	while (counter<3){
	char**argvn=(char**) calloc((64) , sizeof(char*));
	printf("position: %i\n", argvn);
	if (counter%2==0){
		argvn[0]="posicion par";

	}
	else{
		argvn[0]="impar";
	}
	printf("%s\n", argvn[0]);
	counter++;
}
//char**argvn;
printf("%s\n", argvn[0]);
//int a;
//for (a=counter; a>0; a--){
//	print(argvn[a]);
//}
}