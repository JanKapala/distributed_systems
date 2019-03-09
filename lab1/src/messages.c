#include "messages.h"

void serialize_msg(char* buff, int tkn_flag, int src_port, char* src_ip,
                    int dest_port, char* dest_ip, char* msg_text){
    memset(buff, '\0', MAX_MSG_LEN);
    sprintf(buff, "%d %d %s %d %s %s", tkn_flag, src_port, src_ip,
            dest_port, dest_ip,msg_text);
}

void parse_msg(char* buff, int* tkn_flag, int* src_port, char* src_ip,
                int* dest_port, char* dest_ip, char* msg_text){
    sscanf(buff, "%d %d %s %d %s %[^\n]", tkn_flag, src_port, src_ip,
            dest_port, dest_ip, msg_text);
}

void print_msg(char* buff){
    int tkn_flag;
    int src_port;
    char src_ip[16]={};
    int dest_port;
    char dest_ip[16]={};
    char msg_text[MAX_MSG_TEXT_LEN]={};

    parse_msg(buff, &tkn_flag, &src_port, src_ip, &dest_port, dest_ip, msg_text);

    char token_flag_human_readable[64]={};
    if(tkn_flag == SENT_TOKEN_FLAG) strcpy(token_flag_human_readable, "SENT_TOKEN_FLAG");
    else if(tkn_flag == RECEIVED_TOKEN_FLAG) strcpy(token_flag_human_readable, "RECEIVED_TOKEN_FLAG");
    else if(tkn_flag == FREE_TOKEN_FLAG) strcpy(token_flag_human_readable, "FREE_TOKEN_FLAG");
    else if(tkn_flag == NEW_CLIENT_TOKEN_FLAG) strcpy(token_flag_human_readable, "NEW_CLIENT_TOKEN_FLAG");
    else if(tkn_flag == SETTING_TOKEN_FLAG) strcpy(token_flag_human_readable, "SETTING_TOKEN_FLAG");
    else strcpy(token_flag_human_readable, "CORRUPT_TOKEN_FLAG");

    printf("\n");
    printf("MESSAGE\n");
    printf("Token flag:     %s\n", token_flag_human_readable);
    printf("Source:         %s:%d\n", src_ip, src_port);
    printf("Destination:    %s:%d\n", dest_ip, dest_port);
    printf("Message's text: %s\n", msg_text);
    printf("\n");
}