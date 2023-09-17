#include "stm32f4xx.h"                  // Device header


void Init_NVIC()
{
	NVIC_InitTypeDef NVICInitStructyre;
	NVICInitStructyre.NVIC_IRQChannel=TIM2_IRQn;
	NVICInitStructyre.NVIC_IRQChannelPreemptionPriority=1;//抢占优先级
	NVICInitStructyre.NVIC_IRQChannelSubPriority=1;//响应优先级
	NVICInitStructyre.NVIC_IRQChannelCmd=ENABLE;//IRQ通道使能
	NVIC_Init(&NVICInitStructyre);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
