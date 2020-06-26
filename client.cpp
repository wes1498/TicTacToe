#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "board.h"

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int connect_client_socket(char *host_name, int port_number)
{
    // Create network socket
    int network_socket;

    struct hostent *server;

    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (network_socket < 0)
        error("ERROR opening socket for server.");
    server = gethostbyname(host_name);

    if (server == NULL)
    {
        fprintf(stderr, "ERROR, no such host\n");
        exit(0);
    }

    struct sockaddr_in server_address;

    // Zero out memory for server info.
    memset(&server_address, 0, sizeof(server_address));

    /* Set up the server info. */
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port_number);
    memmove(server->h_addr, &server_address.sin_addr.s_addr, server->h_length);

    /* Make the connection. */
    int connection_status = connect(network_socket, (struct sockaddr *)&server_address, sizeof(server_address));

    if (connection_status == -1)
    {
        error("Error in connecting to the server.");
        //printf("There was an error connecting to the remote socket.\n\n");
    }
    return network_socket;
}

void write_server_int(int sockfd, int msg)
{
    int n = send(sockfd, &msg, sizeof(int), 0);
    if (n == -1)
    {
        error("ERROR writing int to server socket");
    }
}

void receive_message(int network_socket, char *message)
{
    memset(message, 0, 5);
    read(network_socket, message, 5);
}
int receive_int(int network_socket)
{
    int msg = 0;
    int n = read(network_socket, &msg, sizeof(int));

    if (n == -1 || n != sizeof(int))
        error("ERROR reading int from server socket");

    return msg;
}

/* Get's the players turn and sends it to the server. */
void take_turn(int sockfd)
{
    char buffer[10];

    while (1)
    { /* Ask until we receive. */
        printf("Enter 0 to 8 available moves on the board. ");
        fgets(buffer, 10, stdin);
        int move = buffer[0] - '0';
        if (move <= 8 && move >= 0)
        {
            printf("\n");
            /* Send players move to the server. */
            write_server_int(sockfd, move);
            break;
        }
        else
            printf("\nInvalid move. Please Try again.\n");
    }
}

/* Gets a board update from the server. */
void get_update(int sockfd, char board[][3])
{
    /* Get the update. */
    int player_id = receive_int(sockfd);
    int move = receive_int(sockfd);

    /* Update the game board. */
    board[move / 3][move % 3] = player_id ? 'X' : 'O';
}

int main(int argc, char *argv[])
{

    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }
    int sockfd = connect_client_socket(argv[1], atoi(argv[2]));

    // receive client id after connecting to the server
    int id = receive_int(sockfd);
    char board[3][3] = {{' ', ' ', ' '}, /* Game Board */
                        {' ', ' ', ' '},
                        {' ', ' ', ' '}};
    printf("Tic-Tac-Toe\n\n");

    printf("What team are you on? %c's\n", id ? 'X' : 'O');

    Board b = Board();
    b.draw_board(board);

    char message[6];
    do
    {
        receive_message(sockfd, message);
        if (!strcmp(message, "HOLD"))
            printf("Waiting for a second player...\n");
    } while (strcmp(message, "STRT"));

    while (1)
    {
        receive_message(sockfd, message);

        if (!strcmp(message, "MOVE"))
        {
            printf("Your move...\n");
            take_turn(sockfd);
        }
        else if (strcmp(message, "UPTD"))
        {
            // updated board
            get_update(sockfd, board);
            b.draw_board(board);
        }
        else if (strcmp(message, "INVD"))
        {
            // invalid move
            printf("Invalid move. Please try again.\n");
        }
        else if (strcmp(message, "WAIT"))
        {
            printf("Waiting for other players turn...\n");
        }
        else if (strcmp(message, "WINN"))
        {
            printf("You Win!\n");
            break;
        }
        else if (strcmp(message, "LOST"))
        {
            printf("You Lost!\n");
            break;
        }
        else if (strcmp(message, "TIEE"))
        {
            printf("A Tie!\n");
            break;
        }
        else
        {
            error("Something weird happened.");
        }
    }

    printf("Game over.\n");
    close(sockfd);
    return 0;
}