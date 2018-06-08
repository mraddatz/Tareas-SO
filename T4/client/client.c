#include "client.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int enviar_mensaje(int socket, int id, int size){
    mensaje msg;
    msg.message_type_id = id;
	return write(socket, &msg, sizeof(msg));
}

int leer_mensaje(mensaje *msg, int id_socket){
    if (read(id_socket, msg, sizeof(msg)) < 0) error("ERROR reading from socket");
    //printf("Here is the message: %d\n", msg->message_type_id);
    //fflush(stdout);
    return 1;
}

void esperar_mensaje(mensaje *msg, int socket){
    while (msg->message_type_id == 0u){
     leer_mensaje(msg, socket);
    }
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

int compare(mensaje msg, unsigned char id){
	return msg.message_type_id == id;
}

void connection_established(char nickname, char nickname_oponente, mensaje msg, int socket){}

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
    //mensaje al servidor
    n = enviar_mensaje(sockfd, 1u, 2u);
    msg_servidor.message_type_id = 0u;

    //esperar a que el servidor pida el nickname y enviarlo

    esperar_mensaje(&msg_servidor, sockfd); //acknowledge
	compare_print(&msg_servidor, 2u, "El servidor ha respondido\n");
	esperar_mensaje(&msg_servidor, sockfd);
	if (compare_print(&msg_servidor, 3u, "Enviando nickname del jugador\n")){
		enviar_mensaje(sockfd, 4u, 0u);
	}

	//recibir nickname adversario
	esperar_mensaje(&msg_servidor, sockfd);
	if (compare_print(&msg_servidor, 5u, "Recibido nickname del jugador contrario\n")){
		enviar_mensaje(sockfd, 4u, 0u);
	}

    close(sockfd);

    return 0;
}
