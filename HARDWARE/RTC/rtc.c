#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "rtc.h" 		    
//Mini STM32开发板
//RTC实时时钟 驱动代码			 
//正点原子@ALIENTEK
//2010/6/6
	   
uint8_t timer2off_cleaner = 0;	
volatile u8 hrbit_cnt = 0;
 
static void RTC_NVIC_Config(void)
{	
  NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = RTC_IRQn;		//RTC全局中断
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;	//先占优先级1位,从优先级3位
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;	//先占优先级0位,从优先级4位
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;		//使能该通道中断
	NVIC_Init(&NVIC_InitStructure);		//根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}

//实时时钟配置
//初始化RTC时钟,同时检测时钟是否工作正常
//BKP->DR1用于保存是否第一次配置的设置
//返回0:正常
//其他:错误代码

u8 RTC_Init(void)
{
	//检查是不是第一次配置时钟
	u8 temp=0;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	if (BKP_ReadBackupRegister(BKP_DR1) != 0x5050)		//从指定的后备寄存器中读出数据:读出了与写入的指定数据不相乎
		{	 			
		BKP_DeInit();	//复位备份区域 	
		RCC_LSEConfig(RCC_LSE_ON);	//设置外部低速晶振(LSE),使用外设低速晶振
		while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET&&temp<250)	//检查指定的RCC标志位设置与否,等待低速晶振就绪
			{
			temp++;
			delay_ms(10);
			}
		if(temp>=250)
		{
			return 1;//初始化时钟失败,晶振有问题
		}	    
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);		//设置RTC时钟(RTCCLK),选择LSE作为RTC时钟    
		RCC_RTCCLKCmd(ENABLE);	//使能RTC时钟  
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_WaitForSynchro();		//等待RTC寄存器同步  
		RTC_ITConfig(RTC_IT_SEC, ENABLE);		//使能RTC秒中断
		RTC_ITConfig(RTC_IT_ALR, ENABLE);		//使能RTC alarm
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_EnterConfigMode();/// 允许配置	
		RTC_SetPrescaler(32767); //设置RTC预分频的值
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		RTC_Set(0);  //设置时间
		RTC_ExitConfigMode(); //退出配置模式  
		BKP_WriteBackupRegister(BKP_DR1, 0X5050);	//向指定的后备寄存器中写入用户程序数据
		}
	else//系统继续计时
		{
		RTC_WaitForSynchro();	//等待最近一次对RTC寄存器的写操作完成
		RTC_ITConfig(RTC_IT_SEC, ENABLE);	//使能RTC秒中断
		RTC_ITConfig(RTC_IT_ALR, ENABLE);		//使能RTC alarm
		RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成
		}
	RTC_NVIC_Config();//RCT中断分组设置	
	
//	RTC_Get();//更新时间	
	return 0; //ok

}		 				    
//RTC时钟中断
//每秒触发一次  
//extern u16 tcnt; 
void RTC_IRQHandler(void)
{		 
	if (RTC_GetITStatus(RTC_IT_SEC) != RESET)//秒钟中断
	{		
		hrbit_cnt++;
		printf("hb: %d\n",hrbit_cnt);
 	}
	if(RTC_GetITStatus(RTC_IT_ALR)!= RESET)//闹钟中断
	{
		RTC_ClearITPendingBit(RTC_IT_ALR);		//清闹钟中断
		timer2off_cleaner = 1;
  	} 				  								 
	RTC_ClearITPendingBit(RTC_IT_SEC|RTC_IT_OW);		//清闹钟中断
	RTC_WaitForLastTask();	  	    						 	   	 
}
			   
u8 RTC_Set(u32 sec)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟  
	PWR_BackupAccessCmd(ENABLE);	//使能RTC和后备寄存器访问 
	RTC_SetCounter(sec);	//设置RTC计数器的值

	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	return 0;	    
}

//初始化闹钟		  
//返回值:0,成功;其他:错误代码.
u8 RTC_Alarm_Set(u32 sec)
{

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);	//使能PWR和BKP外设时钟   
	PWR_BackupAccessCmd(ENABLE);	//使能后备寄存器访问  
	//上面三步是必须的!
	
	RTC_SetAlarm(sec);
 
	RTC_WaitForLastTask();	//等待最近一次对RTC寄存器的写操作完成  	
	
	return 0;	    
}











