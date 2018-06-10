#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <locale.h>
#include <wchar.h>
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

//Definicion cartas y mensajes
#define A               1
#define J               11
#define Q               12
#define K               13

#define HEART           1
#define DMOND           2
#define CLOVR           3
#define SPADE           4

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

int connection_established(char *nickname, char *nickname_oponente, mensaje *msg, int socket);
