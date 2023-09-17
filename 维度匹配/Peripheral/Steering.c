#include "Steering.h"

#include "sys.h"
/*
PD13 对应Tim4的CH2
*/

/*
TIM4的计数值为1000
*/
void Tim4_Steering_Init(u16 arr,u16 psc)
{		 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);  	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); 	
	
	GPIO_PinAFConfig(GPIOD,GPIO_PinSource13,GPIO_AF_TIM4); 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;          
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;        
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;       
	GPIO_Init(GPIOD,&GPIO_InitStructure);              
	  
	TIM_TimeBaseStructure.TIM_Prescaler=psc-1; 
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
	TIM_TimeBaseStructure.TIM_Period=arr-1;   
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseStructure);
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; 
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);  

	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);  

	TIM_ARRPreloadConfig(TIM4,ENABLE);
	
	TIM_Cmd(TIM4, ENABLE);  
}  

void SettingTim4PWM(u16 pwmduty)
{
    TIM_SetCompare2(TIM4,pwmduty);
}

void SettingTim4_Frequency(u32 frequency)
{

	u32 timer_clock = 84000000; 
    u16 psc = timer_clock / (frequency*1000);  
    u16 arr = 1000;
    Tim4_Steering_Init(arr, psc);
}

void Steering_Turn(float32_t angle)
{

	float32_t angle_PWM;
	int32_t angle_PWM_Set;
	angle_PWM = 750 + (angle *50/9);
	angle_PWM_Set = round(angle_PWM);
	SettingTim4PWM(angle_PWM_Set);

}


//=============激光模块
void Open_Laser(void)
{
	PAout(7)=0;
}

void Close_Laser(void)
{
	PAout(7)=1;
}


void Init_Laser(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); 	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;      
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;       
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	
	GPIO_Init(GPIOA,&GPIO_InitStructure); 
	Close_Laser();
}




