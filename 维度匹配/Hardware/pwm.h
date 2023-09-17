#ifndef _PWM_H
#define _PWM_H
extern uint16_t Duty;
extern uint32_t Freq;
#include "stm32f4xx_tim.h"
void Init_PWM(u16 arr,u16 psc);
void SettingPWM(u16 pwmduty);
void SettingPWM_Frequency(u32 frequency);
#endif
