#include "client.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
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

int enviar_mensaje_payload(int socket, mensaje *msg){
	return write(socket, msg, sizeof(*msg));
}

int leer_mensaje(mensaje *msg, int id_socket){
    if (read(id_socket, msg, sizeof(mensaje)) < 0) error("ERROR reading from socket");
    return 1;
}

void esperar_mensaje(mensaje *msg, int socket){
    while (msg->message_type_id == NULL_MSG){
     leer_mensaje(msg, socket);
    }
}

int compare_print(mensaje *msg, unsigned char id, char *print){
	if (msg->message_type_id == id){
	    msg->message_type_id = NULL_MSG;
		printf(print);
		fflush(stdout);
		return 1;
	}else{
		printf("failed, rcvd %d and expecting %d\n", msg->message_type_id, id);
	    msg->message_type_id = NULL_MSG;
		fflush(stdout);
		return 0;
	}
}

int compare(mensaje msg, unsigned char id){
	return msg.message_type_id == id;
}

int (*acciones[30]) (char nickname, char nickname_oponente, mensaje *msg, int socket);

int ejecutar_accion(int (*acciones[])(char *nickname, char *nickname_oponente, mensaje *msg, int socket), int accion,char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	return 	(*acciones[accion])(nickname, nickname_oponente, msg, socket);
}

int connection_established(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	printf("Connection established\n");
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

int begin_connection(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	printf("Iniciando conexion\n");
	fflush(stdout);
    enviar_mensaje(socket, 1u, 0u, "");
    msg->message_type_id = 0u;
	return 1;
}

int send_nickname(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
    printf("Enter your nickname: \n");
	fflush(stdout);
    //scanf("%s", nickname);
	printf("Your nickname is: %s\n", nickname);
	printf("Sending nickname\n");
	msg->message_type_id = 4u;
	memcpy(msg->payload, nickname, 10);
	msg->size = 10;
    enviar_mensaje_payload(socket, msg);
    msg->message_type_id = 0u;
	return 1;
}

int oponent_nickname(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	printf("Recibido nickname contrario\n");
	memcpy(nickname_oponente, msg->payload, msg->size);
	printf("Your opponents nickname is: %s\n", nickname_oponente);
	fflush(stdout);
	//nickname_oponente
    msg->message_type_id = 0u;
	return 1;
}

int initial_pot(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	int pot;
	memcpy(&pot, &msg->payload, msg->size);
	printf("Your initial pot is: %d\n", pot);
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

int game_start(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	printf("Game started correctly\n");
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

int start_round(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	int pot;
	memcpy(&pot, &msg->payload, msg->size);
	printf("Your current pot is: %d\n", pot);
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

int initial_bet(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	int bet;
	memcpy(&bet, &msg->payload, msg->size);
	printf("Your initial bet is: %d\n", bet);
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

int turn(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	unsigned char player;
	memcpy(&player, &msg->payload, msg->size);
	if (player == 1){
		printf("Your turn\n");
	}else{
		printf("Oponents turn\n");
	}
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

//int change_cards(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
//	unsigned char change[5] = {1u, 0u, 1u, 0u, 0u};
//	printf("Insert cards you want to change: ");
//	printf("\n");
//    msg->message_type_id = 0u;
//
//	mensaje apuesta;
//    printf("Enter your bet id: \n");
//	fflush(stdout);
//	//scanf("%d", bet);
//	printf("Sending bet\n");
//	apuesta.message_type_id = 15u;
//	memcpy(&apuesta.payload, &bet, sizeof(bet));
//	fflush(stdout);
//	apuesta.size = sizeof(bet);
//    enviar_mensaje_payload(socket, &apuesta);
//	return 1;
//}

int get_bet(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	unsigned char bets[msg->size];
	memcpy(&bets, &msg->payload, msg->size);
	char *prints[5] = {" fold(1)", " 0(2)", " 100(3)", " 200(4)", " 500(5)"};
	printf("Your bet option ids are:");
	for (int i=0; i < msg->size; i++){
		printf(prints[bets[i]-1u]);
	}
	printf("\n");
    msg->message_type_id = 0u;

	unsigned char bet = 1u;
	mensaje apuesta;
    printf("Enter your bet id: \n");
	fflush(stdout);
	//scanf("%d", bet);
	printf("Sending bet\n");
	apuesta.message_type_id = 15u;
	memcpy(&apuesta.payload, &bet, sizeof(bet));
	fflush(stdout);
	apuesta.size = sizeof(bet);
    enviar_mensaje_payload(socket, &apuesta);
	return 1;
}

//17
int OK_bet(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	printf("Bet accepted\n");
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

//18
int round_ended(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	printf("End of round\n");
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}

int five_cards(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	unsigned char cards[msg->size];
	memcpy(&cards, &msg->payload, msg->size);
	char *cartas[13] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10",
			"J", "Q", "K"};
	char pintas[4] = {HEART_REPR, DMOND_REPR, CLOVR_REPR, SPADE_REPR};
	printf("Your cards are:");
	for (int i=0; i < msg->size; i++){
		//wprintf("%s%s",cartas[cards[i]-1u], pintas[cards[++i]-1u]);
	}
	msg->message_type_id = 0;
	printf("\n");
	fflush(stdout);
	return 1;
}

//20
int winner(char *nickname, char *nickname_oponente, mensaje *msg, int socket){
	unsigned char player;
	memcpy(&player, &msg->payload, msg->size);
	if (player == 1){
		printf("Your turn\n");
	}else{
		printf("Oponents turn\n");
	}
	fflush(stdout);
    msg->message_type_id = 0u;
	return 1;
}


int main(int argc, char *argv[])
{
    int sockfd, numero_puerto_input, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256];
	//Necesito argumentos de puerto y server
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    //fijo puerto
    numero_puerto_input = atoi(argv[2]);
    //Creo socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    //Error
    if (sockfd < 0) 
        error("ERROR opening socket");
    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(numero_puerto_input);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    //
    //Aca empieza lo importante
    //

    char nickname[100];
    char nickname_oponente[100];
    acciones[1] = begin_connection;
    acciones[2] = connection_established;
    acciones[3] = send_nickname;
    acciones[5] = oponent_nickname;
    acciones[6] = initial_pot;
    acciones[7] = game_start;
    acciones[8] = start_round;
    acciones[9] = initial_bet;
    acciones[10] = five_cards;
    acciones[11] = turn;
    acciones[14] = get_bet;
    acciones[17] = OK_bet;
    acciones[18] = round_ended;
    mensaje msg_servidor;
    msg_servidor.message_type_id = NULL_MSG;

    // //mensaje al servidor
    // n = enviar_mensaje(sockfd, START_CONN, 0u, "");
    // //esperar a que el servidor pida el nickname y enviarlo

    // esperar_mensaje(&msg_servidor, sockfd); //acknowledge
	// compare_print(&msg_servidor, CONN_ESTAB, "Conexion establecida.\n");
	// esperar_mensaje(&msg_servidor, sockfd);
	// if (compare_print(&msg_servidor, ASK_NICK, "Enviando nickname del jugador\n")){
    //     char nick[254];
    //     printf("Ingresa tu nick: ");
    //     scanf("%s",nick);
	// 	enviar_mensaje(sockfd, RET_NICK, strlen(nick), nick);
	// }
    // printf("Esperando oponente...\n");

	// //recibir nickname adversario
	// esperar_mensaje(&msg_servidor, sockfd);
	// if (compare_print(&msg_servidor, OPNT_FOUND, "Recibido nickname del jugador contrario\n")){
	// 	printf("Oponente: %s\n", msg_servidor.payload);
	// }


    //mensaje al servidor
    //ejecutar_accion(acciones, CONN_ESTAB, '1', 'b', &msg_servidor, sockfd);
	//(*acciones[START_CONN])("", "", msg_servidor, sockfd);

    ejecutar_accion(acciones, START_CONN, &nickname, &nickname_oponente, &msg_servidor, sockfd);

    while (msg_servidor.message_type_id != 30){
    	esperar_mensaje(&msg_servidor, sockfd);
    	printf("recibido %D\n", msg_servidor.message_type_id);
        ejecutar_accion(acciones, msg_servidor.message_type_id, nickname, nickname_oponente, &msg_servidor, sockfd);
    }



    close(sockfd);

    return 0;
}
