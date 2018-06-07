#ifndef POKER_H
#define POKER_H

#include <stdlib.h>
#include <stdbool.h>
#include <locale.h>
#include <wchar.h>

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

// Socket placeholder
typedef struct {
} Socket;

// Jugador
typedef struct {
    int id;
    int pot;
    char* nombre;
    int cartas[5][2];
    Socket socket;
} Jugador;

typedef struct {
    Jugador* jugadores[2];
    int num_jugadores;
    int dealer;
    int turno;
    int pozo;
} Partida;

// INITS

Partida* partida_init();

Jugador* jugador_init(Partida* p, char* nombre);

// Cobra la apuesta minima
void apuesta_minima(Jugador* jugadores);

// Reparte las 5 cartas iniciales
void repartir_cartas(Jugador* jugadores);

// Cambio de cartas de cada ronda. Se ejecuta al recibir RET_CARDS_CHNG
void cambiar_cartas(int cartas[][2], int n, Jugador* j);

// Decide quien parte esta vuelta y le envia el msje
void who_first(Partida* p);

// Despues de mandar GET_BET y de recibir RET_BET
bool manage_bet(Partida* p, int bet_code);

int evaluar_mano(int cartas[5][2]);

int count_pintas(int cartas[5][2], int pinta);

int count_nums(int cartas[5][2], int num);

void print_cartas(int cartas[][2], int n);

// Libera el heap
void free_memory(Partida* p);

#endif