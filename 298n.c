#include "298n.h"
#include "stm32f10x.h"                  // Device header
#include "stm32f10x_tim.h"              // Keil::Device:StdPeriph Drivers:TIM

void RCC_Configuration(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2|RCC_APB1Periph_TIM3,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO|RCC_APB2Periph_GPIOA,ENABLE);
}

void GPIO_Configuration()
{

	GPIO_InitTypeDef GPIOA_InitStructure;
	GPIOA_InitStructure.GPIO_Pin = GPIO_Pin_7;
	GPIOA_InitStructure.GPIO_Mode= GPIO_Mode_AF_PP;
	GPIOA_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIOA_InitStructure);
	}
void TIM_Configuration()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	TIM_TimeBaseStructure.TIM_Period =99;
	TIM_TimeBaseStructure.TIM_Prescaler =0;
	TIM_TimeBaseStructure.TIM_ClockDivision=0;
	TIM_TimeBaseStructure.TIM_CounterMode= TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
}

void PWM_Configuration()
{	TIM_OCInitTypeDef TIM_OCInitStructure;
    
    uint16_t CCR2_Val=99;//兰有问题
   
    //TIM_OCStructInit(&TIM_OCInitStructure);//给结构体赋一个初始值
    TIM_OCInitStructure.TIM_OCMode=TIM_OCMode_PWM1;//配置输出比较模式
    TIM_OCInitStructure.TIM_OCPolarity=TIM_OCPolarity_High;//配置输出比较极性，高极性；REF波形直接输出
    
    TIM_OCInitStructure.TIM_OutputState=TIM_OutputState_Enable;//输出使能	
    TIM_OCInitStructure.TIM_Pulse=CCR2_Val;//设置CCR捕获比较寄存器的值
    TIM_OC2Init(TIM3,&TIM_OCInitStructure);
    TIM_OC2PolarityConfig(TIM3,TIM_OCPreload_Enable);//
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    TIM_Cmd(TIM3,ENABLE);
	 
    
	
}

//int main(void)
//{
//	RCC_Configuration();
//	GPIO_Configuration();
//	TIM_Configuration();
//	PWM_Configuration();
//	//while(1){
//	TIM_SetCompare2(TIM3,60);//PA7 zhangkongbi
//	
//}
