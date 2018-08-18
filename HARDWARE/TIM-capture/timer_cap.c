#include "timer_cap.h"
#include "stm32f10x_tim.h"
//#include "delay.h"

u8 TIM3CH1_CAPTURE_STA = 0;	//ͨ��1���벶���־������λ�������־����6λ�������־		
u16 TIM3CH1_CAPTURE_UPVAL;
u16 TIM3CH1_CAPTURE_DOWNVAL;

u8 TIM3CH2_CAPTURE_STA = 0;	//ͨ��2���벶���־������λ�������־����6λ�������־		
u16 TIM3CH2_CAPTURE_UPVAL;
u16 TIM3CH2_CAPTURE_DOWNVAL;

u8 TIM3CH3_CAPTURE_STA = 0;	//ͨ��3���벶���־������λ�������־����6λ�������־		
u16 TIM3CH3_CAPTURE_UPVAL;
u16 TIM3CH3_CAPTURE_DOWNVAL;

u8 TIM3CH4_CAPTURE_STA = 0;	//ͨ��1���벶���־������λ�������־����6λ�������־		
u16 TIM3CH4_CAPTURE_UPVAL;
u16 TIM3CH4_CAPTURE_DOWNVAL;

u32 tempup1 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tempup2 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tempup3 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tempup4 = 0;	//�����ܸߵ�ƽ��ʱ��
u32 tim3_T1;
u32 tim3_T2;
u32 tim3_T3;
u32 tim3_T4;

int pwmout1, pwmout2, pwmout3, pwmout4; 				//���ռ�ձ�

//��ʱ��4ͨ��1���벶������

TIM_ICInitTypeDef TIM3_ICInitStructure;

void TIM3_Cap_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);  //ʹ��GPIOBʱ��

	
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 ;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PD12 �������� 
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOD, GPIO_Pin_12);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7| GPIO_Pin_0
			| GPIO_Pin_1;  //PB6,8,9 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PB6,7,8,9 ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_Init(GPIOB, &GPIO_InitStructure);	
	GPIO_ResetBits(GPIOA, GPIO_Pin_6 | GPIO_Pin_7);//PB6,8,9  ����
	GPIO_ResetBits(GPIOB, GPIO_Pin_0 | GPIO_Pin_1);//PB6,8,9  ����

	//��ʼ����ʱ��4 TIM3���ò���������Ҫ��ΪPWM������� 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 	//Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	//��ʼ��TIM3���벶����� ͨ��1
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_1; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��2
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��3
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_3; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//��ʼ��TIM3���벶����� ͨ��4
	TIM3_ICInitStructure.TIM_Channel = TIM_Channel_4; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	TIM3_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	TIM3_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	  //���������Ƶ,����Ƶ 
	TIM3_ICInitStructure.TIM_ICFilter = 0x00;	  //IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM3, &TIM3_ICInitStructure);

	//�жϷ����ʼ��
	 NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM3�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;  //��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);   //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 

	TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4,
			ENABLE);   //����������жϣ�����CC1IE,CC2IE,CC3IE,CC4IE�����ж�	

	TIM_Cmd(TIM3, ENABLE); 		//ʹ�ܶ�ʱ��4

}

//��ʱ��4�жϷ������,��CH1��Ϊ������ת����ͨ�� ��PB6�˿�
void TIM3_IRQHandler(void)
{
	if ((TIM3CH1_CAPTURE_STA & 0X80) == 0) 		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC1) != RESET) 		//����1���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); 		//����жϱ�־λ
			if (TIM3CH1_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM3CH1_CAPTURE_DOWNVAL = TIM_GetCapture1(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH1_CAPTURE_DOWNVAL < TIM3CH1_CAPTURE_UPVAL)
				{/* �����������ʼֵ����ĩβֵ��˵������������� */
					tim3_T1 = 65535;
				}
				else
				tim3_T1 = 0;  
				tempup1 = TIM3CH1_CAPTURE_DOWNVAL - TIM3CH1_CAPTURE_UPVAL
						+ tim3_T1;		//�õ��ܵĸߵ�ƽ��ʱ��
				//pwmout1 = tempup1;		//�ܵĸߵ�ƽ��ʱ��
				tempup1 =tempup1 *172/10000;//�������&&UltrasonicWave_Distance<85
			
				
				
				TIM3CH1_CAPTURE_STA = 0;		//�����־λ���㣬��һ������Ҫ��
				TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Rising); //����Ϊ�����ز���		  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM3CH1_CAPTURE_UPVAL = TIM_GetCapture1(TIM3);		//��ȡ����������
				TIM3CH1_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC1PolarityConfig(TIM3, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}
	//delay_ms(50);
	if ((TIM3CH2_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC2) != RESET)		//����2���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC2);		//����жϱ�־λ
			if (TIM3CH2_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM3CH2_CAPTURE_DOWNVAL = TIM_GetCapture2(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH2_CAPTURE_DOWNVAL < TIM3CH2_CAPTURE_UPVAL)
				{
					tim3_T2 = 65535;
				}
				else
					tim3_T2 = 0;
				tempup2 = TIM3CH2_CAPTURE_DOWNVAL - TIM3CH2_CAPTURE_UPVAL
						+ tim3_T2;		//�õ��ܵĸߵ�ƽ��ʱ��
				//pwmout2 = tempup2;		//�ܵĸߵ�ƽ��ʱ��
				tempup2 =tempup2 *172/10000;//�������&&UltrasonicWave_Distance<85
				TIM3CH2_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Rising); //����Ϊ�����ز���		  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM3CH2_CAPTURE_UPVAL = TIM_GetCapture2(TIM3);		//��ȡ����������
				TIM3CH2_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC2PolarityConfig(TIM3, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}

	if ((TIM3CH3_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC3) != RESET)		//����3���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC3);		//����жϱ�־λ
			if (TIM3CH3_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM3CH3_CAPTURE_DOWNVAL = TIM_GetCapture3(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH3_CAPTURE_DOWNVAL < TIM3CH3_CAPTURE_UPVAL)
				{
					tim3_T3 = 65535;
				}
				else
					tim3_T3 = 0;
				tempup3 = TIM3CH3_CAPTURE_DOWNVAL - TIM3CH3_CAPTURE_UPVAL
						+ tim3_T3;		//�õ��ܵĸߵ�ƽ��ʱ��
				pwmout3 = tempup3;		//�ܵĸߵ�ƽ��ʱ��
				TIM3CH3_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Rising); //����Ϊ�����ز���		  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM3CH3_CAPTURE_UPVAL = TIM_GetCapture3(TIM3);		//��ȡ����������
				TIM3CH3_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC3PolarityConfig(TIM3, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}

	if ((TIM3CH4_CAPTURE_STA & 0X80) == 0)		//��δ�ɹ�����	
	{
		if (TIM_GetITStatus(TIM3, TIM_IT_CC4) != RESET)		//����4���������¼�
		{
			TIM_ClearITPendingBit(TIM3, TIM_IT_CC4);		//����жϱ�־λ
			if (TIM3CH4_CAPTURE_STA & 0X40)		//����һ���½���
			{
				TIM3CH4_CAPTURE_DOWNVAL = TIM_GetCapture4(TIM3);//��¼�´�ʱ�Ķ�ʱ������ֵ
				if (TIM3CH4_CAPTURE_DOWNVAL < TIM3CH4_CAPTURE_UPVAL)
				{
					tim3_T4 = 65535;
				}
				else
					tim3_T4 = 0;
				tempup4 = TIM3CH4_CAPTURE_DOWNVAL - TIM3CH4_CAPTURE_UPVAL
						+ tim3_T4;		//�õ��ܵĸߵ�ƽ��ʱ��
				pwmout4 = tempup4;		//�ܵĸߵ�ƽ��ʱ��
				TIM3CH4_CAPTURE_STA = 0;		//�����־λ����
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Rising); //����Ϊ�����ز���		  
			}
			else //��������ʱ�䵫�����½��أ���һ�β��������أ���¼��ʱ�Ķ�ʱ������ֵ
			{
				TIM3CH4_CAPTURE_UPVAL = TIM_GetCapture4(TIM3);		//��ȡ����������
				TIM3CH4_CAPTURE_STA |= 0X40;		//����Ѳ���������
				TIM_OC4PolarityConfig(TIM3, TIM_ICPolarity_Falling);//����Ϊ�½��ز���
			}
		}
	}
}
