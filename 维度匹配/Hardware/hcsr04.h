#ifndef _HC_SR04_H
#define _HC_SR04_H
#include "stm32f4xx.h"
#include "delay.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
void HC_SR04_Init(void);
uint32_t Getlength(void);

#define TRIG_Send(a)   if(a)\
											 GPIO_SetBits(GPIOB,GPIO_Pin_6);\
											 else\
											 GPIO_ResetBits(GPIOB,GPIO_Pin_6)
       			
#define ECHO_Reci GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)				
 
#endif
