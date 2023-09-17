#include "uart.h"

#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE
{
	int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
	x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
	while ((USART1->SR & 0X40) == 0)
		; // 循环发送,直到发送完毕
	USART1->DR = (u8)ch;
	return ch;
}
#endif
//=========
void Init_Uart1(u32 baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOInitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;
	GPIOInitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_USART1);  // PA9复用为Tx
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_USART1); // PA10复用为Rx

	USART_InitTypeDef USARTInitStruct;
	USARTInitStruct.USART_BaudRate = baud;
	USARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStruct.USART_Parity = USART_Parity_No;			// 没有校验位
	USARTInitStruct.USART_StopBits = USART_StopBits_1;		// 停止位为1位
	USARTInitStruct.USART_WordLength = USART_WordLength_8b; // 字长为8

	USART_Init(USART1, &USARTInitStruct);

	USART_Cmd(USART1, ENABLE); //

	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 使能接受非空的中断

	NVIC_InitTypeDef NVICInitStructyre;
	NVICInitStructyre.NVIC_IRQChannel = USART1_IRQn;
	NVICInitStructyre.NVIC_IRQChannelPreemptionPriority =2; // 抢占优先级2
	NVICInitStructyre.NVIC_IRQChannelSubPriority = 2;		 // 响应优先级1
	NVICInitStructyre.NVIC_IRQChannelCmd = ENABLE;			 // IRQ通道使能
	NVIC_Init(&NVICInitStructyre);
}
//串口二 TX-PA2 RX-PA3
void Init_Uart2(u32 baud)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOInitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3;
	GPIOInitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIOInitStructure);

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_USART2); // PA2复用为Tx
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_USART2); // PA3复用为Rx

	USART_InitTypeDef USARTInitStruct;
	USARTInitStruct.USART_BaudRate = baud;
	USARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStruct.USART_Parity = USART_Parity_No;			// 没有校验位
	USARTInitStruct.USART_StopBits = USART_StopBits_1;		// 停止位为1位
	USARTInitStruct.USART_WordLength = USART_WordLength_8b; // 字长为8

	USART_Init(USART2, &USARTInitStruct);

	USART_Cmd(USART2, ENABLE); //

	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE); // 使能接受非空的中断

	NVIC_InitTypeDef NVICInitStructyre;
	NVICInitStructyre.NVIC_IRQChannel = USART2_IRQn;
	NVICInitStructyre.NVIC_IRQChannelPreemptionPriority = 3; // 抢占优先级
	NVICInitStructyre.NVIC_IRQChannelSubPriority = 3;		 // 响应优先级
	NVICInitStructyre.NVIC_IRQChannelCmd = ENABLE;			 // IRQ通道使能
	NVIC_Init(&NVICInitStructyre);
}

//串口三 TX-PA10 RX-PA11
void Init_Uart3(u32 baud)
{

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);

	GPIO_InitTypeDef GPIOInitStructure;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIOInitStructure.GPIO_OType = GPIO_OType_PP;
	GPIOInitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIOInitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIOInitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIOInitStructure);

	GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_USART3);  // PB10 to USART3_Tx
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_USART3);  // PB11 to USART3_Rx

	USART_InitTypeDef USARTInitStruct;
	USARTInitStruct.USART_BaudRate = baud;
	USARTInitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USARTInitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USARTInitStruct.USART_Parity = USART_Parity_No;            // No parity
	USARTInitStruct.USART_StopBits = USART_StopBits_1;         // 1 stop bit
	USARTInitStruct.USART_WordLength = USART_WordLength_8b;    // Word length of 8 bits

	USART_Init(USART3, &USARTInitStruct);

	USART_Cmd(USART3, ENABLE); // Enable USART3

	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE); // Enable Rx buffer not empty interrupt

	NVIC_InitTypeDef NVICInitStructyre;
	NVICInitStructyre.NVIC_IRQChannel = USART3_IRQn;
	NVICInitStructyre.NVIC_IRQChannelPreemptionPriority = 3; // Preemption priority
	NVICInitStructyre.NVIC_IRQChannelSubPriority = 3;        // SubPriority
	NVICInitStructyre.NVIC_IRQChannelCmd = ENABLE;           // Enable the USART3 IRQ channel
	NVIC_Init(&NVICInitStructyre);
}

#define SCREEN_ID_SHIFT 8
uint16_t CMD_ID(uint8_t screen_id, uint8_t widget_id)
{
	return (screen_id << SCREEN_ID_SHIFT) | widget_id;
}

uint16_t cmd_id; // 画面id 控件id 判断按下了哪一个按键
uint32_t values;
uint32_t valueh;
uint8_t size;
#define MAX_BUFFER_SIZE 42
#define FRAME_SIZE 14

uint8_t recvBuffer[MAX_BUFFER_SIZE];
volatile uint16_t recvBuffer_head = 0;
volatile uint16_t recvBuffer_tail = 0;
uint8_t frame[FRAME_SIZE];
uint16_t frame_index = 0;
uint8_t sendingData = 0; // A flag indicating if we are currently sending data.

void USART1_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART1, USART_IT_RXNE) == SET)
	{
		uint8_t byte = USART_ReceiveData(USART1);
        queue_push(byte);
	}
	if (USART_GetFlagStatus(USART1, USART_IT_TC) == SET)
	{
		// Transmission complete. Clear the sendingData flag.
		sendingData = 0;
		USART_ClearFlag(USART1, USART_FLAG_TC);
	}
}

void USART2_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART2, USART_IT_RXNE) == SET)
	{
		
	}
	if (USART_GetFlagStatus(USART2, USART_IT_TC) == SET)
	{
		
		 USART_ClearFlag(USART2, USART_FLAG_TC);

	}
}

void USART3_IRQHandler(void)
{
	if (USART_GetFlagStatus(USART3, USART_IT_RXNE) == SET)
	{

	}
	if (USART_GetFlagStatus(USART3, USART_IT_TC) == SET)
	{
		 USART_ClearFlag(USART3, USART_FLAG_TC);

	}
}

void  SendChar(unsigned char t)
{
    USART_SendData(USART1,t);
    while(USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
    while((USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET));//等待串口发送完毕
}
