#include "dht11.h"
#include "delay.h"
#include "OLED_I2C.h"
#include <stdio.h>
//��λDHT11
void DHT11_Rst(void)	   
{                 
	DHT11_IO_OUT(); 	//SET OUTPUT
    DHT11_DQ_OUT=0; 	//����DQ
    DelayMs(20);    	//��������18ms
    DHT11_DQ_OUT=1; 	//DQ=1 
	DelayUs(30);     	//��������20~40us
}

//�ȴ�DHT11�Ļ�Ӧ
//����1:δ��⵽DHT11�Ĵ���
//����0:����
u8 DHT11_Check(void) 	   
{   
	u8 retry=0;
	DHT11_IO_IN();//SET INPUT	 {GPIOB->CRH&=0XFFFF0FFF;GPIOB->CRH|=8<<12;}
    while (DHT11_DQ_IN&&retry<100)//DHT11������40~80us
	{
		retry++;
		DelayUs(1);
	};	 
	if(retry>=100)return 1;
	else retry=0;
    while (!DHT11_DQ_IN&&retry<100)//DHT11���ͺ���ٴ�����40~80us
	{
		retry++;
		DelayUs(1);
	};
	if(retry>=100)return 1;	    
	return 0;
}

//��DHT11��ȡһ��λ
//����ֵ��1/0
u8 DHT11_Read_Bit(void) 			 
{
 	u8 retry=0;
	while(DHT11_DQ_IN&&retry<100)//�ȴ���Ϊ�͵�ƽ
	{
		retry++;
		DelayUs(1);
	}
	retry=0;
	while(!DHT11_DQ_IN&&retry<100)//�ȴ���ߵ�ƽ
	{
		retry++;
		DelayUs(1);
	}
	DelayUs(40);//�ȴ�40us
	if(DHT11_DQ_IN)return 1;
	else return 0;		   
}
//��DHT11��ȡһ���ֽ�
//����ֵ������������
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

//��DHT11��ȡһ������
//temp:�¶�ֵ(��Χ:0~50��)
//humi:ʪ��ֵ(��Χ:20%~90%)
//����ֵ��0,����;1,��ȡʧ��
u8 DHT11_Read_Data(u8 *temp,u8 *humi)    
{        
 	u8 buf[5];
	u8 i;
	DHT11_Rst();
	if(DHT11_Check()==0)
	{
		for(i=0;i<5;i++)//��ȡ40λ����
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

//��ʼ��DHT11��IO�� DQ ͬʱ���DHT11�Ĵ���
//����1:������
//����0:����    	 
u8 DHT11_Init(void)
{	 
 	GPIO_InitTypeDef  GPIO_InitStructure;
 	
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	
 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PB11�˿�����
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);				 //��ʼ��IO��
 
	GPIO_SetBits(GPIOB,GPIO_Pin_15);						 //PB11 �����
			    
	DHT11_Rst();  //��λDHT11
	return DHT11_Check();//�ȴ�DHT11�Ļ�Ӧ
} 
 void dht11_go()
 {
	 u8 t=0;			    
	u8 temperature;  	    
	u8 humidity;
	
	char I[10],C[1],D[10],H[30],M[10],S[10];//��ʪ�������м�洢����
	C[0] =':';
	 if(t%10==0)			//ÿ100ms��ȡһ��
		{									  
			DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ	
		}				   
	 	DelayMs(10);
		t++;
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
 
			//GPIO_ResetBits(GPIOC, GPIO_Pin_13);//����LED������ʾ
			printf("Temperature:%d Humidity:%d\r\n ",temperature,humidity);
		if(t==20)
		{
			t=0;
		}
 }
	
		
		
	








