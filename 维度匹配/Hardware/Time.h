#ifndef  Time_H
#define  Time_H
#include "stm32f4xx_tim.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_it.h"
#include "misc.h"
#include "stdio.h"
#include "math.h"
#include "ADC.h"
#include "Proc.h"
extern uint32_t capture1 ;
extern uint32_t capture2 ;
extern uint32_t capture3 ;
extern uint32_t capture4 ;
extern int32_t sub_capture;
extern int32_t sub_capture4_1;
extern int32_t sub_capture3_1;
extern int32_t sub_capture2_1;


extern int32_t SUB0;
extern int32_t SUB1;
extern int32_t SUB2;
extern int32_t SUB3;
extern int32_t SUB4;
extern int32_t SUB5;

extern int32_t sub_capture2_AD;//4-1
extern int32_t sub_capture2_BC;//3-2
extern uint8_t channel_ready_flags; // ����һ����־λ����ʾÿ��ͨ���Ƿ��Ѿ����յ�����
extern uint8_t data_ready_flags;
extern uint8_t phasic_conv_flag;
extern uint16_t Renew_Count;
extern uint8_t Capture_Secure_FLAG;
extern uint8_t Capture_OK_FLAG;
extern float TIM5phasic_float;//TIM5���벶����λ,С��
extern u32 TIM5phasic_U32;//TIM5���벶���źŵ�Ƶ��
extern float TIM5frequency_float;//TIM5���벶����λ
extern u32 TIM5frequency_U32;//TIM5���벶���źŵ�Ƶ��
extern uint16_t DMA_CNT;//500ms����һ������
extern uint8_t Data_Init_Finsh;
extern uint8_t Capture_Secure_FLAG1;
extern uint8_t Capture_Secure_FLAG2;
extern uint8_t Capture_Secure_FLAG3;
extern uint8_t Capture_Secure_FLAG4;
void TIM5_CH1CH2_Cap_Init(u32 arr, u16 psc);
void TIM3Freq_Init(void);
uint32_t TIM3_GetFreq(void);
void TIM7Serve_Init(void);//��ʱ��,���ڽ��̹���
void TIM7_Sets(u8 sta); //���� 0�� 1��
int32_t GetSubCapture();

#endif 
