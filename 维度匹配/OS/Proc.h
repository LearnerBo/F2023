#ifndef PROC_H
#define PROC_H
#include "stm32f4xx.h"                  // Device header
#include "stm32f4xx_it.h"
#include "time.h"
extern uint8_t SysTick_5ms;
extern uint8_t SysTick_1s;
extern uint8_t SysTick_Signal_1s;
extern uint8_t SysTick_Location_500ms;
extern uint8_t SysTick_Loc_500ms;
extern uint8_t SysTick_Mode_Rec_100ms;
extern uint8_t SysTick_2s;
extern uint8_t SysTick_500ms;
extern uint8_t SysTick_Display_500ms;
extern uint8_t SysTick_100ms;
extern uint8_t SysTick_50ms;
extern uint8_t SysTick_25ms;
extern uint8_t SysTick_5ms;


void Per_Task(void);

#endif