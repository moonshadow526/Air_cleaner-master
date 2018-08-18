#ifndef __TIMER4_CAP_H
#define __TIMER4_CAP_H
#include "sys.h"


extern u32 tempup1;	//捕获总高电平的时间
extern u32 tempup2;	//捕获总高电平的时间
extern u32 tempup3;	//捕获总高电平的时间
extern u32 tempup4;	//捕获总高电平的时间

void TIM3_Cap_Init(u16 arr, u16 psc);
#endif
