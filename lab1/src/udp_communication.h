#ifndef UDP_COMMUNICATION
#define UDP_COMMUNICATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

#define MAX_CONNECTION 1


int listening_socket;
int outgoing_socket;

char* user_ID;
char* IP;
int port;
char* next_client_IP;
int next_client_port;
_Bool token_holding_flag;
char* protocol;

void init_listening_socket();

void receive_message(char* buffer);

void send_message(char* buffer);

#endif