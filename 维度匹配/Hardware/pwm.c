#include "sys.h"
/*
PA1对应的是TIM2的通道2(参考芯片手册引脚复用)
*/
uint16_t Duty;
uint32_t Freq;
void Init_PWM(u16 arr,u16 psc)
{		 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource6,GPIO_AF_TIM3); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       
	GPIO_Init(GPIOA,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr-1;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  

	TIM_ARRPreloadConfig(TIM3,ENABLE);
	
	TIM_Cmd(TIM3, ENABLE);  
}  

void SettingPWM(u16 pwmduty)
{
    TIM_SetCompare1(TIM3,pwmduty/10);
}

void SettingPWM_Frequency(u32 frequency)
{

	u32 timer_clock = 84000000; 
    u16 psc = timer_clock / (frequency*10);  
    u16 arr = 10;
    Init_PWM(arr, psc);
}

