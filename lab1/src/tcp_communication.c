#include "tcp_communication.h"

int init_listening_socket(int incoming_port, char* incoming_ip_address){
    struct sockaddr_in incoming_addr =
    {
        .sin_family = AF_INET,
        .sin_port = htons( incoming_port )
    };
    if( inet_pton( AF_INET, incoming_ip_address, & incoming_addr.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }
    
    const int listening_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if( listening_socket < 0 )
    {
        perror( "socket() ERROR" );
        exit( 2 );
    }
    
    socklen_t len = sizeof(incoming_addr);
    if( bind( listening_socket,( struct sockaddr * ) & incoming_addr, len ) < 0 )
    {
        perror( "bind() ERROR" );
        exit( 3 );
    }
    
    if( listen( listening_socket, MAX_CONNECTION ) < 0 )
    {
        perror( "listen() ERROR" );
        exit( 4 );
    }

    return listening_socket;
}

void receive_message(char* buffer){
    printf( "Waiting for token...\n" );
    const int incoming_socket = accept(listening_socket, NULL, NULL );
    if( incoming_socket < 0 )
    {
        perror( "accept() ERROR" );
    }

    memset(buffer, '\0', MAX_MSG_LEN);
    if( recv( incoming_socket, buffer, MAX_MSG_LEN, 0 ) <= 0 )
    {
        perror( "recv() ERROR" );
        exit( 5 );
    }
}

void send_message(char* buffer){
    struct sockaddr_in next_client =
    {
        .sin_family = AF_INET,
        .sin_port = htons( next_client_port )
    };
    if( inet_pton( AF_INET, next_client_IP, & next_client.sin_addr ) <= 0 )
    {
        perror( "inet_pton() ERROR" );
        exit( 1 );
    }

    const int outgoing_socket = socket( AF_INET, SOCK_STREAM, 0 );
    if( outgoing_socket < 0 )
    {
        perror( "socket() ERROR" );
        exit( 2 );
    }

    socklen_t len = sizeof(next_client);
    if( connect(outgoing_socket, (const struct sockaddr*) & next_client, len))
    {
        perror( "connect() ERROR" );
        exit( 4 );
    }

    if( send( outgoing_socket, buffer, strlen(buffer), 0 ) <= 0 )
    {
        perror( "send() ERROR" );
        exit( 6 );
    }

    shutdown(outgoing_socket, SHUT_RDWR);
}