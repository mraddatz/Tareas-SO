#include "include/poker.h"

int pintas[4] = {HEART, DMOND, CLOVR, SPADE};
int pintas_repr[4] = {HEART_REPR, DMOND_REPR, CLOVR_REPR, SPADE_REPR};
int cartas[13] = {A,2};

Partida* partida_init() {
    Partida* p = malloc(sizeof(Partida));
    p->dealer = 0;
    p->num_jugadores = 0;
    return p;
}

Jugador* jugador_init(char* nombre) {
    Jugador* j = malloc(sizeof(Jugador));
    j->pot = 1000;
    j->nombre = nombre;
    // INICIALIZAR SOCKET
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
void repartir_cartas(Jugador* jugadores) {
    srand(12345);
    int n = sizeof(jugadores) / sizeof(Jugador);
    for (int i=0; i < n; i++) {
        for (int k=0; k < 5; k++) {
            jugadores[i].cartas[k][0] = (rand() % 13) + 1; // Numero
            jugadores[i].cartas[k][1] = (rand() % 4) + 1;  // Pinta
        }
        // ENVIAR PAQUETE FIVE_CARDS A J.SOCKET
    }
}

// Cambio de cartas de cada ronda. Se ejecuta al recibir RET_CARDS_CHNG
void cambiar_cartas(int cartas[][2], int n, Jugador* j) {
    int nuevas_cartas[n][2];
    int cont = 0;
    for (int i=0; i < n; i++) {
        for (int k=0; k < 5; k++) {
            if (j->cartas[k][0] == cartas[i][0] && j->cartas[k][1] == cartas[i][1]) {
                j->cartas[k][0] = (rand() % 13) + 1; // Numero
                j->cartas[k][1] = (rand() % 4) + 1;  // Pinta
                nuevas_cartas[cont][0] = j->cartas[k][0];
                nuevas_cartas[cont][1] = j->cartas[k][1];
            }
        }
    //  ENVIAR A CLIENTE LAS CARTAS NUEVAS_CARTAS
    }
}

// Decide quien parte esta vuelta y le envia el msje
void who_first(Partida* p) {
    for (int i=0; i < p->num_jugadores; i++) {
        if (i == p->dealer) {
            Jugador j = p->jugadores[i];
            // ENVIAR MENSAJE A J
            p->dealer = (p->dealer + 1) % p->num_jugadores;
            return;
        }
    }
}

void print_cartas(int cartas[][2], int n) {
    for (int i=0; i < n; i++) {
        wprintf(L"%d%lc - ", cartas[i][0], pintas_repr[cartas[i][1]-1]);
    }
    wprintf(L"\n");
}

// Libera el heap
void free_memory(Partida* p) {
    for (int i=0; i < p->num_jugadores; i++) {
        free(&(p->jugadores[i]));
    }
    free(p);
}