#ifndef UART_H
#define UART_H
#include "stm32f4xx.h"                  // Device header
#include "stdint.h"
#include "stdio.h"
#include "cmd_process.h"
#include "cmd_queue.h"
#include "hmi_driver.h"
extern uint8_t sendingData;
extern  uint16_t cmd_id;
extern uint32_t values;//值
extern uint32_t valueh;//用于判断按键是否按下,0x101FFFC为按下,0x100FFFC为松开
extern uint8_t size;
#define QUEUE_SIZE 42

typedef struct
{
	uint8_t data[QUEUE_SIZE];
	uint16_t head;
	uint16_t tail;
	uint16_t size;
} Queue;
extern Queue recvQueue;   // Our queue instance.

void Init_Uart1(u32 baud);
void Init_Uart2(u32 baud);
void Init_Uart3(u32 baud);
uint16_t CMD_ID(uint8_t screen_id, uint8_t widget_id);
void  SendChar(unsigned char t);
#endif
