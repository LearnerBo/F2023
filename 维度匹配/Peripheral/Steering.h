#ifndef STEERING_H 
#define STEERING_H
#include "stm32f4xx.h"
#include "stm32f4xx_it.h"
#include "arm_math.h"
void Tim4_Steering_Init(u16 arr,u16 psc);
void SettingTim4PWM(u16 pwmduty);
void SettingTim4_Frequency(u32 frequency);
void Steering_Turn(float32_t angle);
void Open_Laser(void);
void Close_Laser(void);
void Init_Laser(void);

#endif 