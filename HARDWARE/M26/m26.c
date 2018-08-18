#include "m26.h"
#include "usart.h"
#include "usart3.h"
#include "delay.h"
#include "status_manager.h"
#include <string.h>
#include "timer.h"

//extern u32 ChipUniqueID[3];
//extern char deviceid[12];
//char expressText[200];  //存放待加密的明文数据，具体缓存大小根据用户待加密数据长度自己任意修改
//char cipherText[400];//存放已加密的密文数据，具体缓存大小根据用户解密后的数据长度自己任意修改
extern u8 datatemp[] ;


void m26_Powerkey_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //LED0-->PB14 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_14);						 //PB14 输出1

}

uint8_t M26_State = 0;


/*-------------------------------------------------------------------*/
/* 内部函数原型 -----------------------------------------------------*/
void 		m26_poweron(void);
uint8_t  	m26_send_cmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
uint8_t* 	m26_check_cmd(uint8_t *str);
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/


/**
  * @brief	M26 TCP连接测试
  */
void m26_build_tcp_connect(void)
{
	int i = 0,j = 0;
//	printf("read flash data is: %10s\n",datatemp);
		m26_send_cmd("AT\r\n", "OK", 35);
		m26_send_cmd("AT+CPIN?\r\n", "OK", 35);
		m26_send_cmd("AT+CREG?\r\n", "OK", 35);
		m26_send_cmd("AT+CGATT?\r\n", "OK", 35);
		m26_send_cmd("AT+QIFGCNT=0\r\n", "OK", 35);
		printf("recive CMD_AT data is: %s \n",USART3_RX_BUF);
//			m26_send_cmd("AT+QICSGP=1,\"FXTX.ZB.SD\"\r\n", "OK", 200);
//			m26_send_cmd("AT+QIMUX=0\r\n", "OK", 200);
//			m26_send_cmd("AT+QIMODE=0\r\n", "OK", 200);
		m26_send_cmd("AT+QIDNSIP=1\r\n", "OK", 35);
//			m26_send_cmd("AT+QINDI=1\r\n", "OK", 35);
		printf("recive CMD_AT data is: %s \n",USART3_RX_BUF);
//			m26_send_cmd("AT+QIREGAPP\r\n", "OK", 200);
//			m26_send_cmd("AT+QIACT\r\n", "OK", 200);
//			m26_send_cmd("AT+QILOCIP\r\n", "OK", 200);
//			m26_send_cmd("AT+QICLOSE\r\n", "CLOSE OK", 200);
			for(i=0; i<3; i++)
			{
//				m26_send_cmd("AT+QIOPEN=\"TCP\",\"120.78.65.151\",\"8082\"\r\n", "CONNECT OK", 300);
//				m26_send_cmd(datatemp, "CONNECT OK", 300);
				printf("recive tcp data is: %s \n",USART3_RX_BUF);
//				m26_send_cmd("AT+QIOPEN=\"TCP\",\"b2115902m0.51mypc.cn\",\"57380\"\r\n", "CONNECT OK", 500);
				m26_send_cmd("AT+QIOPEN=\"TCP\",\"kdk57.top\",\"8082\"\r\n", "CONNECT OK", 500);
				if(strstr((const char*)USART3_RX_BUF, "OK"))
				{
					printf("TCP status judge\n");
					for(j = 0 ;j<25; j++)
					{
						USART3_RX_STA = 0;
						if(strstr((const char*)USART3_RX_BUF, "CONNECT OK"))
						{
							printf("connect ok!!!/n");
							return;
						}
						delay_ms(400);
					}
				}
//				ret = m26_send_cmd("AT+QIOPEN=\"TCP\",\"119.90.52.85\",\"12345\"\r\n", "CONNECT OK", 300);
				printf("recive CMD data is: %s \n",USART3_RX_BUF);
				
				if(strstr((const char*)USART3_RX_BUF, "ALREADY CONNECT"))
				{
					m26_send_cmd("AT+QICLOSE\r\n", "CLOSE OK", 200);
				}
				
				else if(strstr((const char*)USART3_RX_BUF, "ERROR"))
				{
					m26_send_cmd("AT+QIMUX=0\r\n", "OK", 35);
					if(!(m26_send_cmd("AT+QISATA=0\r\n", "TCP CONNECTING", 35)))
						m26_send_cmd("AT+QICLOSE\r\n", "CLOSE OK", 35);
					else
					{
						set_status_flag(POWERON);
						return;
					}
					
				}				
				else if(strstr((const char*)USART3_RX_BUF, "CONNECT FAIL"))
				{
					set_status_flag(POWERON);
					return;
				}
				else 
				{
					set_status_flag(POWERON);
				}
			}
			printf("break ok!!!/n");
}

void m26_send_msg(char *encodeText)
{
	int i = 0;
	m26_send_cmd("AT+QISEND\r\n", ">", 35);
	
	if(!(m26_send_cmd((char *)encodeText, "SEND OK", 300)))
	{
		printf("cmd echo data is: %s\n",USART3_RX_BUF);
	}
	else
	{
		printf("=====send error ==== \n");
		set_status_flag(POWERON);
	}
	if(!(m26_send_cmd("AT+CSQ\r\n", "99", 35)))
	{
		printf("=====no signal======\n");
		set_status_flag(POWERON);
	}
	printf("cmd echo data is: %s\n",USART3_RX_BUF);

//	m26_send_cmd("{\"uuid\":\"12345\"}", "SEND OK", 35);
}

/*-------------------------------------------------------------------*/
/* 内部函数实现 -----------------------------------------------------*/

/**
  * @brief	M26 模块开机
  */
void m26_resest(void)
{
	if (0 == (m26_send_cmd("AT\r\n", "OK", 200)))
	{
		M26_POWERKEY = 0;
		delay_ms(800);
		M26_POWERKEY = 1;
		delay_s(12);
		M26_POWERKEY = 0;
	}
	M26_POWERKEY = 1;
	delay_ms(1000);
	M26_POWERKEY = 0;
	
	/* 上电等待模块自启动 */
	delay_s(10);
}

/**
  * @brief	向 M26 模块发送命令
  * @param	cmd			发送的命令
  * @param	ack			期待的应答结果
  * @param	waittime 	等待时间(单位:10ms)
  * @retval	0 发送成功
			1 发送失败
  */
uint8_t  m26_send_cmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime)
{
	uint8_t res = 0;
	
	/* 发送下一条命令之前，清接收状态 */
	USART3_RX_STA = 0; 
	if((strstr((const char*)cmd,"AT+")))
		usart3_send("%s",cmd);
	else if((strstr((const char*)cmd,"+++")))
		usart3_send("%s",cmd);
	else
	{
		usart3_send("%s",cmd);
		usart3_send("%c",0x1A);
	}
	if (ack && waittime)
	{	
		waittime= waittime*2;
		while(--waittime)
		{
	//		printf("wait %d\n",waittime);
			if (USART3_RX_STA!=0 && (USART3_RX_STA & 0x8000)!=1)
				TIM_SetCounter(TIM4,0);
			delay_ms(5);
			
			/* 接收到一帧数据 */
			if (USART3_RX_STA & 0x8000)
			{
				/* 接收到期待的应答结果 */
				if (m26_check_cmd(ack)) 
				{
					return 0;
				}
					
				else
				{
					USART3_RX_STA = 0;
				}
			}
			else
			{
				/* 接收到期待的应答结果 */
				if (m26_check_cmd(ack)) 
				{
					return 0;
				}
					
			}
		}
		
		if (waittime == 0)
			res = 1;
	}
	
	return res;
}

/**
  * @brief	向 M26 模块发送命令后，检测接收到的应答
  * @param	str		期待的应答结果
  * @retval 0		没有得到期待的应答结果
			其他 	期待应答结果的位置
  */
uint8_t* m26_check_cmd(uint8_t *str)
{
	char *strx = 0;
	
	/* 接收到一次数据 */
//	if (USART3_RX_STA & 0x8000)
//	{
		/* 添加结束符 */
		USART3_RX_BUF[USART3_RX_STA & 0x7FFF] = 0;
		strx = strstr((const char*)USART3_RX_BUF, (const char*)str);
//		printf("cmd echo data is: %x\n",strx);
//	}
	
	return (uint8_t*)strx;
}

