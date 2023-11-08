#include "sys.h"
#include "usart.h"	  
#include <string.h>
#include <stdlib.h>
#include "rtc.h" 
#include "dht11.h"
#include "298n.h"
void uart_init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断（一个字节中断）
	USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);//使能空闲中断（一帧数据中断）
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}

/*实现发送字符串的功能描述：
依次发送字符串中的字符，每发送一个检查下TXE标志位，修改库函数中的USART_SendData()函数即可
发送完全部字符以后，最后检查TC标志位 ，*/
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
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	uint8_t d=0;
	int i = 0;
	u16 alarm_buf[64]={0};
	u16 rtc_buf[64]={0};
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		//接收串口数据
		d = USART_ReceiveData(USART1);	
		
		g_usart1_buf[g_usart1_cnt] = d;
		
		g_usart1_cnt++;

		//设置闹钟
		if(d == 'A'|| g_usart1_cnt>= sizeof(g_usart1_buf))
		{ 		
			char *s = strtok((char *)g_usart1_buf,"- :");  //分割符是- :
			 while(s!=NULL)
			{   
			    alarm_buf[i] = atoi(s);    //2022-1-10 23:50:5A
				i++;
				s = strtok(NULL,"- :");
				g_usart1_cnt = 0;
			}
			RTC_Alarm_Set(alarm_buf[0],alarm_buf[1],alarm_buf[2],alarm_buf[3], alarm_buf[4],alarm_buf[5]); //闹钟设置
			printf("%d-%d-%d %d:%d:%d  AlarmSet！\r\n",alarm_buf[0],alarm_buf[1],alarm_buf[2],alarm_buf[3],alarm_buf[4],alarm_buf[5]);
			GPIO_SetBits(GPIOC, GPIO_Pin_13);
			GPIO_SetBits(GPIOA, GPIO_Pin_1);
			TIM_SetCompare2(TIM3,0);//PA7 zhangkongbi
		}	
		//设置时间
		else if(d == 'R'|| g_usart1_cnt>= sizeof(g_usart1_buf))
		{ 		
			char *s = strtok((char *)g_usart1_buf,"- :");  //分割符是- :
			 while(s!=NULL)
			{   
			    rtc_buf[i] = atoi(s);    //2022-1-10 23:50:5R
				i++;
				s = strtok(NULL,"- :");
				g_usart1_cnt = 0;
			}
			RTC_Set(rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3], rtc_buf[4],rtc_buf[5]); //设置时间
			
			printf("%d-%d-%d %d:%d:%d  TimeSet！\r\n",rtc_buf[0],rtc_buf[1],rtc_buf[2],rtc_buf[3],rtc_buf[4],rtc_buf[5]);
		}	
		//清空串口接收中断标志位
		
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
		//RTC_Init_LSE();
		//dht11_on();	
		
		
	} 
	else if(USART_GetITStatus(USART1,USART_IT_IDLE) != RESET)
		{
		USART1->SR;//先读SR
		USART1->DR;//再读DR

	}

} 

/*不勾选微库则需要这个*/
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

///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
		/* 发送一个字节数据到串口 */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* 等待发送完毕 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
		return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
		/* 等待串口输入数据 */
		while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);

		return (int)USART_ReceiveData(USART1);
}
