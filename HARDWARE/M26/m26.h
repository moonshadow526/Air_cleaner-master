#ifndef _M26_H_
#define _M26_H_
#include "includes.h"
#define M26_POWERKEY PBout(14)


void get_url(void);
void m26_resest(void);
void m26_call_test(void);
void m26_ring_test(void);
void m26_build_tcp_connect(void);
uint8_t  m26_send_cmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
void m26_send_msg(char *encodeText);
void m26_Powerkey_Init(void);

#endif


