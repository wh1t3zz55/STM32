#include "dht11.h"
#include "delay.h"
#include "OLED_I2C.h"
#include <stdio.h>
//复位DHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_DQ_OUT=0; 	//拉低DQ
    DelayMs(20);    	//拉低至少18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	DelayUs(30);     	//主机拉高20~40us
}

//等待DHT11的回应
//返回1:未检测到DHT11的存在
//返回0:存在
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
    while (DHT11_DQ_IN&&retry<100)//DHT11会拉低40~80us
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11拉低后会再次拉高40~80us
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//从DHT11读取一个位
//返回值：1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//等待变为低电平
	{
		retry++;
		DelayUs(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//等待变高电平
	{
		retry++;
		DelayUs(1);
	}
	DelayUs(40);//等待40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}
//从DHT11读取一个字节
//返回值：读到的数据
u8 DHT11_Read_Byte(void)    
{        
    u8 i,dat;
    dat=0;
	for (i=0;i<8;i++) 
	{
   		dat<<=1; 
	    dat|=DHT11_Read_Bit();
    }						    
    return dat;
}

//从DHT11读取一次数据
//temp:温度值(范围:0~50°)
//humi:湿度值(范围:20%~90%)
//返回值：0,正常;1,读取失败
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//读取40位数据
		{
			buf[i]=DHT11_Read_Byte();
		}
		if((buf[0]+buf[1]+buf[2]+buf[3])==buf[4])
		{
			*humi=buf[0];
			*temp=buf[2];
		}
	}else return 1;
	return 0;	    
}

//初始化DHT11的IO口 DQ 同时检测DHT11的存在
//返回1:不存在
//返回0:存在    	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB端口时钟
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PB11端口配置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);				 //初始化IO口
 
	GPIO_SetBits(GPIOB,GPIO_Pin_15);						 //PB11 输出高
			    
	DHT11_Rst();  //复位DHT11
	return DHT11_Check();//等待DHT11的回应
} 
 void dht11_go()
 {
	 u8 t=0;			    
	u8 temperature;  	    
	u8 humidity;
	
	char I[10],C[1],D[10],H[30],M[10],S[10];//温湿度数据中间存储变量
	C[0] =':';
	 if(t%10==0)			//每100ms读取一次
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值	
		}				   
	 	DelayMs(10);
		t++;
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
 
			//GPIO_ResetBits(GPIOC, GPIO_Pin_13);//板载LED灯亮提示
			printf("Temperature:%d Humidity:%d\r\n ",temperature,humidity);
		if(t==20)
		{
			t=0;
		}
 }
	
		
		
	








