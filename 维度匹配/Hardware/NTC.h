#ifndef __NTC_H
#define __NTC_H

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

#include "ADC.h"
#include "math.h"
#include "stdlib.h"
#include <stdint.h>
#include <stdio.h>

u16 Get_ADC(u8 ADC_channel);
u32 Get_Res(u8 ADC_channel);
u8 Get_Temp(u8 ADC_channel);
double Get_Temp_Precise(u8 ADC_channel);//精确到小数点后1位
double Get_Temp_PJL( u8 ADC_channel);
#endif

