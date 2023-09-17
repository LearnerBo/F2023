#ifndef OS_TASK_H
#define OS_TASK_H
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
#include "pwm.h"
#include "DAC.h"
#include "cmd_process.h"
#include "cmd_queue.h"
#include "hmi_driver.h"
#include "Excel.h"
#define THRESHOLD 100
extern uint8_t x;
extern uint8_t y;
extern uint8_t Change_Flag;
extern uint8_t Location_Ready_Flag;
extern char *xLabels[] ;
extern char *yLabels[] ;
extern uint8_t Mode;
void sweptFresignal_Task(void);
void sweptFreSinsignal_Task(void);
void display_Task(void);
void modeRecTask(void);
void soundlocationTask(void);
void Close_InterTask(void);
void Location_Task(void);
void LocationStatic_Task(void);
void LocationStaticPlus_Task(void);
void SystemTask(void);

// void Angle_Location_Task();
// void Steering_Task();



#endif 