#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "tcp_communication.h"
#include "messages.h"

void parse_input(int argc, char* args[]){
    if(argc != 7){
        printf("Invalid number of command line arguments (should be 6)");
        exit(1);
    }

    user_ID = args[1];
    port = atoi(args[2]);
    if(port < 1024){
        printf("Invalid listening port number (should be in range [1024;65535])");
    }
    //TODO check a value of the port number 
    next_client_IP = args[3];
    //TODO check a value of the IP_address
    next_client_port = atoi(args[4]);
    if(next_client_port < 1024){
        printf("Invalid next client port number (should be in range [1024;65535])");
    }
    int raw_token_holding_flag = atoi(args[5]);
    if(raw_token_holding_flag != 0 && raw_token_holding_flag != 1){
        printf("Invalid token holding flag (should be '0' or '1')");
        exit(1);
    }
    token_holding_flag = raw_token_holding_flag;

    protocol = args[6];
    if(strcmp(protocol,"TCP") != 0 && strcmp(protocol,"UDP") != 0){
        printf("Invalid protocol (should be 'TCP' or 'UDP')");
        exit(1);
    }
}

void print_configuration_info(){
    printf("\n");
    printf("CONFIGURATION INFO\n");
    printf("User ID:             %s\n", user_ID);
    printf("Own address:         %s:%d\n", IP, port);
    printf("Next client address: %s:%d\n", next_client_IP, next_client_port);
    if(token_holding_flag == 0) printf("Token holding flag:  false\n");
    else if(token_holding_flag == 1) printf("Token holding flag:  true\n");
    else printf("Token holding flag:  corrupted\n");
    printf("Protocol:            %s\n", protocol);
    printf("\n");
}

// States machine
#define FIRST_NEW_CLIENT_STATE 10
#define ANOTHER_NEW_CLIENT_STATE 11
#define COMMUNICATION_STATE 12
#define ADDING_NEW_CLIENT_STATE 13

int main(int argc, char** args)
{
    parse_input(argc, args);
    IP = "127.0.0.1";
    init_main_socket();

    print_configuration_info();
    
    // Determine initial state
    int current_state;
    if(port == next_client_port && strcmp(IP, next_client_IP) == 0){
        current_state = FIRST_NEW_CLIENT_STATE;
    }
    else current_state = ANOTHER_NEW_CLIENT_STATE;

    // Message details
    int tkn_flag;
    int src_port;
    char src_ip[16]={};
    int dest_port;
    char dest_ip[16]={};
    char msg_text[MAX_MSG_TEXT_LEN]={};
    char buffer[MAX_MSG_LEN]={};

    // Main loop of the states transitions
    while(1){
        if(current_state == FIRST_NEW_CLIENT_STATE){
            printf("\nEnter FIRST_NEW_CLIENT_STATE\n");

            // Wait for 'NEW_CLIENT' message and skip other messages
            tkn_flag = -1;
            while(tkn_flag != NEW_CLIENT_TOKEN_FLAG){
                receive_message(buffer);
                parse_msg(buffer, &tkn_flag, &src_port, src_ip, &dest_port, dest_ip, msg_text);
            }

            current_state = ADDING_NEW_CLIENT_STATE;
        }
        else if(current_state == ADDING_NEW_CLIENT_STATE){
            printf("\nEnter ADDING_NEW_CLIENT_STATE\n");

            // Remember new next client address
            char new_next_client_ip[64] = {};
            strcpy(new_next_client_ip, src_ip);
            int new_next_client_port = src_port;

            // Make setting message for new client
            tkn_flag = SETTING_TOKEN_FLAG;
            strcpy(dest_ip, src_ip);
            dest_port = src_port;
            strcpy(src_ip, next_client_IP);
            src_port = next_client_port;
            
            serialize_msg(buffer, tkn_flag, src_port, src_ip, dest_port, dest_ip, "setting message");

            // Update next client address
            strcpy(next_client_IP, new_next_client_ip);
            next_client_port = new_next_client_port;

            printf("Token ring topology changed: next client updated\n");
            print_configuration_info();

            // Send setting message to the new client
            send_message(buffer);

            // Go to next state
            current_state = COMMUNICATION_STATE;
        }
        // Second and another new clients case
        else if(current_state == ANOTHER_NEW_CLIENT_STATE){
            printf("\nEnter ANOTHER_NEW_CLIENT_STATE\n");

            serialize_msg(buffer, NEW_CLIENT_TOKEN_FLAG, port, IP, next_client_port, next_client_IP,
                            "new client hello message");

            send_message(buffer);

            // Wait for setting message and skip other messages
            tkn_flag = -1;
            while(tkn_flag != SETTING_TOKEN_FLAG){
                receive_message(buffer);
                parse_msg(buffer, &tkn_flag, &src_port, src_ip, &dest_port, dest_ip, msg_text);
            }

            // Update next client address
            next_client_IP = src_ip;
            next_client_port = src_port;

            printf("Token ring connection established\n");

            current_state = COMMUNICATION_STATE;
        }
        else if(current_state == COMMUNICATION_STATE){
            printf("\nEnter COMMUNICATION_STATE\n");

            if(token_holding_flag == 0){
                receive_message(buffer);
                parse_msg(buffer, &tkn_flag, &src_port, src_ip, &dest_port, dest_ip, msg_text);
            }
            else tkn_flag = FREE_TOKEN_FLAG;
            

            if(tkn_flag == NEW_CLIENT_TOKEN_FLAG) current_state = ADDING_NEW_CLIENT_STATE;
            else if(tkn_flag == FREE_TOKEN_FLAG){
                printf("Free token received\n");

                printf("Type address [IP:port]:\n");
                scanf("%[^:]:%d",dest_ip, &dest_port);
                while(port == dest_port && strcmp(IP, dest_ip) == 0){
                    printf("It's an address of this client, type some other address:\n");
                    printf("Type address [IP:port]:\n");
                    scanf("%[^:]:%d",dest_ip, &dest_port);
                }
                printf("Type message text:\n");
                scanf("%s",msg_text);

                serialize_msg(buffer, SENT_TOKEN_FLAG, port, IP, dest_port, dest_ip,msg_text);
                send_message(buffer);

                token_holding_flag = 0;
            }
            else if(tkn_flag == SENT_TOKEN_FLAG){
                printf("Sent token received\n");
                if(strcmp(src_ip, IP) == 0 && src_port == port){
                    printf("There is no client who is destination of this message:\n");
                    print_msg(buffer);
                    serialize_msg(buffer, FREE_TOKEN_FLAG, src_port, src_ip, dest_port,
                                    dest_ip, msg_text);
                }
                else if(strcmp(dest_ip, IP) == 0 && dest_port == port){
                    printf("Received Message:\n");
                    print_msg(buffer);
                    memset(msg_text, '\0', MAX_MSG_TEXT_LEN);
                    serialize_msg(buffer, RECEIVED_TOKEN_FLAG, src_port, src_ip, dest_port,
                                    dest_ip, msg_text);
                }
                send_message(buffer);
            }
            else if(tkn_flag == RECEIVED_TOKEN_FLAG){
                printf("Received token received\n");
                if(strcmp(src_ip, IP) == 0 && src_port == port){
                    printf("The message has been successfully delivered:\n");
                    serialize_msg(buffer, FREE_TOKEN_FLAG, src_port, src_ip, dest_port,
                                    dest_ip, msg_text);
                }
                send_message(buffer);
            }
            else{
                perror("Received message's token flag is corrupted\n");
                exit(1);
            }
        }
    }
    
    shutdown(main_socket, SHUT_RDWR);
}

//TODO
/*
    -> ogarnac IP (pobieranie zamiast hardcode localhost)
    -> oczyszczenie kodu
    -> UDP implementation
    -> wybor implementavji komunikacji przez wskazniki
    -> prezentacja w make'u
    -> wydobyc i dac w inne miejsce ustawianie adresu nowgo klienta
*/