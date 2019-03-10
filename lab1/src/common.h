#ifndef COMMON
#define COMMON

#define MAX_MSG_LEN 4096
#define MAX_MSG_TEXT_LEN 4000

int main_socket;

char* user_ID;
char* IP;
int port;
char* next_client_IP;
int next_client_port;
_Bool token_holding_flag;
char* protocol;

#endif