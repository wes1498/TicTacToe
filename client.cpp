#include <iostream>
#include <iostream>
#include "board.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg)
{
    #ifdef DEBUG
    perror(msg);
    #else
    printf("The game Disconnected.\nGame over.\n");
    #endif 

    exit(0);
}

int connect_client_socket(char * host_name, int port_number)
{
    // Create network socket
    int network_socket;
    
    struct hostent *server;

    /* AF_INET = IPv4 Internet protocols 
       SOCK_STREAM = TCP SOCKET
       SOCKET_PROTOCOL set to 0 == default TCP */

    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (network_socket < 0) 
        error("ERROR opening socket for server.");
    server = gethostbyname(hostname);

    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    // specify address for the socket
    struct sockaddr_in server_address;
	
	// Zero out memory for server info.
	//memset(&server_address, 0, sizeof(server_address));

	/* Set up the server info. */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(portno); 
    memmove(server->h_addr, &server_address.sin_addr.s_addr, server->h_length);
    

	/* Make the connection. */
    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));	

    if (connection_status == -1) {
        error("Error in connecting to the server.");
        //printf("There was an error connecting to the remote socket.\n\n");
    }
    return network_socket;
}
void receive_message(int network_socket, char* message)
{
    // 4 letter message
    memset(message, 0, 4);
    int n = read(network_socket, message, 3);
}

int main(int argc, char *argv[]){

    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    int network_socket = connect_client_socket(argv[1], atoi(argv[2]));
    char board[3][3] = { {' ', ' ', ' '}, /* Game Board */ 
                        {' ', ' ', ' '}, 
                        {' ', ' ', ' '} };
    printf("Tic-Tac-Toe\n\n");

    printf("What team are you on? %c's\n", id ? 'X' : 'O');\

    Board b;
    b.draw_board(board);

    close(network_socket);
}