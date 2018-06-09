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

int leer_mensaje(mensaje *msg, int id_socket){
    if (read(id_socket, msg, sizeof(mensaje)) < 0) error("ERROR reading from socket");
    msg->payload[msg->size] = '\0'; // Limitar payload segun size

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

    mensaje msg_servidor;
    msg_servidor.message_type_id = NULL_MSG;

    //mensaje al servidor
    n = enviar_mensaje(sockfd, START_CONN, 0u, "");
    //esperar a que el servidor pida el nickname y enviarlo

    esperar_mensaje(&msg_servidor, sockfd); //acknowledge
	compare_print(&msg_servidor, CONN_ESTAB, "Conexion establecida.\n");
	esperar_mensaje(&msg_servidor, sockfd);
	if (compare_print(&msg_servidor, ASK_NICK, "Enviando nickname del jugador\n")){
        char nick[254];
        printf("Ingresa tu nick: ");
        scanf("%s",nick);
		enviar_mensaje(sockfd, RET_NICK, strlen(nick), nick);
	}
    printf("Esperando oponente...\n");

	//recibir nickname adversario
	esperar_mensaje(&msg_servidor, sockfd);
	if (compare_print(&msg_servidor, OPNT_FOUND, "Recibido nickname del jugador contrario\n")){
		printf("Oponente: %s\n", msg_servidor.payload);
	}

    close(sockfd);

    return 0;
}
