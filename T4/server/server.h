#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

typedef struct{
	unsigned char message_type_id;
	unsigned char size;
	unsigned char payload[10];
}mensaje;

void error(const char *msg);

int enviar_mensaje(int socket, int id, int size);

int leer_mensaje(mensaje *msg, int id_socket);

void esperar_mensaje(mensaje *msg, int socket);

int compare_print(mensaje *msg, unsigned char id, char *print);

int compare(mensaje msg, unsigned char id);
