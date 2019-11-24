#include <iostream>
#include <iostream>

#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

int connect_client_socket(int port_number)
{
    // Create network socket
    int network_socket;

    /* AF_INET = IPv4 Internet protocols 
       SOCK_STREAM = TCP SOCKET
       SOCKET_PROTOCOL set to 0 == default TCP */

    network_socket = socket(AF_INET, SOCK_STREAM, 0);

    // specify address for the socket
    struct sockaddr_in server_address;

    /* set up the server info */
    server_address.sin_family = AF_INET;	
    //server_address.sin_port = htons(atoi(argv[1]));
    server_address.sin_port = htons(port_number);	
    // connects to IP address 0000
    server_address.sin_addr.s_addr=INADDR_ANY;

    int connection_status = connect(network_socket, (struct sockaddr *) &server_address, sizeof(server_address));	

    if (connection_status == -1) {
        printf("There was an error connecting to the remote socket.\n\n");
    }
    return network_socket;

}
int main(int argc, char *argv[]){

    int network_socket = connect_client_socket(9002);

    // receive data from th server
    char server_response[256];
    recv(network_socket, &server_response, sizeof(server_response),0);

    // print server response
    printf("the server sent the data %s\n", server_response); 

    //close(network_socket);
}