#include "include/main.h"
#include "include/structs.h"
#include "include/poker.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){
    setlocale(LC_CTYPE, ""); // Para poder imprimir las pintas
    wprintf(L"Bienvenido al Poker.\n");
    wprintf(L"Pintas: %lc %lc %lc %lc\n", HEART_REPR, CLOVR_REPR, DMOND_REPR, SPADE_REPR);
    int cartas[3][2] = {{3,2}, {10,3}, {2,1}};
    print_cartas(cartas, 3);
    wprintf(L"fin\n");
}
