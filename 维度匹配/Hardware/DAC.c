#include "DAC.h"

// uint16_t sine_wave[WAVEFORM_POINTS];

// // 初始化正弦波查找表
// void init_sine_wave(void)
// {
//     for (uint16_t i = 0; i < WAVEFORM_POINTS; ++i)
//     {
//         sine_wave[i] = (sin(2 * 3.14159 * i / WAVEFORM_POINTS) + 1)  * 4095; // 将浮点值转换为12位DAC值
//         printf("%d ",sine_wave[i]);
//     }
// }

// void DAC_Config(void)
// {
//     DAC_InitTypeDef DAC_InitStruct;
//     DMA_InitTypeDef DMA_InitStruct;
//     GPIO_InitTypeDef GPIO_InitStruct;

//     RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1 | RCC_AHB1Periph_GPIOA, ENABLE);
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

//     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5; // 使用DAC Channel2: PA.05
//     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
//     GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
//     GPIO_Init(GPIOA, &GPIO_InitStruct);

//     DMA_DeInit(DMA1_Stream6); // 使用DMA1 Stream6 Channel7
//     DMA_InitStruct.DMA_Channel = DMA_Channel_7;
//     DMA_InitStruct.DMA_PeripheralBaseAddr = (uint32_t)DAC->DHR12R2;
//     DMA_InitStruct.DMA_Memory0BaseAddr = (uint32_t)&sine_wave;
//     DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;
//     DMA_InitStruct.DMA_BufferSize = WAVEFORM_POINTS;//256
//     DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//     DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;
//     DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
//     DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
//     DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;
//     DMA_InitStruct.DMA_Priority = DMA_Priority_High;
//     DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Disable;
//     DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
//     DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
//     DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
//     DMA_Init(DMA1_Stream6, &DMA_InitStruct);

//     DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO; // 使用TIM6作为触发源
//     DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;
//     DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Disable;
//     DAC_Init(DAC_Channel_2, &DAC_InitStruct);

//     DAC_DMACmd(DAC_Channel_2, ENABLE);
//     DAC_Cmd(DAC_Channel_2, ENABLE);

//     DMA_Cmd(DMA1_Stream6, ENABLE);
//     DAC_DMACmd(DAC_Channel_2,ENABLE);
// }

// void DACFre_Config(uint16_t freq)
// {
//     TIM_TimeBaseInitTypeDef TIM_InitStruct;
//     RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);

//     TIM_TimeBaseStructInit(&TIM_InitStruct);
//     TIM_InitStruct.TIM_Period = 84000000 / freq / WAVEFORM_POINTS - 1; // 计算出频率对应的计数周期
//     TIM_TimeBaseInit(TIM6, &TIM_InitStruct);

//     TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

//     TIM_Cmd(TIM6, ENABLE);
// }

// void TIM6_Init(void)
// {

//   // 开启TIM6时钟
//   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
  
//   // 初始化TIM6
//   TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//   TIM_TimeBaseStructure.TIM_Prescaler = (SystemCoreClock / 1000000) - 1;  // 设置预分频值，得到1us的时基
//   TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  // 设置为向上计数模式
//   TIM_TimeBaseStructure.TIM_Period = 1000 - 1;  // 设置自动重加载值，每1000us产生一次更新（溢出）事件，即每1ms
//   TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // 设置时钟分频因子，不分频
//   TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
//   TIM_TimeBaseInit(TIM6, &TIM_TimeBaseStructure);
  
//   // 设置TIM6的更新事件（溢出）为触发源
//   TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);

//   // 启动TIM6
//   TIM_Cmd(TIM6, ENABLE);
// }


u16 sinTable[WaveSize];
u16 cosTable[WaveSize];
/*
psc =0;
*/
void TIM6DAC_Init(u16 Freq, u16 psc)
{
    static u16 arr;
    arr = round((float32_t)(84000000)/(float32_t)WaveSize/(float32_t)Freq);
    // psc = 8400000 / WaveSize / Freq;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); ///使能TIM6时钟

    TIM_TimeBaseInitStructure.TIM_Period = arr-1; 	//自动重装载值(84000000/(WaveSize)/arr)
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc; //定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;

    TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStructure); //初始化TIM6
	
	/* TIM6 TRGO selection */
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	
	TIM_Cmd(TIM6,ENABLE); //使能定时器6

}

DAC_InitTypeDef  DAC_InitStructure;
void DAC_GPIO_Configuration(void)
{
    
	/* Preconfiguration before using DAC----------------------------------------*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/* DMA1 clock enable */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	/* GPIOA clock enable (to be used with DAC) */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);                         
	/* DAC Periph clock enable */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);

	/* DAC channel 1 & 2 (DAC_OUT1 = PA.4) configuration */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_NOPULL;// GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void DAC_Ch1_SineWaveConfig(void)
{
	DMA_InitTypeDef DMA_InitStructure;

	/* DAC channel1 Configuration */
	DAC_InitStructure.DAC_Trigger = DAC_Trigger_T6_TRGO;
	DAC_InitStructure.DAC_WaveGeneration = DAC_WaveGeneration_None;
	DAC_InitStructure.DAC_OutputBuffer = DAC_OutputBuffer_Disable;//DAC_OutputBuffer_Enable;
	DAC_Init(DAC_Channel_1, &DAC_InitStructure);

	/* DMA1_Stream5 channel7 configuration **************************************/
	DMA_DeInit(DMA1_Stream5);
	DMA_InitStructure.DMA_Channel = DMA_Channel_7;  
	DMA_InitStructure.DMA_PeripheralBaseAddr =(uint32_t)(&DAC->DHR12R1);
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)&sinTable;
	DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;
	DMA_InitStructure.DMA_BufferSize = WaveSize;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode =DMA_Mode_Circular; //DMA_Mode_Normal;//
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMA_InitStructure);

	/* Enable DMA1_Stream6 */
	DMA_Cmd(DMA1_Stream5, ENABLE);

	/* Enable DAC Channel1 */
	DAC_Cmd(DAC_Channel_1, ENABLE);
//	DAC_SetChannel1Data(DAC_Align_12b_R,0);
	/* Enable DMA for DAC Channel2 */
	DAC_DMACmd(DAC_Channel_1, ENABLE);
}


void sin_Generation(void)
{
    u16 n;
    for(n = 0; n < WaveSize; n++)
    {
        sinTable[n] = (sin(2 * PI * n / WaveSize) + 1) * 2047;
    }
}
void cos_Generation(void)
{
    u16 n;
    for(n = 0; n < WaveSize; n++)
    {
        cosTable[n] = (cos(2 * PI * n / WaveSize) + 1) * 2047;
    }
}
void DMA1_Stream5_NVIC_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority =2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void DMA2_Stream1_IRQHandler(void)
{
	if (DMA_GetITStatus(DMA1_Stream5, DMA_IT_TCIF0))
	{
		DMA_ClearITPendingBit(DMA1_Stream5, DMA_IT_TCIF0);
		{

		}
	}
}

void DAC_Signal_Init(void)
{
    DAC_GPIO_Configuration();
	DMA1_Stream5_NVIC_Init();
	sin_Generation();
	DAC_Ch1_SineWaveConfig();
    TIM6DAC_Init(15000,0);
}