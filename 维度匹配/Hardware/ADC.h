#ifndef _ADC_H
#define _ADC_H
#include <stdint.h>
#include "stm32f4xx.h"                  // Device header
#include "delay.h"
#include "arm_math.h"
#include "uart.h"
#define AD_Points 4096
extern uint16_t AD[AD_Points];
extern float32_t AD_Value[AD_Points];
extern uint16_t AD2[AD_Points];
extern float32_t AD2_Value[AD_Points];
extern uint16_t AD3[AD_Points];
extern float32_t AD3_Value[AD_Points];
void  Adc1_Dma2_Init(void);
void  Adc2_Dma2_Init(void);
void  Adc3_Dma2_Init(void);
uint16_t Get_Adc(uint8_t ch)  ;
uint16_t Get_Adc_Average(uint8_t ch,uint8_t times);
void Data_Init(const uint16_t * Input,float32_t *Output,uint16_t size);//输入的数组 输出的数组 大小




#endif

