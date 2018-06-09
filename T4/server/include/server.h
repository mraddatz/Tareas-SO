#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#define NULL_PAYLOAD ""

typedef struct{
	unsigned char message_type_id;
	unsigned char size;
	unsigned char payload[254];
}mensaje;

void error(const char *msg);

int enviar_mensaje(int socket, unsigned char id, unsigned char size, char* payload);

int leer_mensaje(mensaje *msg, int id_socket);

void esperar_mensaje(mensaje *msg, int socket);

int compare_print(mensaje *msg, unsigned char id, char *print);

int compare(mensaje msg, unsigned char id);
