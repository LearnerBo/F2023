#include "hcsr04.h"


#define HCSR04_PORT     GPIOB
#define HCSR04_CLK      RCC_AHB1Periph_GPIOB
#define HCSR04_TRIG     GPIO_Pin_6
#define HCSR04_ECHO     GPIO_Pin_7

uint16_t msHcCount = 0;

void hcsr04_NVIC()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
			
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;             
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;         
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;       
	NVIC_Init(&NVIC_InitStructure);
}

void HC_SR04_Init(void)
{  
   TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;   
   GPIO_InitTypeDef GPIO_InitStructure;
   RCC_AHB1PeriphClockCmd(HCSR04_CLK, ENABLE);
  
   GPIO_InitStructure.GPIO_Pin =HCSR04_TRIG;      
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
   GPIO_InitStructure.GPIO_OType= GPIO_OType_PP;
   GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);
   GPIO_ResetBits(HCSR04_PORT,HCSR04_TRIG);
    
   GPIO_InitStructure.GPIO_Pin =   HCSR04_ECHO;     
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
   GPIO_Init(HCSR04_PORT, &GPIO_InitStructure);  
   GPIO_ResetBits(HCSR04_PORT,HCSR04_ECHO);    
         
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);   
    
   TIM_DeInit(TIM2);
   TIM_TimeBaseStructure.TIM_Period = (1000-1); 
   TIM_TimeBaseStructure.TIM_Prescaler =(72-1); 
   TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  
   TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);          
       
   TIM_ClearFlag(TIM2, TIM_FLAG_Update);  
   TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);    
   hcsr04_NVIC();
   TIM_Cmd(TIM2,DISABLE);     
}

static void OpenTimerForHc(void)  
{
  TIM_SetCounter(TIM2,0);
  msHcCount = 0;
  TIM_Cmd(TIM2, ENABLE); 
}

static void CloseTimerForHc(void)    
{
  TIM_Cmd(TIM2, DISABLE); 
}

// void TIM2_IRQHandler(void)  
// {
//   if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  
//   {
//       TIM_ClearITPendingBit(TIM2, TIM_IT_Update ); 
//       msHcCount++;
//   }
// }

uint32_t GetEchoTimer(void)
{
  uint32_t t = 0;
  t = msHcCount*1000;
  t += TIM_GetCounter(TIM2);
  TIM2->CNT = 0;  
  delay_ms(50);//ясЁы50ms
  return t;
}

uint32_t Getlength(void )
{
  uint32_t t = 0;
  int i = 0;
  uint32_t lengthTemp = 0;
  uint32_t sum = 0;
  while(i!=5)
  {
     TRIG_Send(1);      
     delay_us(20);
     TRIG_Send(0);
     while(ECHO_Reci == 0);      
     OpenTimerForHc();        
     i = i + 1;
     while(ECHO_Reci == 1);
     CloseTimerForHc();        
     t = GetEchoTimer();        
     lengthTemp = (t/58);//cm
     sum = lengthTemp + sum ;
       
   }
   lengthTemp = sum/5;
   return lengthTemp;
}
