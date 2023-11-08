#include "sys.h"
#include "usart2.h"	  
#include <string.h>
#include <stdlib.h>
#include "rtc.h" 
#include "dht11.h"
void uart2_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

  
	//USART2_TX   GPIOA.2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA.2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.2
   
  //USART2_RX	  GPIOA.3��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;//PA3
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.3  

  //USART2 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
  
   //USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

  USART_Init(USART2, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//�������ڽ����жϣ�һ���ֽ��жϣ�
	USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);//ʹ�ܿ����жϣ�һ֡�����жϣ�
  USART_Cmd(USART2, ENABLE);                    //ʹ�ܴ���1 

}

static volatile uint8_t  g_USART2_buf[128]={0};
static volatile uint32_t g_USART2_cnt=0;
void USART2_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t d=0;
	int i = 0;
	u16 alarm_buf[64]={0};
	u16 rtc_buf[64]={0};
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//���մ�������
		d = USART_ReceiveData(USART2);	
		
		g_USART2_buf[g_USART2_cnt] = d;
		
		g_USART2_cnt++;

		//��������
		
		//����ʱ��
		if(d == 'R'|| g_USART2_cnt>= sizeof(g_USART2_buf))
		{ 		
			char *s = strtok((char *)g_USART2_buf,"- :");  //�ָ����- :
			 while(s!=NULL)
			{   
			    rtc_buf[i] = atoi(s);    //2022-1-10 23:50:5R
				i++;
				s = strtok(NULL,"- :");
				g_USART2_cnt = 0;
			}
			RTC_Set(rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3], rtc_buf[4],rtc_buf[5]); //����ʱ��
			
			printf("%d-%d-%d %d:%d:%d  TimeSet��\r\n",rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3],rtc_buf[4],rtc_buf[5]);
		}	
		//��մ��ڽ����жϱ�־λ
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);
		//RTC_Init_LSE();
		//dht11_on();	
		
		
	} 
	else if(USART_GetITStatus(USART2,USART_IT_IDLE) != RESET)
		{
		USART2->SR;//�ȶ�SR
		USART2->DR;//�ٶ�DR

	}

} 

