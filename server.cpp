#include <iostream>
#include "board.h"

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

int player_count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void error(const char *msg)
{
    perror(msg);
    pthread_exit(NULL);
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

    if (argc < 2) {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    int network_socket = setup_listener(atoi(argv[1]));
    //pthread_mutex_init(&count_mutex, NULL);

    pthread_mutex_destroy(&mutexcount);
    pthread_exit(NULL); 

    close(network_socket);
}
