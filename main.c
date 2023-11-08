/************************************************************************************
2023@Wh1t3zZ
硬件：
IIC4针OLED DHT11 USBTTL串口
接线方法：
OLED:   VCC->3.3V    GND->GND    SCL->PB6    SDA->PB7
串口：  VCC->5V      GND->GND    TX->PA10    RX->PA9
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
	
	//char I[10],C[1],D[10],H[30],M[10],S[10];//温湿度数据中间存储变量
	//C[0] =':';

	uart_init(9600);//初始换串口 括号内传入波特率参数
	uart2_init(115200);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级 
	DelayInit();//初始化延迟函数
	I2C_Configuration();//初始化OLED屏幕IIC函数
	OLED_Init();//OLED屏幕初始化函数
	DHT11_Init();//温度传感器初始化函数
	LED_GPIO_Config();//C8T6最小系统板载GPIO13LED灯初始化函数
	BEEP_GPIO_Config();
	RCC_Configuration();
	GPIO_Configuration();
  TIM_Configuration();
	PWM_Configuration();
	TIM_SetCompare2(TIM3,60);//PA7 zhangkongbi
	
	OLED_Fill(0xFF);//全屏点亮
	OLED_Fill(0x00);//全屏灭
	
	OLED_ShowStr(75, 0, "-",2);	
	OLED_ShowStr(100, 0, "-",2);
	OLED_ShowCN(0, 0, 0); //日
	OLED_ShowCN(16, 0, 1); //期
	OLED_ShowCN(0, 2, 2); //	时
	OLED_ShowCN(16, 2, 3); //间
	OLED_ShowStr(60, 2, ":",2); 
	OLED_ShowStr(90, 2, ":",2); 
	RTC_Init_LSE(); //初始化RTC时钟
	weekset();
	while(1)
	{
		DA_OUTPUT(1000);
	}
	
}	

	 


	
	
	/************************
	while(1)
	{
		
		//OLED_CLS();//清屏
		//disp_string_8x16_16x16(1,1,"温度:");
		//disp_string_8x16_16x16(3,1,"湿度:");
		//DelayUs(1);
		
		//printf("111111",temperature);
		
		
		if(t%10==0)			//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值	
			sprintf(I,"%d",humidity);
			sprintf(D,"%d",temperature);
			
			OLED_ShowCN(0,6,5);//温
			OLED_ShowCN(16,6,7);//度
			OLED_ShowStr(30,6,C,2);//冒号：
      OLED_ShowStr(38,6,D,2);//温度数值
			OLED_ShowCN(54,6,8);//°
			
			OLED_ShowCN(64,6,6);//湿
			OLED_ShowCN(80,6,7);//度
			OLED_ShowStr(94,6,C,2);//冒号：
			OLED_ShowStr(102,6,I,2);//湿度数值
			OLED_ShowStr(120,6,"%",2);//湿度数值
 
			GPIO_ResetBits(GPIOC, GPIO_Pin_13);//板载LED灯亮提示
			//OLED_ShowStr(32,4,temperature,2);	
			//OLED_ShowStr(32,6,humidity,2);	
			printf("温度:%d\r\n 湿度:%d\r\n ",temperature,humidity);
			
			//DelayMs(10);    0温16 度32 ：40 数56 湿72 度
			
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
	
	
			OLED_ShowCN(0,6,5);//温
			OLED_ShowCN(16,6,7);//度
			OLED_ShowStr(32,6,C,2);//冒号：
      OLED_ShowStr(40,6,D,2);//温度数值
			
			OLED_ShowCN(60,6,6);//湿
			OLED_ShowCN(76,6,7);//度
			OLED_ShowStr(92,6,C,2);//冒号：
			OLED_ShowStr(100,6,I,2);//湿度数值
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	while(1)
	{
		OLED_Fill(0xFF);//全屏点亮
		DelayS(2);
		OLED_Fill(0x00);//全屏灭
		DelayS(2);
		for(i=0;i<5;i++)
		{
			OLED_ShowCN(22+i*16,0,i);//测试显示中文
		}
		DelayS(2);
		OLED_ShowStr(0,3,"HelTec Automation",1);//测试6*8字符
		OLED_ShowStr(0,4,"Hello Tech",2);				//测试8*16字符
		DelayS(2);
		OLED_CLS();//清屏
		OLED_OFF();//测试OLED休眠
		DelayS(2);
		OLED_ON();//测试OLED休眠后唤醒
		OLED_DrawBMP(0,0,128,8,(unsigned char *)BMP1);//测试BMP位图显示
		DelayS(2);
	}*/

