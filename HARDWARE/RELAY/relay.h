#ifndef _RELAY_H_
#define _RELAY_H_

#include "sys.h"

#define RELAYOUT PCout(7)// PC7
#define RELAYCHKSTUS PCin(8)// PC8
#define RELAYLED PCout(4)// PC4
#define RELAYBEEP PAout(7)// PC4


void Relay_Init(void);
void Relay_Status_Check_Init(void);
void Relay_Led_Init(void);
void Relay_Beep_Init(void);

#endif
