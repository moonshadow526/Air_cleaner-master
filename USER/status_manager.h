#ifndef _STATUS_MANAGER_H_
#define _STATUS_MANAGER_H_

#include "includes.h"
#define POWERON 1
#define TCP_CONNECT 2
#define SEND_MSG 3
#define HEART_BIT 4

//enum Status{
//	POWERON=1, TCP_CONNECT, SEND_MSG, HEART_BIT
//};


void set_status_flag(u8 status);
int get_status_flag(void);

#endif
