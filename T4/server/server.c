/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include "server.h"

void error(const char *msg)
{
    perror(msg);
    exit(1);
}

int enviar_mensaje(int socket, int id, int size){
    mensaje msg;
    msg.message_type_id = id;
	return write(socket, &msg, sizeof(msg));
}

int enviar_mensaje_payload(int socket, mensaje *msg){
	return write(socket, msg, sizeof(*msg));
}

int leer_mensaje(mensaje *msg, int id_socket){
    if (read(id_socket, msg, sizeof(*msg)) < 0) error("ERROR reading from socket");
    //printf("Here is the message: %d\n", msg->message_type_id);
    //fflush(stdout);
    return 1;
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

void esperar_mensaje(mensaje *msg, int socket){
    while (msg->message_type_id == 0u){
     leer_mensaje(msg, socket);
    }
}

int compare(mensaje msg, unsigned char id){
	return msg.message_type_id == id;
}

int main(int argc, char *argv[]){
     int id_socket_in, socket_cliente_1, socket_cliente_2, numero_puerto_input;
     socklen_t cli_addr_size;
     struct sockaddr_in serv_addr, cli_addr;
     if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }


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



     //Esperar mensaje
     listen(id_socket_in,5);

     cli_addr_size = sizeof(cli_addr);
     socket_cliente_1 = accept(id_socket_in,
                 (struct sockaddr *) &cli_addr, 
                 &cli_addr_size);


     //leer mensaje
     mensaje msg_cliente_1, msg_cliente_2;
     msg_cliente_1.message_type_id = 0u;
     msg_cliente_2.message_type_id = 0u;



     esperar_mensaje(&msg_cliente_1, socket_cliente_1);
     if (compare_print(&msg_cliente_1, 1u, "conectado a cliente 1\n")){
         enviar_mensaje(socket_cliente_1, 2u, 0u);
         usleep(2);
         enviar_mensaje(socket_cliente_1, 3u, 0u);
     }

     socket_cliente_2 = accept(id_socket_in,
                 (struct sockaddr *) &cli_addr,
                 &cli_addr_size);
     esperar_mensaje(&msg_cliente_2, socket_cliente_2);

     if (compare_print(&msg_cliente_2, 1u, "conectado a cliente 2\n")){
         enviar_mensaje(socket_cliente_2, 2u, 0u);
         usleep(2);
         enviar_mensaje(socket_cliente_2, 3u, 0u);
     }
     esperar_mensaje(&msg_cliente_2, socket_cliente_2);
     if (compare_print(&msg_cliente_2, 4u, "nickname jugador 2 recibido\n")){
    	 mensaje nick_msg_2;
    	 memcpy(&nick_msg_2, &msg_cliente_2, sizeof(msg_cliente_2));
    	 nick_msg_2.message_type_id = 5u;
    	 enviar_mensaje_payload(socket_cliente_1, &nick_msg_2);
    	 //enviar_mensaje(socket_cliente_1, 5u, 0u);
     }

     esperar_mensaje(&msg_cliente_1, socket_cliente_1);
     if (compare_print(&msg_cliente_1, 4u, "nickname jugador 1 recibido\n")){
    	 mensaje nick_msg_1;
    	 memcpy(&nick_msg_1, &msg_cliente_1, sizeof(msg_cliente_1));
    	 nick_msg_1.message_type_id = 5u;
    	 enviar_mensaje_payload(socket_cliente_2, &nick_msg_1);
     }


     //mensaje initial_pot;
     mensaje initial_pot;
     initial_pot.message_type_id = 6u;
     int pot = 1000;
     initial_pot.size = sizeof(pot);
     memcpy(&initial_pot.payload, &pot,initial_pot.size);
     enviar_mensaje_payload(socket_cliente_1, &initial_pot);
     enviar_mensaje_payload(socket_cliente_2, &initial_pot);

     mensaje game_start;
     game_start.message_type_id = 7u;
     game_start.size = 0u;
     enviar_mensaje_payload(socket_cliente_1, &game_start);
     enviar_mensaje_payload(socket_cliente_2, &game_start);

     mensaje start_round;
     start_round.message_type_id = 8u;
     pot = 200;
     start_round.size = sizeof(pot);
     memcpy(&start_round.payload, &pot, start_round.size);
     enviar_mensaje_payload(socket_cliente_1, &start_round);
     enviar_mensaje_payload(socket_cliente_2, &start_round);

     mensaje initial_bet;
     initial_bet.message_type_id = 9u;
     int bet = 10;
     initial_bet.size = sizeof(bet);
     memcpy(&initial_bet.payload, &bet, initial_bet.size);
     enviar_mensaje_payload(socket_cliente_1, &initial_bet);
     enviar_mensaje_payload(socket_cliente_2, &initial_bet);

     mensaje first;
     first.message_type_id = 11u;
     unsigned char player = 1;
     first.size = sizeof(player);
     memcpy(&first.payload, &player, first.size);
     enviar_mensaje_payload(socket_cliente_1, &first);
     enviar_mensaje_payload(socket_cliente_2, &first);


     close(socket_cliente_1);
     close(socket_cliente_2);
     close(id_socket_in);
     return 0; 
}
