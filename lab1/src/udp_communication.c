#include "udp_communication.h"

void init_main_socket(){
    struct sockaddr_in incoming_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(port)
    };
    if(inet_pton(AF_INET, IP, & incoming_addr.sin_addr) <= 0 ){
        perror("inet_pton() ERROR");
        exit(1);
    }
    socklen_t len = sizeof(incoming_addr);
    
    main_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if(main_socket < 0){
        perror("socket() ERROR");
        exit(2);
    }
    
    if(bind(main_socket,(struct sockaddr *) & incoming_addr, len) < 0 ){
        perror("bind() ERROR");
        exit(3);
    }
}

void receive_message(char* buffer){
    printf("Waiting for token...\n");

    memset(buffer, '\0', MAX_MSG_LEN);
    if(recv(main_socket, buffer, MAX_MSG_LEN, 0) < 0){
        perror("recvfrom() ERROR");
        exit(5);
    }
}

void send_message(char* buffer){
    struct sockaddr_in next_client_addr = {
        .sin_family = AF_INET,
        .sin_port = htons(next_client_port)
    };
    if(inet_pton(AF_INET, next_client_IP, & next_client_addr.sin_addr) <= 0){
        perror("inet_pton() ERROR");
        exit(1);
    }
    socklen_t len = sizeof(next_client_addr);

    if(sendto(main_socket, buffer, MAX_MSG_LEN, 0, (struct sockaddr*) & next_client_addr, len) <= 0 ){
        perror("sendto() ERROR");
        exit(6);
    }
}