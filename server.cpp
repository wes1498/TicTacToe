#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

#include "board.h"

int player_count = 0;
pthread_mutex_t count_mutex = PTHREAD_MUTEX_INITIALIZER;

void error(const char *msg) {
    perror(msg);
    pthread_exit(NULL);
}

int create_socket(int port_number) {
    // Create server socket
    int server_sockfd;
    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // define server address
    struct sockaddr_in server_address;

    /* set up the server info */
    server_address.sin_family = AF_INET;	
    server_address.sin_port = htons(port_number);
    server_address.sin_addr.s_addr=INADDR_ANY;

    // binds server information to IP and port
    bind(server_sockfd, (struct sockaddr *) &server_address, sizeof(server_address));

    return server_sockfd;
}
/* Writes a message to a client socket. */
void write_client_message(int client_socket, char * message) {
    write(client_socket, message, strlen(message));
}
void get_clients(int network_socket_listener, int * client_sockets) {
    int connections = 0;
    socklen_t client_socket_length;
    struct sockaddr_in server_address, client_address;

    // make sure you get 2 clients
    
    while(connections < 2)
    {
        /* Listen for clients. */
	    listen(network_socket_listener, 20 - player_count);
        
        /* Zero out memory for the client information. */
        //memset(&client_address, 0, sizeof(client_address));

        client_socket_length = sizeof(client_address);
	
	    //Accept the connection from the client. 
        client_sockets[connections] = accept(network_socket_listener, (struct sockaddr *) &client_address, &client_socket_length);
        
        /* Send the client it's ID. */
        write(client_sockets[connections], &connections, sizeof(int));
        
        /* Increment the player count. */
        pthread_mutex_lock(&count_mutex);
        player_count++;
        printf("Number of players is now %d.\n", player_count);
        pthread_mutex_unlock(&count_mutex);

        if (connections == 0) {
            // tell client server is waiting for another player
            write_client_message(client_sockets[0],"WAITING");
        }

        connections++;
    }
}

void* start_game(void *thread_data)  {

    //client file descriptor 
    int *client_sockfd = (int*)thread_data;
    char board[3][3] = { {' ', ' ', ' '},
                        {' ', ' ', ' '}, 
                        {' ', ' ', ' '} };

    Board b = Board();
    b.draw_board(board);
    

}
int main(int argc, char *argv[]){

    if (argc < 2) {
        printf("Must provide a port number \n");
        exit(-1);
    }

    int port = atoi(argv[1]);

    int server_sockfd = create_socket(port);
    pthread_mutex_init(&count_mutex, NULL);

    while(1){
        // client sockets
        if(player_count<20){
            int *client_sockfd = (int*)malloc(2*sizeof(int));
            memset(client_sockfd, 0, 2*sizeof(int));

            get_clients(server_sockfd, client_sockfd);

            // start a new thread for this game
            pthread_t thread;
            int result = pthread_create(&thread, NULL, start_game, (void *)client_sockfd);
            if (result){
                printf("Failed to create thread. error: %d\n", result);
                exit(-1);
            }
        }
    }

    pthread_mutex_destroy(&count_mutex);
    pthread_exit(NULL); 

    close(server_sockfd);
}
