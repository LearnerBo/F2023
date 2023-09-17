#include "time.h"
float TIM5phasic_float;
u32 TIM5phasic_U32;
float TIM5frequency_float;
u32 TIM5frequency_U32;
// uint8_t DMA2_Stream0_Finish=0;
// uint16_t DMA_CNT=0;//500ms更新一次数据
uint32_t capture1 = 0;
uint32_t capture2 = 0;
uint32_t capture3 = 0;
uint32_t capture4 = 0;

int32_t SUB0 = 10;
int32_t SUB1 = 1000;
int32_t SUB2 = 10000;
int32_t SUB3 = 100000;
int32_t SUB4 = 1000000;
int32_t SUB5 = 10000000;


int32_t sub_capture;
int32_t sub_capture4_1;
int32_t sub_capture3_1;
int32_t sub_capture2_1;

int32_t sub_capture2_AD;
int32_t sub_capture2_BC;
uint8_t Capture_Secure_FLAG=1;
uint8_t Capture_OK_FLAG = 0;
uint8_t Cflag=0;
uint8_t channel_ready_flags = 0; // 创建一个标志位来表示每个通道是否已经接收到脉冲
uint8_t data_ready_flags = 0;

uint8_t Capture_Secure_FLAG1=1;
uint8_t Capture_Secure_FLAG2=1;
uint8_t Capture_Secure_FLAG3=1;
uint8_t Capture_Secure_FLAG4=1;

void TIM5_IRQHandler(void)
{
	if ((TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG1==1)) // 检查通道1是否触发中断
	{
		capture1 = TIM_GetCapture1(TIM5);
		Capture_Secure_FLAG1 = 0;
		

	}
	if ((TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG2==1)) // 检查通道2是否触发中断
	{
	
		capture2 = TIM_GetCapture2(TIM5); // 获取并保存通道2的捕获值  //1M的计数器,计数得到的值 psc=84-1;计数器的值为1M
		Capture_Secure_FLAG2 =0;

		
	}
	if ((TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG3==1)) // 检查通道2是否触发中断
	{
					capture3 = TIM_GetCapture3(TIM5);
					Capture_Secure_FLAG3 =0;

	}

	if ((TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG4==1)) // 检查通道2是否触发中断
	{
					capture4 = TIM_GetCapture4(TIM5);
					Capture_Secure_FLAG4 =0;

	}

	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3| TIM_IT_CC4 | TIM_IT_Update); // 清除中断标志位 在这里清除标志测量的更精准
	
}

int32_t GetSubCapture()
{ int32_t sub_capture_value;
	if(Capture_OK_FLAG == 1)
	{
	sub_capture_value = sub_capture ;
	data_ready_flags = 1;
	}
	return sub_capture_value;

}


// 定时器5通道1输入捕获配置 0xFFFFFFFF 83
// arr：自动重装值(TIM5,TIM5是32位的!!)
// psc：时钟预分频数 PA0->CHANEL1和PA1->CHANEL2
// 通道0在前时,小于180.通道1在前时,大于180
void TIM5_CH1CH2_Cap_Init(u32 arr, u16 psc)
{
	TIM_ICInitTypeDef TIM5_ICInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  // TIM5时钟使能
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能PORTA时钟

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; // GPIOA0
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // 复用功能
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // 速度100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	   // 下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   // 初始化PA0

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); // PA0复用位定时器5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); // PA1复用位定时器5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5); // PA0复用位定时器5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); // PA0复用位定时器5

	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// 定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
	TIM_TimeBaseStructure.TIM_Period = arr;						// 自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	// 初始化TIM5输入捕获参数
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;				 // CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 // 上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 // 配置输入分频,不分频
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;						 // IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	 TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	选择输入端 IC1映射到TI1上
	 TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//上升沿捕获
	 TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //映射到TI1上
	 TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //配置输入分频,不分频
	 TIM5_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 配置输入滤波器 不滤波
	 TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3;				 // CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 // 上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 // 配置输入分频,不分频
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;						 // IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_4;				 // CC1S=01 	选择输入端 IC1映射到TI1上
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 // 上升沿捕获
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // 映射到TI1上
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 // 配置输入分频,不分频
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;						 // IC1F=0000 配置输入滤波器 不滤波
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 |TIM_IT_CC3 | TIM_IT_CC4, ENABLE); // 允许更新中断 ,允许CC1IE捕获中断
	// TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2, ENABLE); // 允许更新中断 ,允许CC1IE捕获中断

	TIM_Cmd(TIM5, ENABLE); // 使能定时器5

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							  // 根据指定的参数初始化VIC寄存器、
}


void TIM3Freq_Init(void) // 引脚PA6 测量频率
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;

	/* TIM3 clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/* GPIOA clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* TIM3 channel 1 pin (PA.06) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Connect TIM pins to AF2 */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_TIM3);

	/* Time base configuration */
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 0xFFFFFFFF; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1;  // PSC  //1-1测量高频  84-1测量低频 分频系数越高,能测量的信号频率就越低 反之同理
	// 84-1 可以测量16hz
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	/* TIM3 configuration: Input Capture mode ---------------------*/
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICFilter = 0x0;
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1);
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset);
	/* TIM enable counter */
	TIM_Cmd(TIM3, ENABLE);
}

uint32_t TIM3_GetFreq(void)
{
	float freq;
	freq = 1000000 / (TIM_GetCapture1(TIM3) + 1);
	return round(freq);
}


void TIM7_Sets(u8 sta) // 开关 0关 1开
{
	if (sta)
	{

		TIM_SetCounter(TIM7, 0); // 计数器清空
		TIM_Cmd(TIM7, ENABLE);	 // 使能TIMx
	}
	else
		TIM_Cmd(TIM7, DISABLE); // 关闭定时器4
}

uint8_t Data_Init_Finsh = 0;
uint16_t DMA_CNT = 0; // 500ms更新一次数据

// 10ms中断1次
void TIM7Serve_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); // 使能TIM7时钟

	TIM_TimeBaseStructure.TIM_Period = 999;						// 9999				// 设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler = 83;					// 8399				// 设置用来作为TIMx时钟频率除数的预分频值
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// 设置时钟分割
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM向上计数模式
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);				// 根据指定的参数初始化TIMx的时间基数单位

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); // 使能指定的TIM7中断

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); // 初始化NVIC寄存器

	TIM_Cmd(TIM7, ENABLE); // 使能TIMx外设
}
uint16_t Renew_Count = 0;
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET) // 中断标志位
	{
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update); // 清除中断标志位
}