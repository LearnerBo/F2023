#ifndef _FFT_H
#define _FFT_H
#include "stm32f4xx.h"                  // Device header
#include "arm_math.h"
#include "uart.h"
#include "main.h"
#include "fft.h"
#include "ADC.h"
#include "AD7606.h"
#include "Task.h"
#include "Kalman.h"
#define FFT_LENGTH		2048		//FFT长度，默认是1024点FFT

extern float FFT_InputBuf[FFT_LENGTH*2];	//FFT输入数组
extern float FFT_OutputBuf[FFT_LENGTH];	//FFT输出数组
extern float input[2*FFT_LENGTH];
extern float32_t FFT_phase_Pi;
extern float32_t 	FFT_phaseDifference;
extern float32_t FFT_phaseDifference_AB;
extern float32_t FFT_frequency_Chanel_0;
extern float32_t FFT_frequency_Chanel_1;
extern float32_t FFT_frequency_Chanel_2;
extern float32_t FFT_frequency_Chanel_3;
extern float32_t FFT_frequency_Chanel_4;
extern float32_t FFT_frequency_Chanel_5;
extern float32_t FFT_frequency_Chanel_6;
extern float32_t FFT_frequency_Chanel_7;
void Init_FFT(u16 FFT_Length);
void Generate_InputArray(float input[],u16 Length);
void FFT_GetValue(float input[],float output[], u16 Length);
u16 Max(float input[],u16 Length);
void test_fft(void);
// void Data_Init(void);
void arm_rfft_f32_app(void);
// void DATA_Procesess(void);  //数据处理的样例

void FFT_Phase_Computation(uint16_t SignalFreq,uint16_t SAMPLE_Rate);

void FFT_Freq_Computation(uint8_t AD7606_Chanle, uint16_t SAPLE_Rate);
void FFT_Phase_Computation_Filter(uint16_t SignalFreq, uint16_t SAMPLE_Rate);

#endif
