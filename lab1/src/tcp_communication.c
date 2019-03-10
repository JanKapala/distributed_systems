#include "tcp_communication.h"

void init_main_socket_tcp(){
    struct sockaddr_in incoming_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    if(inet_pton(AF_INET, IP, & incoming_addr.sin_addr) <= 0){
        perror("inet_pton() ERROR");
        exit(1);
    }
    socklen_t len = sizeof(incoming_addr);
    
    main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(main_socket < 0){
        perror("socket() ERROR");
        exit(1);
    }
    
    if(bind(main_socket,(struct sockaddr *) & incoming_addr, len) < 0 ){
        perror("bind() ERROR");
        exit(1);
    }
    
    if(listen(main_socket, MAX_CONNECTION) < 0){
        perror("listen() ERROR");
        exit(1);
    }
}

void receive_message_tcp(char* buffer){
    printf("Waiting for token...\n");
    const int incoming_socket = accept(main_socket, NULL, NULL);
    if(incoming_socket < 0){
        perror("accept() ERROR");
    }

    memset(buffer, '\0', MAX_MSG_LEN);
    if(recv(incoming_socket, buffer, MAX_MSG_LEN, 0) <= 0){
        perror("recv() ERROR");
        exit(1);
    }

    shutdown(incoming_socket, SHUT_RDWR);
}

void send_message_tcp(char* buffer){
    struct sockaddr_in next_client_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(next_client_port)
    };
    if(inet_pton(AF_INET, next_client_IP, & next_client_addr.sin_addr) <= 0 ){
        perror("inet_pton() ERROR");
        exit(1);
    }
    socklen_t len = sizeof(next_client_addr);

    const int outgoing_socket = socket(AF_INET, SOCK_STREAM, 0);
    if(outgoing_socket < 0){
        perror("socket() ERROR");
        exit(1);
    }

    if(connect(outgoing_socket, (const struct sockaddr*) & next_client_addr, len)){
        perror("connect() ERROR");
        exit(1);
    }

    if(send(outgoing_socket, buffer, MAX_MSG_LEN, 0) <= 0){
        perror("send() ERROR");
        exit(1);
    }

    shutdown(outgoing_socket, SHUT_RDWR);
}