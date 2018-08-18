#include "m26.h"
#include "usart.h"
#include "usart3.h"
#include "delay.h"
#include "status_manager.h"
#include <string.h>
#include "timer.h"

//extern u32 ChipUniqueID[3];
//extern char deviceid[12];
//char expressText[200];  //��Ŵ����ܵ��������ݣ����建���С�����û����������ݳ����Լ������޸�
//char cipherText[400];//����Ѽ��ܵ��������ݣ����建���С�����û����ܺ�����ݳ����Լ������޸�
extern u8 datatemp[] ;


void m26_Powerkey_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;				 //LED0-->PB14 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_ResetBits(GPIOB,GPIO_Pin_14);						 //PB14 ���1

}

uint8_t M26_State = 0;


/*-------------------------------------------------------------------*/
/* �ڲ�����ԭ�� -----------------------------------------------------*/
void 		m26_poweron(void);
uint8_t  	m26_send_cmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime);
uint8_t* 	m26_check_cmd(uint8_t *str);
/*-------------------------------------------------------------------*/
/*-------------------------------------------------------------------*/


/**
  * @brief	M26 TCP���Ӳ���
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
/* �ڲ�����ʵ�� -----------------------------------------------------*/

/**
  * @brief	M26 ģ�鿪��
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
	
	/* �ϵ�ȴ�ģ�������� */
	delay_s(10);
}

/**
  * @brief	�� M26 ģ�鷢������
  * @param	cmd			���͵�����
  * @param	ack			�ڴ���Ӧ����
  * @param	waittime 	�ȴ�ʱ��(��λ:10ms)
  * @retval	0 ���ͳɹ�
			1 ����ʧ��
  */
uint8_t  m26_send_cmd(uint8_t *cmd, uint8_t *ack, uint16_t waittime)
{
	uint8_t res = 0;
	
	/* ������һ������֮ǰ�������״̬ */
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
			
			/* ���յ�һ֡���� */
			if (USART3_RX_STA & 0x8000)
			{
				/* ���յ��ڴ���Ӧ���� */
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
				/* ���յ��ڴ���Ӧ���� */
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
  * @brief	�� M26 ģ�鷢������󣬼����յ���Ӧ��
  * @param	str		�ڴ���Ӧ����
  * @retval 0		û�еõ��ڴ���Ӧ����
			���� 	�ڴ�Ӧ������λ��
  */
uint8_t* m26_check_cmd(uint8_t *str)
{
	char *strx = 0;
	
	/* ���յ�һ������ */
//	if (USART3_RX_STA & 0x8000)
//	{
		/* ���ӽ����� */
		USART3_RX_BUF[USART3_RX_STA & 0x7FFF] = 0;
		strx = strstr((const char*)USART3_RX_BUF, (const char*)str);
//		printf("cmd echo data is: %x\n",strx);
//	}
	
	return (uint8_t*)strx;
}
