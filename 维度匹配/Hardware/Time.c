#include "time.h"
float TIM5phasic_float;
u32 TIM5phasic_U32;
float TIM5frequency_float;
u32 TIM5frequency_U32;
// uint8_t DMA2_Stream0_Finish=0;
// uint16_t DMA_CNT=0;//500ms����һ������
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
uint8_t channel_ready_flags = 0; // ����һ����־λ����ʾÿ��ͨ���Ƿ��Ѿ����յ�����
uint8_t data_ready_flags = 0;

uint8_t Capture_Secure_FLAG1=1;
uint8_t Capture_Secure_FLAG2=1;
uint8_t Capture_Secure_FLAG3=1;
uint8_t Capture_Secure_FLAG4=1;

void TIM5_IRQHandler(void)
{
	if ((TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG1==1)) // ���ͨ��1�Ƿ񴥷��ж�
	{
		capture1 = TIM_GetCapture1(TIM5);
		Capture_Secure_FLAG1 = 0;
		

	}
	if ((TIM_GetITStatus(TIM5, TIM_IT_CC2) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG2==1)) // ���ͨ��2�Ƿ񴥷��ж�
	{
	
		capture2 = TIM_GetCapture2(TIM5); // ��ȡ������ͨ��2�Ĳ���ֵ  //1M�ļ�����,�����õ���ֵ psc=84-1;��������ֵΪ1M
		Capture_Secure_FLAG2 =0;

		
	}
	if ((TIM_GetITStatus(TIM5, TIM_IT_CC3) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG3==1)) // ���ͨ��2�Ƿ񴥷��ж�
	{
					capture3 = TIM_GetCapture3(TIM5);
					Capture_Secure_FLAG3 =0;

	}

	if ((TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)&&(Capture_Secure_FLAG == 1)&&(Capture_Secure_FLAG4==1)) // ���ͨ��2�Ƿ񴥷��ж�
	{
					capture4 = TIM_GetCapture4(TIM5);
					Capture_Secure_FLAG4 =0;

	}

	TIM_ClearITPendingBit(TIM5, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3| TIM_IT_CC4 | TIM_IT_Update); // ����жϱ�־λ �����������־�����ĸ���׼
	
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


// ��ʱ��5ͨ��1���벶������ 0xFFFFFFFF 83
// arr���Զ���װֵ(TIM5,TIM5��32λ��!!)
// psc��ʱ��Ԥ��Ƶ�� PA0->CHANEL1��PA1->CHANEL2
// ͨ��0��ǰʱ,С��180.ͨ��1��ǰʱ,����180
void TIM5_CH1CH2_Cap_Init(u32 arr, u16 psc)
{
	TIM_ICInitTypeDef TIM5_ICInitStructure;

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);  // TIM5ʱ��ʹ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // ʹ��PORTAʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3; // GPIOA0
	// GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1; // GPIOA0
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;		   // ���ù���
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	   // �ٶ�100MHz
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;		   // ���츴�����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;	   // ����
	GPIO_Init(GPIOA, &GPIO_InitStructure);				   // ��ʼ��PA0

	GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5); // PA0����λ��ʱ��5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5); // PA1����λ��ʱ��5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_TIM5); // PA0����λ��ʱ��5
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_TIM5); // PA0����λ��ʱ��5

	TIM_TimeBaseStructure.TIM_Prescaler = psc;					// ��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period = arr;						// �Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;

	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	// ��ʼ��TIM5���벶�����
	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;				 // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 // �����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 // ���������Ƶ,����Ƶ
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;						 // IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	 TIM5_ICInitStructure.TIM_Channel = TIM_Channel_2; //CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	 TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	//�����ز���
	 TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; //ӳ�䵽TI1��
	 TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 //���������Ƶ,����Ƶ
	 TIM5_ICInitStructure.TIM_ICFilter = 0x01;//IC1F=0000 ���������˲��� ���˲�
	 TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_3;				 // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 // �����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 // ���������Ƶ,����Ƶ
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;						 // IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_4;				 // CC1S=01 	ѡ������� IC1ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;	 // �����ز���
	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // ӳ�䵽TI1��
	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;			 // ���������Ƶ,����Ƶ
	TIM5_ICInitStructure.TIM_ICFilter = 0x01;						 // IC1F=0000 ���������˲��� ���˲�
	TIM_ICInit(TIM5, &TIM5_ICInitStructure);

	TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2 |TIM_IT_CC3 | TIM_IT_CC4, ENABLE); // ��������ж� ,����CC1IE�����ж�
	// TIM_ITConfig(TIM5, TIM_IT_Update | TIM_IT_CC1 | TIM_IT_CC2, ENABLE); // ��������ж� ,����CC1IE�����ж�

	TIM_Cmd(TIM5, ENABLE); // ʹ�ܶ�ʱ��5

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0; // ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;		  // �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			  // IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							  // ����ָ���Ĳ�����ʼ��VIC�Ĵ�����
}


void TIM3Freq_Init(void) // ����PA6 ����Ƶ��
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
	TIM_TimeBaseInitStructure.TIM_Prescaler = 84 - 1;  // PSC  //1-1������Ƶ  84-1������Ƶ ��Ƶϵ��Խ��,�ܲ������ź�Ƶ�ʾ�Խ�� ��֮ͬ��
	// 84-1 ���Բ���16hz
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


void TIM7_Sets(u8 sta) // ���� 0�� 1��
{
	if (sta)
	{

		TIM_SetCounter(TIM7, 0); // ���������
		TIM_Cmd(TIM7, ENABLE);	 // ʹ��TIMx
	}
	else
		TIM_Cmd(TIM7, DISABLE); // �رն�ʱ��4
}

uint8_t Data_Init_Finsh = 0;
uint16_t DMA_CNT = 0; // 500ms����һ������

// 10ms�ж�1��
void TIM7Serve_Init()
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE); // ʹ��TIM7ʱ��

	TIM_TimeBaseStructure.TIM_Period = 999;						// 9999				// ��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = 83;					// 8399				// ����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		// ����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; // TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);				// ����ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE); // ʹ��ָ����TIM7�ж�

	NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure); // ��ʼ��NVIC�Ĵ���

	TIM_Cmd(TIM7, ENABLE); // ʹ��TIMx����
}
uint16_t Renew_Count = 0;
void TIM7_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM7, TIM_IT_Update) == SET) // �жϱ�־λ
	{
	}
	TIM_ClearITPendingBit(TIM7, TIM_IT_Update); // ����жϱ�־λ
}