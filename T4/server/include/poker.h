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

#define INITIAL_POT     1000U

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
    int pot;
    int cartas[5][2];
    char* nombre;
} Jugador;

typedef struct {
    int dealer;
    int pozo;
    Jugador* jugadores[2];
} Partida;

// INITS

Partida* partida_init(Jugador* j1, Jugador* j2);

Jugador* jugador_init(char* nombre);

// Cobra la apuesta minima
void apuesta_minima(Jugador* jugadores);

// Reparte las 5 cartas iniciales
void repartir_cartas(Jugador* j1, Jugador* j2);

// Cambio de cartas de cada ronda. Se ejecuta al recibir RET_CARDS_CHNG
void cambiar_cartas(int cartas[][2], int n, Jugador* j);

// Decide quien parte esta vuelta y le envia el msje
void who_first(Partida* p);

// Despues de mandar GET_BET y de recibir RET_BET
bool manage_bet(Partida* p, Jugador* j, int bet_code, int* status_code);

int evaluar_mano(int cartas[5][2]);

int count_pintas(int cartas[5][2], int pinta);

int count_nums(int cartas[5][2], int num);

void print_cartas(int cartas[][2], int n);

// Libera el heap
void free_memory(Partida* p);

#endif