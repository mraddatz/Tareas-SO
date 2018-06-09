#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

#define NULL_MSG        0U
#define START_CONN      1U
#define CONN_ESTAB      2U
#define ASK_NICK        3U
#define RET_NICK        4U
#define OPNT_FOUND      5U
#define INIT_POT        6U
#define GAME_START      7U
#define START_RND       8U
#define INIT_BET        9U
#define FIVE_CARDS      10U
#define WHO_FIRST       11U
#define GET_CARDS_CHNG  12U
#define RET_CARDS_CHNG  13U
#define GET_BET         14U
#define RET_BET         15U
#define ERR_BET         16U
#define OK_BET          17U
#define END_ROUND       18U
#define SHOW_OPNT_CARDS 19U
#define WIN_LOSE        20U
#define UPDT_POT        21U
#define GAME_END        22U
#define IMAGE           23U
#define ERROR           24U

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
