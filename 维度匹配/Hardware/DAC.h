#ifndef DAC_H
#define DAC_H
#include "stm32f4xx.h"
#include "stm32f4xx.h"
#include "stm32f4xx_dac.h"
#include "stm32f4xx_dma.h"
#include "stm32f4xx_tim.h"
#include "arm_math.h"
#include "uart.h"
#include "MATH.H"
#include "sys.h"
#define WAVEFORM_POINTS 256
#define WaveSize 42
extern u16 sinTable[WaveSize];
extern u16 cosTable[WaveSize];

void init_sine_wave(void);
void TIM6_Init(void);
void DAC_Config(void);
void DACFre_Config(uint16_t freq);

void TIM6DAC_Init(u16 arr, u16 psc);
void DAC_GPIO_Configuration(void);
void DAC_Ch1_SineWaveConfig(void);
void sin_Generation(void);
void cos_Generation(void);
void DMA1_Stream5_NVIC_Init(void);
void DAC_Signal_Init(void);


#endif
