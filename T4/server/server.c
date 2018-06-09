/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include "include/server.h"
#include "include/poker.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

int enviar_mensaje(int socket, unsigned char id, unsigned char size, char* payload){
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
    if (read(id_socket, msg, sizeof(mensaje)) < 0) error("ERROR reading from socket");
    msg->payload[msg->size] = '\0'; // Limitar payload segun size
    return 1;
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

void getBin(int num, char *str) {
    unsigned byte = 0;
    int cont = 0;
    do {
        for (int i=0; i < 8; i++) {
            if (num % 2) byte += pow(2, i);
            num >>= 1;
        }
        str[cont] = byte;
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

    char pot_buffer[4];

    // Aceptar conexiones
    listen(id_socket_in,5);

    cli_addr_size = sizeof(cli_addr);
    socket_cliente_1 = accept(id_socket_in,
                (struct sockaddr *) &cli_addr, 
                &cli_addr_size);

    esperar_mensaje(&msg_cliente_1, socket_cliente_1);
    if (compare_print(&msg_cliente_1, START_CONN, "conectado a cliente 1\n")){
        enviar_mensaje(socket_cliente_1, CONN_ESTAB, 0u, NULL_PAYLOAD);
        sleep(1);
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
        sleep(1);
        enviar_mensaje(socket_cliente_2, ASK_NICK, 0u, NULL_PAYLOAD);
    }
    esperar_mensaje(&msg_cliente_2, socket_cliente_2);

    if (compare_print(&msg_cliente_2, RET_NICK, "nickname jugador 2 recibido\n")){
        printf("El nick 2 es %s\n", msg_cliente_2.payload);
        enviar_mensaje(socket_cliente_1, OPNT_FOUND, msg_cliente_2.size, msg_cliente_2.payload);
        enviar_mensaje(socket_cliente_2, OPNT_FOUND, msg_cliente_1.size, msg_cliente_1.payload);
        sleep(1);
        getBin(INITIAL_POT, pot_buffer);
        enviar_mensaje(socket_cliente_1, INIT_POT, 2u, pot_buffer);
        enviar_mensaje(socket_cliente_2, INIT_POT, 2u, pot_buffer);
        sleep(1);
        enviar_mensaje(socket_cliente_1, GAME_START, 0u, NULL_PAYLOAD);
        enviar_mensaje(socket_cliente_2, GAME_START, 0u, NULL_PAYLOAD);
    }

    Jugador* j1 = jugador_init(msg_cliente_1.payload);
    Jugador* j2 = jugador_init(msg_cliente_2.payload);
    Partida* p = partida_init(j1, j2);
    repartir_cartas(j1, j2);
    
    while (true) {
        getBin(j1->pot, pot_buffer);
        enviar_mensaje(socket_cliente_1, START_RND, 1u, j1->pot);
        getBin(j2->pot, pot_buffer);
        enviar_mensaje(socket_cliente_2, START_RND, 1u, j2->pot);

        if (j1->pot >= 10) {
            enviar_mensaje(socket_cliente_1, INIT_BET, 0u, NULL_PAYLOAD);
            j1->pot -= 10;
        }
        else {
            enviar_mensaje(socket_cliente_1, WIN_LOSE, 1u, 2u);
            enviar_mensaje(socket_cliente_2, WIN_LOSE, 1u, 1u);
        }
        if (j2->pot >= 10) {
            enviar_mensaje(socket_cliente_2, INIT_BET, 0u, NULL_PAYLOAD);
            j2->pot -= 10;
        }
        else {
            enviar_mensaje(socket_cliente_1, WIN_LOSE, 1u, 1u);
            enviar_mensaje(socket_cliente_2, WIN_LOSE, 1u, 2u);
        }

    }

    close(socket_cliente_1);
    close(socket_cliente_2);
    close(id_socket_in);
    return 0; 
}
