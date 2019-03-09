#ifndef MESSAGES
#define MESSAGES

#include "string.h"
#include <stdio.h>

#include "common.h"

#define SENT_TOKEN_FLAG 0
#define RECEIVED_TOKEN_FLAG 1
#define FREE_TOKEN_FLAG 2
#define NEW_CLIENT_TOKEN_FLAG 3
#define SETTING_TOKEN_FLAG 4

void serialize_msg(char* buff, int tkn_flag, int src_port, char* src_ip,
                    int dest_port, char* dest_ip, char* msg_text);

void parse_msg(char* buff, int* tkn_flag, int* src_port, char* src_ip,
                int* dest_port, char* dest_ip, char* msg_text);

void print_msg(char* buff);

#endif