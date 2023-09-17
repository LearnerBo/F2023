#ifndef CMD_H 
#define CMD_H
#include "uart.h"
#include <string.h>
#include "stm32f4xx_usart.h"
#include "stm32f4xx_tim.h"
void send__curve_8bit(uint8_t scren_id, uint8_t widget_id, uint8_t channel, uint8_t data);
void send_string_data(uint8_t screen_id, uint8_t widget_id, char* str, uint8_t str_length);
void send_string_source_data(uint8_t scren_id,uint8_t widget_id ,uint16_t source_data);

// uint8_t data_curve_8bit[] = {0xEE, 0xB1, 0x32, 0x00, 0x02, 0x00, 0x01, 0x00, 0x00, 0x01, 0x02, 0xFF, 0xFC, 0xFF, 0xFF};//data_curve_8bit[9]是传感器读到的数据,用于绘制8bit数据的曲线
#endif 
