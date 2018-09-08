#include "includes.h"

extern uint8_t get_Air_adc_val_f;
extern u8 Relayout_f;
extern u32 ChipUniqueID[3];
extern volatile u8 hrbit_cnt;

//extern char expressText[200];  //存放待加密的明文数据，具体缓存大小根据用户待加密数据长度自己任意修改
//extern char cipherText[400];//存放已加密的密文数据，具体缓存大小根据用户解密后的数据长度自己任意修改
u8 datatemp[] = {"AT+QIOPEN=\"TCP\",\"000.000.000.000\",\"12345\"\r\n"};

int main(void)
{	 
//	int i;
	static char *deviceid= NULL;
	char *pconnect_msg =NULL;
	uint8_t  breath_LED_f = 0;
	uint16_t Air_adc_val;
	float Air_vol;
	int run_status;
	uint16_t val = 0;
	static uint8_t tmp = 0;
	
	delay_init();	    	 //延时函数初始化	  
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为9600
	TIM2_Int_Init(5000-1,7200-1);  //watchdog timer
	TIM5_Int_Init(1000-1,7200-1);  //heartbeat timer
	Adc_Init();
	usart3_init(115200);
	m26_Powerkey_Init();
	KEY_Init();
	EXTIX_Init();
	Relay_Init();
	Relay_Status_Check_Init();
	Relay_Led_Init();
	Relay_Beep_Init();
	
	Init_PWM();
	RTC_Init();
	Get_ChipID();
	IWDG_Init(4,625);   //1s watch dog
//	WWDG_Init(0X7F,0X5F,WWDG_Prescaler_8); 

//	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,sizeof(datatemp));
//	printf("read flash data is: %s\n",datatemp);
	if(NULL == (deviceid = (u8*)malloc(sizeof(char)*25)))
	{
		return NULL;
	}
	memset(deviceid,0,sizeof((char *)deviceid));
	sprintf(deviceid,"%x%x%x",ChipUniqueID[2],ChipUniqueID[1],ChipUniqueID[0]);
	printf("device id is: %24s\n",deviceid);
//	memset(expressText ,0 ,200);
//	memset(cipherText , 0 ,200);
//	strcpy(expressText , deviceid);
//	printf("device id is: %s\n",expressText);
//	memset(expressText ,0 ,200);
//	memset(cipherText , 0 ,400);
//	strcpy(expressText , (char *)ChipUniqueID);
//	AES_Encrypt(expressText  , strlen(expressText) , cipherText);	//use aesKey encrypt
//	
//	 printf("AES_Encrypt data is:   %s\r\n",cipherText);
//	 
//	 
//	memset(expressText ,0 ,200);
//	AES_Decrypt(cipherText , (sizeof(cipherText)) , expressText);//use aesKey decrypt
//	printf("AES_Decrypt data is:   %x\r\n",(unsigned int)expressText);	
	
//	RTC_Alarm_Set(42);	
//	STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)datatemp,sizeof(datatemp));
//	printf("read flash data is: %s\n",datatemp);
	set_status_flag(POWERON);
	while(1) 
	{	
//		KEY_Scan(0);
		run_status = get_status_flag();
		switch(run_status)
		{
			case POWERON:
			{
				m26_resest();
				set_status_flag(TCP_CONNECT);
				USART3_RX_STA = 0;
				Usart_recive_cmpl = 0;
				TIM_SetCounter(TIM4,0);
				TIM_Cmd(TIM4,DISABLE);
				break;
			}
			case TCP_CONNECT:
			{
				set_status_flag(SEND_MSG);
				m26_build_tcp_connect();
				USART3_RX_STA = 0;
				Usart_recive_cmpl = 0;
				TIM_SetCounter(TIM4,0);
				TIM_Cmd(TIM4,DISABLE);				
				break;
			}
			case SEND_MSG:
			{
				pconnect_msg = make_msg("connect",1,deviceid," ",1,"success",0);	
				m26_send_msg(pconnect_msg);
				set_status_flag(HEART_BIT);
				hrbit_cnt = 0;
				USART3_RX_STA = 0;
				Usart_recive_cmpl = 0;
				TIM_SetCounter(TIM4,0);
				TIM_Cmd(TIM4,DISABLE);
				break;
			}
			case HEART_BIT:
			{
				
				if(hrbit_cnt >= 10)
				{
					hrbit_cnt = 0;
					pconnect_msg = make_msg("health",1,deviceid," ",1,"success",0);	
					m26_send_msg(pconnect_msg);
	//				set_status_flag(HEART_BIT);
	//				memset(USART3_RX_BUF,0,USART3_MAX_RECV_LEN);
					USART3_RX_STA = 0;
					Usart_recive_cmpl = 0;
					TIM_SetCounter(TIM4,0);
					TIM_Cmd(TIM4,DISABLE);
				}
	//			m26_build_tcp_connect();
				
				break;
			}
			default:
			{
				break;
			}
		}

		Data_Parser();
//		printf("==============================main loop============================\n");

//		if (get_Air_adc_val_f)
//		{
			Air_adc_val = Get_Adc_Average(2,3);
			Air_vol = Air_adc_val*3.45/4096;
			if (Air_vol > 0.4)           //2.2v
				breath_LED_f = 1;
//		}
		//breath led control
//		if(breath_LED_f)
//		{
//			if(dir)
//				val++;
//			else
//				val--;
//			if(val > 100) dir=0;
//			if(val == 0 ) dir=1;
//			TIM3->CCR3 = val;
//			delay_ms(5);
//		}
		if(breath_LED_f)
		{
//			if(dir)
//				val++;
//			else
//				val--;
//			if(val > 100) dir=0;
//			if(val == 0 ) dir=1;
//			TIM3->CCR3 = val;
//			delay_ms(5);
			if(hrbit_cnt%2 > 0)
				TIM3->CCR3 = 150;
			else 
				TIM3->CCR3 = 0;
			
		}	
			
		else
		{
			TIM3->CCR3 = 150;
		}
		breath_LED_f = 0;
		
		//relay led control
		if(0 == RELAYCHKSTUS)
		{
			RELAYLED = 1;

		}
		else
		{
			RELAYLED = 0;
		}
		
	}
	free(deviceid);
}

void brink_led()
{
	
}