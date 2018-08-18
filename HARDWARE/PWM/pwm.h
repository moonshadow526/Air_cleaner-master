#ifndef		__PWM_H
#define		__PWM_H
#include "sys.h"

#define 	Moto_PwmMax 	2000
#define FR1 PEout(3)// PB5
#define FR2 PEout(4)// PB5
#define PG PEout(5)// PE5
//#define ALM PEout(6)// PE6
#define EN PEout(6)// PE6

#define BREATHLED PBout(0)

void Init_PWM(void);
void breath_LED(uint16_t val,uint8_t dir);


#endif




