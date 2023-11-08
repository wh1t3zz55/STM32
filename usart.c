#include "sys.h"
#include "usart.h"	  
#include <string.h>
#include <stdlib.h>
#include "rtc.h" 
#include "dht11.h"
#include "298n.h"
void uart_init(u32 bound){
  //GPIO�˿�����
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
   
  //USART1_RX	  GPIOA.10��ʼ��
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
  GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

  //Usart1 NVIC ����
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
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

  USART_Init(USART1, &USART_InitStructure); //��ʼ������1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����жϣ�һ���ֽ��жϣ�
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//ʹ�ܿ����жϣ�һ֡�����жϣ�
  USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 

}

/*ʵ�ַ����ַ����Ĺ���������
���η����ַ����е��ַ���ÿ����һ�������TXE��־λ���޸Ŀ⺯���е�USART_SendData()��������
������ȫ���ַ��Ժ������TC��־λ ��*/
void USART_SendByte(USART_TypeDef* USARTx, uint16_t Data)
{
  /* Check the parameters */
  assert_param(IS_USART_ALL_PERIPH(USARTx));
  assert_param(IS_USART_DATA(Data)); 
    
  /* Transmit Data */
  USARTx->DR = (Data & (uint16_t)0x01FF);
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TXE)==RESET);
}

void USART_SendString(USART_TypeDef* USARTx, char *str)
{	
	while(*str!='\0')
	{
		USART_SendByte(USARTx,*str++);
	}
	while( USART_GetFlagStatus(USARTx, USART_FLAG_TC)==RESET);
}

uint8_t USART_ReceiveByte(USART_TypeDef* USARTx)
{
	while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE)==RESET);
	return (uint8_t) USART_ReceiveData(USART1);

}

static volatile uint8_t  g_usart1_buf[128]={0};
static volatile uint32_t g_usart1_cnt=0;
void USART1_IRQHandler(void)                	//����1�жϷ������
{
	uint8_t d=0;
	int i = 0;
	u16 alarm_buf[64]={0};
	u16 rtc_buf[64]={0};
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		//���մ�������
		d = USART_ReceiveData(USART1);	
		
		g_usart1_buf[g_usart1_cnt] = d;
		
		g_usart1_cnt++;

		//��������
		if(d == 'A'|| g_usart1_cnt>= sizeof(g_usart1_buf))
		{ 		
			char *s = strtok((char *)g_usart1_buf,"- :");  //�ָ����- :
			 while(s!=NULL)
			{   
			    alarm_buf[i] = atoi(s);    //2022-1-10 23:50:5A
				i++;
				s = strtok(NULL,"- :");
				g_usart1_cnt = 0;
			}
			RTC_Alarm_Set(alarm_buf[0],alarm_buf[1],alarm_buf[2],alarm_buf[3], alarm_buf[4],alarm_buf[5]); //��������
			printf("%d-%d-%d %d:%d:%d  AlarmSet��\r\n",alarm_buf[0],alarm_buf[1],alarm_buf[2],alarm_buf[3],alarm_buf[4],alarm_buf[5]);
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			TIM_SetCompare2(TIM3,0);//PA7 zhangkongbi
		}	
		//����ʱ��
		else if(d == 'R'|| g_usart1_cnt>= sizeof(g_usart1_buf))
		{ 		
			char *s = strtok((char *)g_usart1_buf,"- :");  //�ָ����- :
			 while(s!=NULL)
			{   
			    rtc_buf[i] = atoi(s);    //2022-1-10 23:50:5R
				i++;
				s = strtok(NULL,"- :");
				g_usart1_cnt = 0;
			}
			RTC_Set(rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3], rtc_buf[4],rtc_buf[5]); //����ʱ��
			
			printf("%d-%d-%d %d:%d:%d  TimeSet��\r\n",rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3],rtc_buf[4],rtc_buf[5]);
		}	
		//��մ��ڽ����жϱ�־λ
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//RTC_Init_LSE();
		//dht11_on();	
		
		
	} 
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
		{
		USART1->SR;//�ȶ�SR
		USART1->DR;//�ٶ�DR

	}

} 

/*����ѡ΢������Ҫ���*/
#pragma import(__use_no_semihosting)
struct __FILE
{
	int handle;
};

FILE __stdout;
void _sys_exit(int x)
{
	x = x;
}

///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
		/* �ȴ������������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
