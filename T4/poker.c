#include "include/poker.h"

int pintas[4] = {HEART, DMOND, CLOVR, SPADE};
int cartas[13] = {A,2};

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
    }
}

void print_cartas(int cartas[][2], int n) {
    int pintas[4] = {HEART_REPR, DMOND_REPR, CLOVR_REPR, SPADE_REPR};
    for (int i=0; i < n; i++) {
        wprintf(L"%d%lc - ", cartas[i][0], pintas[cartas[i][1]-1]);
    }
    wprintf(L"\n");
}