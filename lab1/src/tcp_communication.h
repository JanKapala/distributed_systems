#ifndef TCP_COMMUNICATION
#define TCP_COMMUNICATION

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "common.h"

#define MAX_CONNECTION 10

void init_main_socket_tcp();
void receive_message_tcp(char* buffer);
void send_message_tcp(char* buffer);

#endif