#include "stm32f10x.h"
#include "motor.h"
#include "stm32f10x_tim.h"

int16_t MOTOR1,MOTOR2,MOTOR3,MOTOR4;

void Moto_PwmRflash(int16_t MOTO1_PWM,int16_t MOTO2_PWM,int16_t MOTO3_PWM,int16_t MOTO4_PWM)
{		
	if(MOTO1_PWM>Moto_PwmMax)	MOTO1_PWM = Moto_PwmMax;
	if(MOTO2_PWM>Moto_PwmMax)	MOTO2_PWM = Moto_PwmMax;
	if(MOTO3_PWM>Moto_PwmMax)	MOTO3_PWM = Moto_PwmMax;
	if(MOTO4_PWM>Moto_PwmMax)	MOTO4_PWM = Moto_PwmMax;
	if(MOTO1_PWM<0)	MOTO1_PWM = 0;
	if(MOTO2_PWM<0)	MOTO2_PWM = 0;
	if(MOTO3_PWM<0)	MOTO3_PWM = 0;
	if(MOTO4_PWM<0)	MOTO4_PWM = 0;
	
	TIM2->CCR1 = MOTO1_PWM; //+ 1000;    // PA0 PA1 PA2 PA3
	TIM2->CCR2 = MOTO2_PWM; //+ 1000;
	TIM2->CCR3 = MOTO3_PWM; //+ 1000;
	TIM2->CCR4 = MOTO4_PWM; //+ 1000;
}

void Tim2_init(void)
{
	TIM_TimeBaseInitTypeDef		TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  			  TIM_OCInitStructure;
	GPIO_InitTypeDef          GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	TIM_TimeBaseStructure.TIM_Period = 499;		//2KHz的频率
	TIM_TimeBaseStructure.TIM_Prescaler = 71;	//72MHz时钟频率，现72分频
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;//
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
	
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_Cmd(TIM2, ENABLE);
}
/*
 * 函数名：Init_Motor
 * 描述  ：初始化电机控制
 * 输入  ：无
 * 输出  ：无	
 */
void Init_Motor(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOE, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOE, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOE,GPIO_Pin_3|GPIO_Pin_4);						 //PB.5 输出高

 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5|GPIO_Pin_6;	    		 //LED1-->PE.5 端口配置, 推挽输出
 GPIO_Init(GPIOE, &GPIO_InitStructure);	  				 //推挽输出 ，IO口速度为50MHz
 GPIO_SetBits(GPIOE,GPIO_Pin_5|GPIO_Pin_6); 						 //PE.5 输出高

	//GPIO_InitTypeDef GPIO_InitStructure;
	//ê1?üμ??úó?μ?ê±?ó
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE); 
	//éè??μ??úê1ó?μ?μ?1ü??
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 ; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP; 
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	Tim2_init();  //timer2  initial
	Moto_PwmRflash(0, 0, 0, 0);
}

