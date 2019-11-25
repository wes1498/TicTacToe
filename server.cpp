#include <iostream>
#include "board.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>


int play_game() {
    char board[3][3] = { {' ', ' ', ' '}, /* Game Board */ 
                         {' ', ' ', ' '}, 
                         {' ', ' ', ' '} };
    Board b;
    b.Draw(board);
    return 0;
}

int setup_listener(int port_number) 
{
    // Create server socket
    int network_socket;
    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // define server address
    struct sockaddr_in server_address;

    /* set up the server info */
    server_address.sin_family = AF_INET;	
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr=INADDR_ANY;

    // binds server information to IP and port
    bind(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    return network_socket;
}

int main(int argc, char *argv[]){
    //printf("Welcome to Tic Tac Toe!\n\n");
    char server_message[256] = "you have connected to the server!";
    int network_socket = setup_listener(9002);
    listen(network_socket, 5);
    int client_socket;
    client_socket = accept(network_socket, NULL, NULL);

    send(client_socket, server_message, sizeof(server_message), 0);

    close(network_socket);
}
