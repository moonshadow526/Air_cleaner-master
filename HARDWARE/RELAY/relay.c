#include "relay.h"

void Relay_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC.7 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
 GPIO_ResetBits(GPIOC,GPIO_Pin_7);						 //PB.7 输出高
					
}

void Relay_Status_Check_Init(void)
{
	
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;				 //PC.8 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; 		 	// input down
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
					
}

void Relay_Led_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //PC.7 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
 GPIO_ResetBits(GPIOC,GPIO_Pin_4);						 //PB.7 输出高
					
}

void Relay_Beep_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	 //使能PC端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PC.7 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOC
 GPIO_ResetBits(GPIOA,GPIO_Pin_7);						 //PB.7 输出高
					
}

