#include "dac.h"
#include"stm32f10x.h"


void GPIO_init(void)
{
	RCC_APB2PeriphClockCmd(Pin_RCC,ENABLE);
	GPIO_InitTypeDef GPIO_InitStruct;//声明一个管脚初始化的结构体
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_1|GPIO_Pin_11|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStruct);
	
	GPIO_SetBits(GPIOB,GPIO_Pin_1);
	GPIO_ResetBits(GPIOB,GPIO_Pin_11);
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}

void DA_OUTPUT(uint16_t value)
{
	uint8_t i;
	value<<=6;
	CS_0;CLK_0;
	for(i = 0; i < 12; i++)
	{
		if(value&0x8000)
		{
			DIN_1;
		}
		else
		{
			DIN_0;
		}
		CLK_1;
		value<<=1;
		CLK_0;
	}
	CS_1;CLK_0;
}
