#include "stm32f4xx.h" // Device header
#include "sys.h"
#include "delay.h"
#include "uart.h"
#include "fft.h"
#include "kalman.h"
#include "Time.h"
#include "ADC.h" /* #define AD_Points 2046    extern uint16_t AD[AD_Points];*/
#include "Task.h"
#include "AD7606.h"
#include "byyc_gpio.h"
#include "byyc_spi.h"
#include "byyc_dma.h"
#include "stm32f4xx_it.h"
#include "Proc.h"
#include "steering.h"
#include "OS_Task.h"
#include "pwm.h"
#include "DAC.h"
#include "Excel.h"
int main()
{
	SysTick_Config(168000 / 8);
	delay_init(168);
	Init_Uart1(115200);
	TIM5_CH1CH2_Cap_Init(0xffffffff, 83); // 83
	DAC_Signal_Init();
	while (1)
	{
		Per_Task();
		modeRecTask();
		SystemTask();
		Close_InterTask();
	}
}
