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

<<<<<<< HEAD:T4/server/server.h

//Definicion cartas y mensajes
#define A               1u
#define J               11u
#define Q               12u
#define K               13u

#define HEART           1u
#define DMOND           2u
#define CLOVR           3u
#define SPADE           4u

#define HEART_REPR      0x2665
#define DMOND_REPR      0x2666
#define CLOVR_REPR      0x2663
#define SPADE_REPR      0x2660

#define START_CONN      1
#define CONN_ESTAB      2
#define ASK_NICK        3
#define RET_NICK        4
#define OPNT_FOUND      5
#define INIT_POT        6
#define GAME_START      7
#define START_RND       8
#define INIT_BET        9
#define FIVE_CARDS      10
#define WHO_FIRST       11
#define GET_CARDS_CHNG  12
#define RET_CARDS_CHNG  13
#define GET_BET         14
#define RET_BET         15
#define ERR_BET         16
#define OK_BET          17
#define END_ROUND       18
#define SHOW_OPNT_CARDS 19
#define WIN_LOSE        20
#define UPDT_POT        21
#define GAME_END        22
#define IMAGE           23
#define ERROR           24

#define FOLD            1
#define BET_0           2
#define BET_100         3
#define BET_200         4
#define BET_500         5

#define PAR             1
#define DOS_PARES       2
#define TRIO            3
#define ESCALA          4
#define COLOR           5
#define POKER           6
=======
void getBin(int num, char *str);
>>>>>>> 391fd2858c7bff61f8d0495f8cf4c83fceab8b47:T4/server/include/server.h
