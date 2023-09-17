#include "stm32f4xx.h"                  // Device header


void Init_NVIC()
{
	NVIC_InitTypeDef NVICInitStructyre;
	NVICInitStructyre.NVIC_IRQChannel=TIM2_IRQn;
	NVICInitStructyre.NVIC_IRQChannelPreemptionPriority=1;//��ռ���ȼ�
	NVICInitStructyre.NVIC_IRQChannelSubPriority=1;//��Ӧ���ȼ�
	NVICInitStructyre.NVIC_IRQChannelCmd=ENABLE;//IRQͨ��ʹ��
	NVIC_Init(&NVICInitStructyre);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}
