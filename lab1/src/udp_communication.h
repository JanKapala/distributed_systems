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

void init_main_socket();
void receive_message(char* buffer);
void send_message(char* buffer);

#endif