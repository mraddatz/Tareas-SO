/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include "include/server.h"
#include "include/poker.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int enviar_mensaje(int socket, unsigned char id, unsigned char size, char* payload){
    sleep(1);
    unsigned char buffer[256];
    buffer[0] = id;
    buffer[1] = size;
    int marker = 2;
    for (int i = 0; i < size; i++) {
        buffer[marker++] = payload[i];
    }
	return write(socket, buffer, marker);
}

void esperar_mensaje(mensaje *msg, int socket) {
    while (msg->message_type_id == 0u){
     leer_mensaje(msg, socket);
    }
}

int leer_mensaje(mensaje *msg, int id_socket) {
    if (read(id_socket, msg, sizeof(*msg)) < 0) error("ERROR reading from socket");
    return 1;
}

int enviar_mensaje_payload(int socket, mensaje *msg){
	return write(socket, msg, sizeof(*msg));
}

int compare(mensaje msg, unsigned char id) {
	return msg.message_type_id == id;
}

int compare_print(mensaje *msg, unsigned char id, char *print){ 
	if (msg->message_type_id == id){
	    msg->message_type_id = 0u;
		printf(print);
		fflush(stdout);
		return 1;
	}else{
		printf("failed, %d\n", msg->message_type_id);
	    msg->message_type_id = 0u;
		fflush(stdout);
		return 0;
	}
}

void desarmar_paquete_cartas(char* payload, int n, int cartas[n][2]) {
    for (int i=0; i < n; i++) {
        cartas[i][0] = (int)payload[2*i];
        cartas[i][1] = (int)payload[2*i+1];
    }
}

void armar_paquete_cartas(char* payload, int cartas[5][2]) {
    char buff[1];
    int cont = 0;
    for (int i=0; i<5; i++) {
        getBin(cartas[i][0], buff);
        payload[cont++] = buff[0];
        getBin(cartas[i][1], buff);
        payload[cont++] = buff[0];
    }
}

void armar_paquete_apuestas(char* payload, int apuestas[5], int* apuesta_size) {
    char buff[1];
    int cont = 0;
    *apuesta_size = 0;
    for (int i=0; i<5; i++) {
        if (i != 0) {
            *apuesta_size++;
            getBin(apuestas[i], buff);
            payload[cont++] = buff[0];
        }
    }
}


void getBin(int num, char *str) {
    unsigned byte = 0;
    int cont = 0;
    do {
        for (int i=0; i < 8; i++) {
            if (num % 2) byte += pow(2, i);
            num >>= 1;
        }
        str[cont] = (char)byte;
        cont++;
        byte = 0;
    } while (num != 0);
    str[cont] = '\0';
}

int main(int argc, char *argv[]) {
    int id_socket_in, socket_cliente_1, socket_cliente_2, numero_puerto_input;
    socklen_t cli_addr_size;
    struct sockaddr_in serv_addr, cli_addr;
    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
    srand(12345);


    id_socket_in = socket(AF_INET, SOCK_STREAM, 0);
    if (id_socket_in < 0)
    error("ERROR opening socket");
    bzero((char *) &serv_addr, sizeof(serv_addr));
    numero_puerto_input = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(numero_puerto_input);
    if (bind(id_socket_in, (struct sockaddr *) &serv_addr,
            sizeof(serv_addr)) < 0) 
            error("ERROR on binding");

    // Inicializar mensajes
    mensaje msg_cliente_1, msg_cliente_2;
    msg_cliente_1.message_type_id = NULL_MSG;
    msg_cliente_2.message_type_id = NULL_MSG;

    char int_buffer[10];

    // Aceptar conexiones
    listen(id_socket_in,5);

    cli_addr_size = sizeof(cli_addr);
    socket_cliente_1 = accept(id_socket_in,
                (struct sockaddr *) &cli_addr, 
                &cli_addr_size);

    esperar_mensaje(&msg_cliente_1, socket_cliente_1);
    if (compare_print(&msg_cliente_1, START_CONN, "conectado a cliente 1\n")){
        enviar_mensaje(socket_cliente_1, CONN_ESTAB, 0u, NULL_PAYLOAD);
        enviar_mensaje(socket_cliente_1, ASK_NICK, 0u, NULL_PAYLOAD);
    }
    esperar_mensaje(&msg_cliente_1, socket_cliente_1);
    if (compare_print(&msg_cliente_1, RET_NICK, "nickname jugador 1 recibido\n")){
        printf("El nick 1 es %s\n", msg_cliente_1.payload);
    }

    socket_cliente_2 = accept(id_socket_in,
                (struct sockaddr *) &cli_addr,
                &cli_addr_size);
    esperar_mensaje(&msg_cliente_2, socket_cliente_2);

    if (compare_print(&msg_cliente_2, START_CONN, "conectado a cliente 2\n")){
        enviar_mensaje(socket_cliente_2, CONN_ESTAB, 0u, NULL_PAYLOAD);
        enviar_mensaje(socket_cliente_2, ASK_NICK, 0u, NULL_PAYLOAD);
    }
    esperar_mensaje(&msg_cliente_2, socket_cliente_2);

    if (compare_print(&msg_cliente_2, RET_NICK, "nickname jugador 2 recibido\n")){
        printf("El nick 2 es %s\n", msg_cliente_2.payload);
        enviar_mensaje(socket_cliente_1, OPNT_FOUND, msg_cliente_2.size, msg_cliente_2.payload);
        enviar_mensaje(socket_cliente_2, OPNT_FOUND, msg_cliente_1.size, msg_cliente_1.payload);
        getBin(INITIAL_POT, int_buffer);
        enviar_mensaje(socket_cliente_1, INIT_POT, 2u, int_buffer);
        enviar_mensaje(socket_cliente_2, INIT_POT, 2u, int_buffer);
        enviar_mensaje(socket_cliente_1, GAME_START, 0u, NULL_PAYLOAD);
        enviar_mensaje(socket_cliente_2, GAME_START, 0u, NULL_PAYLOAD);
    }

    Jugador* j1 = jugador_init(msg_cliente_1.payload, socket_cliente_1);
    Jugador* j2 = jugador_init(msg_cliente_2.payload, socket_cliente_2);
    Partida* p = partida_init(j1, j2);
    repartir_cartas(j1, j2);
    while (true) {
        getBin(j1->pot, int_buffer);
        enviar_mensaje(j1->socket, START_RND, 2u, int_buffer);
        getBin(j2->pot, int_buffer);
        enviar_mensaje(j2->socket, START_RND, 2u, int_buffer);

        if (j1->pot >= 10) {
            enviar_mensaje(j1->socket, INIT_BET, 0u, NULL_PAYLOAD);
            j1->pot -= 10;
        }
        else {
            getBin(2u, int_buffer);
            enviar_mensaje(j1->socket, WIN_LOSE, 1u, int_buffer);
            getBin(1u, int_buffer);
            enviar_mensaje(j2->socket, WIN_LOSE, 1u, int_buffer);
        }
        if (j2->pot >= 10) {
            enviar_mensaje(j2->socket, INIT_BET, 0u, NULL_PAYLOAD);
            j2->pot -= 10;
        }
        else {
            getBin(1u, int_buffer);
            enviar_mensaje(j1->socket, WIN_LOSE, 1u, int_buffer);
            getBin(2u, int_buffer);
            enviar_mensaje(j2->socket, WIN_LOSE, 1u, int_buffer);
        }
        p->pozo = 10;

        armar_paquete_cartas(int_buffer, j1->cartas);
        enviar_mensaje(j1->socket, FIVE_CARDS, 10u, int_buffer);

        armar_paquete_cartas(int_buffer, j2->cartas);
        enviar_mensaje(j2->socket, FIVE_CARDS, 10u, int_buffer);

        Jugador* primero;
        Jugador* segundo;
        switch(p->dealer) {
            case 0:
                primero = j1;
                segundo = j2;
                getBin(1u, int_buffer);
                enviar_mensaje(j1->socket, WHO_FIRST, 1u, int_buffer);
                getBin(2u, int_buffer);
                enviar_mensaje(j2->socket, WHO_FIRST, 1u, int_buffer);
                break;
            case 1:
                primero = j2;
                segundo = j1;
                getBin(2u, int_buffer);
                enviar_mensaje(j1->socket, WHO_FIRST, 1u, int_buffer);
                getBin(1u, int_buffer);
                enviar_mensaje(j2->socket, WHO_FIRST, 1u, int_buffer);
                break;
            default:
                p->dealer = 1 - p->dealer;
        }

        enviar_mensaje(j1->socket, GET_CARDS_CHNG, 0u, NULL_PAYLOAD);
        esperar_mensaje(&msg_cliente_1, j1->socket);
        if (compare_print(&msg_cliente_1, RET_CARDS_CHNG, "cartas jugador 1 recibidas\n")){
            int cartas_cambio[5][2];
            desarmar_paquete_cartas(msg_cliente_1.payload, msg_cliente_1.size, cartas_cambio);
            cambiar_cartas(msg_cliente_1.size, cartas_cambio, j1);
            armar_paquete_cartas(int_buffer, j1->cartas);
            enviar_mensaje(j1->socket, FIVE_CARDS, 10u, int_buffer);
        }
        enviar_mensaje(j2->socket, GET_CARDS_CHNG, 0u, NULL_PAYLOAD);
        esperar_mensaje(&msg_cliente_1, j1->socket);
        if (compare_print(&msg_cliente_2, RET_CARDS_CHNG, "cartas jugador 2 recibidas\n")){
            int cartas_cambio[5][2];
            desarmar_paquete_cartas(msg_cliente_2.payload, msg_cliente_2.size, cartas_cambio);
            cambiar_cartas(msg_cliente_2.size, cartas_cambio, j2);
            armar_paquete_cartas(int_buffer, j2->cartas);
            enviar_mensaje(j2->socket, FIVE_CARDS, 10u, int_buffer);
        }
        int* apuesta_size;
        int apuestas[5] = {0, BET_0, 0, 0, 0};
        if (primero->pot >= 100) apuestas[BET_100 - 1] = BET_100;
        if (primero->pot >= 200) apuestas[BET_200 - 1] = BET_200;
        if (primero->pot >= 500) apuestas[BET_500 - 1] = BET_500;
        armar_paquete_apuestas(int_buffer, apuestas, apuesta_size);
        enviar_mensaje(primero->socket, GET_BET, apuesta_size, int_buffer);
    }
    close(j1->socket);
    close(j1->socket);
    close(id_socket_in);
    free_memory(p);
    return 0; 
}
