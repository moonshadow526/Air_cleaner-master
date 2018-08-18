#include "sys.h"
#include "pwm.h"
#include "delay.h"

void breath_LED(uint16_t val,uint8_t dir)
{

}


void Tim3_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			TIM_OCInitStructure;
//	uint16_t PrescalerValue = 0;
	/* -----------------------------------------------------------------------
    TIM3 Configuration: generate 4 PWM signals with 4 different duty cycles:
    The TIM3CLK frequency is set to SystemCoreClock (Hz), to get TIM3 counter
    clock at 24 MHz the Prescaler is computed as following:
	- Prescaler = (TIM3CLK / TIM3 counter clock) - 1
    SystemCoreClock is set to 72 MHz for Low-density, Medium-density, High-density
    and Connectivity line devices and to 24 MHz for Low-Density Value line and
    Medium-Density Value line devices
	
    The TIM3 is running at 36 KHz: TIM3 Frequency = TIM3 counter clock/(ARR + 1)
	= 24 MHz / 1000 = 24 KHz
    TIM3 Channel1 duty cycle = (TIM3_CCR1/ TIM3_ARR)* 100 = 50%
    TIM3 Channel2 duty cycle = (TIM3_CCR2/ TIM3_ARR)* 100 = 37.5%
    TIM3 Channel3 duty cycle = (TIM3_CCR3/ TIM3_ARR)* 100 = 25%
    TIM3 Channel4 duty cycle = (TIM3_CCR4/ TIM3_ARR)* 100 = 12.5%
	----------------------------------------------------------------------- */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/* Compute the prescaler value */
//	PrescalerValue = (uint16_t) (SystemCoreClock / 1) - 1;   // ?¨°??¡ã??¦Ì?¨º¨¦¨¨???¨²60hz   ??o¨®¦Ì¡Â¨º?¨º¡À?¨²??¡ã¨¦
	/* Time base configuration */
	TIM_TimeBaseStructure.TIM_Period = 150;		//400HzµÄÆµÂÊ
	TIM_TimeBaseStructure.TIM_Prescaler = 6;	//
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//?¨°¨¦???¨ºy
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	
	/* PWM1 Mode configuration: Channel */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//3?¨º?????¡À¨¨?a0
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
//	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
	
//	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
//	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
//	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);   
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
}

void Init_PWM(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//¨º1?¨¹¦Ì??¨²¨®?¦Ì?¨º¡À?¨®
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB , ENABLE); 
	//¨¦¨¨??¦Ì??¨²¨º1¨®?¦Ì?¦Ì?1¨¹??
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	Tim3_init();  //timer2  initial

}

