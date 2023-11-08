/************************************************************************************
2023@Wh1t3zZ
Ӳ����
IIC4��OLED DHT11 USBTTL����
���߷�����
OLED:   VCC->3.3V    GND->GND    SCL->PB6    SDA->PB7
���ڣ�  VCC->5V      GND->GND    TX->PA10    RX->PA9
DHT11:  VCC->3.3V    GND->GND    DATA->PB15
DAC:    CLK->PB11    DIN->PB10   CS->PB1
*************************************************************************************/
#include "stm32f10x.h"
#include "OLED_I2C.h"
#include "delay.h"
#include "dht11.h"
#include <stdio.h>
#include <string.h>
#include "sys.h"
#include "usart.h"	 
#include "led.h"
#include "rtc.h" 
#include <math.h>
#include "usart2.h"
#include "dac.h"
#include "298n.h"
extern unsigned char hanzi1[];

int main(void)
{
	
	extern const unsigned char BMP1[];
	//u8 t=0;			    
	//u8 temperature;  	    
	//u8 humidity;
	
	//char I[10],C[1],D[10],H[30],M[10],S[10];//��ʪ�������м�洢����
	//C[0] =':';

	uart_init(9600);//��ʼ������ �����ڴ��벨���ʲ���
	uart2_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ� 
	DelayInit();//��ʼ���ӳٺ���
	I2C_Configuration();//��ʼ��OLED��ĻIIC����
	OLED_Init();//OLED��Ļ��ʼ������
	DHT11_Init();//�¶ȴ�������ʼ������
	LED_GPIO_Config();//C8T6��Сϵͳ����GPIO13LED�Ƴ�ʼ������
	BEEP_GPIO_Config();
	RCC_Configuration();
	GPIO_Configuration();
  TIM_Configuration();
	PWM_Configuration();
	TIM_SetCompare2(TIM3,60);//PA7 zhangkongbi
	
	OLED_Fill(0xFF);//ȫ������
	OLED_Fill(0x00);//ȫ����
	
	OLED_ShowStr(75, 0, "-",2);	
	OLED_ShowStr(100, 0, "-",2);
	OLED_ShowCN(0, 0, 0); //��
	OLED_ShowCN(16, 0, 1); //��
	OLED_ShowCN(0, 2, 2); //	ʱ
	OLED_ShowCN(16, 2, 3); //��
	OLED_ShowStr(60, 2, ":",2); 
	OLED_ShowStr(90, 2, ":",2); 
	RTC_Init_LSE(); //��ʼ��RTCʱ��
	weekset();
	while(1)
	{
		DA_OUTPUT(1000);
	}
	
}	

	 


	
	
	/************************
	while(1)
	{
		
		//OLED_CLS();//����
		//disp_string_8x16_16x16(1,1,"�¶�:");
		//disp_string_8x16_16x16(3,1,"ʪ��:");
		//DelayUs(1);
		
		//printf("111111",temperature);
		
		
		if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ	
			sprintf(I,"%d",humidity);
			sprintf(D,"%d",temperature);
			
			OLED_ShowCN(0,6,5);//��
			OLED_ShowCN(16,6,7);//��
			OLED_ShowStr(30,6,C,2);//ð�ţ�
      OLED_ShowStr(38,6,D,2);//�¶���ֵ
			OLED_ShowCN(54,6,8);//��
			
			OLED_ShowCN(64,6,6);//ʪ
			OLED_ShowCN(80,6,7);//��
			OLED_ShowStr(94,6,C,2);//ð�ţ�
			OLED_ShowStr(102,6,I,2);//ʪ����ֵ
			OLED_ShowStr(120,6,"%",2);//ʪ����ֵ
 
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����LED������ʾ
			//OLED_ShowStr(32,4,temperature,2);	
			//OLED_ShowStr(32,6,humidity,2);	
			printf("�¶�:%d\r\n ʪ��:%d\r\n ",temperature,humidity);
			
			//DelayMs(10);    0��16 ��32 ��40 ��56 ʪ72 ��
			
		}				   
	 	DelayMs(10);
		t++;
		if(t==20)
		{
			t=0;
		}
	}
	**********************************************/
	/*
	
	
			OLED_ShowCN(0,6,5);//��
			OLED_ShowCN(16,6,7);//��
			OLED_ShowStr(32,6,C,2);//ð�ţ�
      OLED_ShowStr(40,6,D,2);//�¶���ֵ
			
			OLED_ShowCN(60,6,6);//ʪ
			OLED_ShowCN(76,6,7);//��
			OLED_ShowStr(92,6,C,2);//ð�ţ�
			OLED_ShowStr(100,6,I,2);//ʪ����ֵ
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	while(1)
	{
		OLED_Fill(0xFF);//ȫ������
		DelayS(2);
		OLED_Fill(0x00);//ȫ����
		DelayS(2);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//������ʾ����
		}
		DelayS(2);
		OLED_ShowStr(0,3,"HelTec Automation",1);//����6*8�ַ�
		OLED_ShowStr(0,4,"Hello Tech",2);				//����8*16�ַ�
		DelayS(2);
		OLED_CLS();//����
		OLED_OFF();//����OLED����
		DelayS(2);
		OLED_ON();//����OLED���ߺ���
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//����BMPλͼ��ʾ
		DelayS(2);
	}*/

