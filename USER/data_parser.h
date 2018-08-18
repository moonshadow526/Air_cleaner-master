#ifndef _DATA_PARSER_H_
#define _DATA_PARSER_H_

#define HEARTBIT 2
#define OPENPOWER 1
#define CLOSEPOWER 0

#include "includes.h"

extern uint8_t Usart_recive_cmpl;

uint8_t Data_Parser(void);
char* makeJson(char* orderword, int type ,char* jdevid, char* quenid ,int code, char* status, char recflg);
u8 parseJson(char * pMsg);
void send_msg_manager(u8 type);
u8*  make_msg (char* orderword,int type ,char* mdevid, char* quenid ,int code, char* status, char recflg);

#endif
