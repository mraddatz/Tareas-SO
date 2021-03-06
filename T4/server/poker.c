#include "include/poker.h"

int pintas[4] = {HEART, DMOND, CLOVR, SPADE};
int pintas_repr[4] = {HEART_REPR, DMOND_REPR, CLOVR_REPR, SPADE_REPR};
int cartas[13] = {A,2};

Partida* partida_init(Jugador* j1, Jugador* j2) {
    Partida* p = malloc(sizeof(Partida));
    p->dealer = 0;
    p->pozo = -1;
    p->jugadores[0] = j1;
    p->jugadores[1] = j2;
    return p;
}

Jugador* jugador_init(char* nombre) {
    Jugador* j = malloc(sizeof(Jugador));
    j->pot = INITIAL_POT;
    j->nombre = nombre;
    return j;
}

// Cobra la apuesta minima
void apuesta_minima(Jugador* jugadores) {
    int n = sizeof(jugadores) / sizeof(Jugador);
    for (int i=0; i < n; i++) {
        Jugador j = jugadores[i];
        if (j.pot >= 10) {
            j.pot -= 10;
            // ENVIAR PAQUETE UPDT_POT CON J.POT A J.SOCKET
        }
        else ;// ENVIAR PAQUETE WIN_LOSE CON UN 2 A J.SOCET
    }
}

// Reparte las 5 cartas iniciales
void repartir_cartas(Jugador* j1, Jugador* j2) {
    for (int k=0; k < 5; k++) {
        j1->cartas[k][0] = (rand() % 13) + 1; // Numero
        j1->cartas[k][1] = (rand() % 4) + 1;  // Pinta

        j2->cartas[k][0] = (rand() % 13) + 1;
        j2->cartas[k][1] = (rand() % 4) + 1;
    }
}

// Cambio de cartas de cada ronda. Se ejecuta al recibir RET_CARDS_CHNG
void cambiar_cartas(int n, int cartas[n][2], Jugador* j) {
    for (int i=0; i < n; i++) {
        for (int k=0; k < 5; k++) {
            if (j->cartas[k][0] == cartas[i][0] && j->cartas[k][1] == cartas[i][1]) {
                j->cartas[k][0] = (rand() % 13) + 1; // Numero
                j->cartas[k][1] = (rand() % 4) + 1;  // Pinta
            }
        }
    }
}

// Decide quien parte esta vuelta y le envia el msje
void who_first(Partida* p) {
    for (int i=0; i < 2; i++) {
        if (i == p->dealer) {
            Jugador* j = p->jugadores[i];
            // ENVIAR MENSAJE A J
            p->dealer = (p->dealer + 1) % 2;
            return;
        }
    }
}

// Despues de mandar GET_BET y de recibir RET_BET
bool manage_bet(Partida* p, Jugador* j, int bet_code, int* status_code) {
    int bet = 0;
    // Jugador* j = p->jugadores[p->turno];
    switch (bet_code) {
        case FOLD:
            *status_code = OK_BET;
            return true; // La vuelta esta lista
        case BET_0:
            bet = 0;
            break;
        case BET_100:
            bet = 100;
            break;
        case BET_200:
            bet = 200;
            break;
        case BET_500:
            bet = 500;
            break;
        default:
            *status_code = ERR_BET;
            return false; // La vuelta no esta lista
    }
    if (j->pot < bet) {
        *status_code = ERR_BET;
        return false;
    }
    if (j->pot >= bet) {
        if (p->pozo == bet) { // Iguala la apouesta
            *status_code = OK_BET;
            return true;
        }
        if (p->pozo < bet) { // Sube la apuesta
            p->pozo = bet;
            *status_code = OK_BET;
            // p->turno = 1 - p->turno;
            // ENVIAR GET_BET A P->JUGADORES[P->TURNO]
            return false;
        }
    }
}

int evaluar_mano(int cartas[5][2]) {
    int pintas_en_mano[4];
    int nums_en_mano[13];

    for (int i=0; i<4; i++) {
        pintas_en_mano[i] = count_pintas(cartas, i);
    }
    for (int i=0; i<13; i++) {
        nums_en_mano[i] = count_nums(cartas, i);
    }

    int num_consec = 0;
 
    bool escala = false;
    bool color = false;
    bool poker = false;
    bool trio = false;
    int pares = 0;

    for (int pinta = 0; pinta < 4; pinta++)
        if (pintas_en_mano[pinta] == 5) color = true;

    int num = 0;
    while (nums_en_mano[num] == 0) num++;
    for (; num < 13 && nums_en_mano[num] > 0; num++)
        num_consec++;
    if (num_consec == 5) {
        escala = true;
    }
    
    for (num = 0; num < 13; num++) {
        if (nums_en_mano[num] == 4) poker = true;
        if (nums_en_mano[num] == 3) trio = true;
        if (nums_en_mano[num] == 2) pares++;
    }

    // Retornar la mano mas alta
    if (poker) return POKER;
    if (color) return COLOR;
    if (escala) return ESCALA;
    if (trio) return TRIO;
    if (pares == 2) return DOS_PARES;
    if (pares == 1) return PAR;
    return 0; // Nada
}

int count_pintas(int cartas[5][2], int pinta) {
    int cont = 0;
    for (int i=0; i < 5; i++) {
        if (cartas[i][1] == pinta) cont++;
    }
    return cont;
}

int count_nums(int cartas[5][2], int num) {
    int cont = 0;
    for (int i=0; i < 5; i++) {
        if (cartas[i][0] == num) cont++;
    }
    return cont;
}

void print_cartas(int cartas[][2], int n) {
    for (int i=0; i < n; i++) {
        wprintf(L"%d%lc - ", cartas[i][0], pintas_repr[cartas[i][1]-1]);
    }
    wprintf(L"\n");
}

// Libera el heap
void free_memory(Partida* p) {
    for (int i=0; i < 2; i++) {
        free(p->jugadores[i]);
    }
    free(p);
}