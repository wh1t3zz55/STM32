#ifndef __DAC_H__
#define __DAC_H__

#include "stm32f10x.h"
#include "stdio.h"
#include "usart.h"
#include "delay.h" 
#include "led.h"
#define CS_1   GPIO_SetBits(Pin_group,CS_Pin)
#define CS_0   GPIO_ResetBits(Pin_group,CS_Pin)

#define CLK_1  GPIO_SetBits(Pin_group,CLK_Pin)
#define CLK_0	 GPIO_ResetBits(Pin_group,CLK_Pin)

#define DIN_1  GPIO_SetBits(Pin_group,DIN_Pin)
#define DIN_0	 GPIO_ResetBits(Pin_group,DIN_Pin)

/*定义使用引脚*/				 
#define   CS_Pin    GPIO_Pin_1
#define   CLK_Pin   GPIO_Pin_11		
#define   DIN_Pin   GPIO_Pin_10

/*定义引脚所在组*/
#define   Pin_group   GPIOB
										 
/*定义使用的时钟*/
#define   Pin_RCC   RCC_APB2Periph_GPIOB
									
void GPIO_init(void);
void DA_OUTPUT(uint16_t value);

#endif
 









































