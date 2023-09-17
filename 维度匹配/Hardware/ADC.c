#include "adc.h"

uint16_t AD[AD_Points];
float32_t AD_Value[AD_Points];
uint16_t AD2[AD_Points];
float32_t AD2_Value[AD_Points];
uint16_t AD3[AD_Points];
float32_t AD3_Value[AD_Points];

// ADCCLK=PCLK2/4=84/4=21Mhz
// Tcov=3Cycles+12=15
// Fs=21/15=1.4MHZ
//===============================ADC1_DMA2
void Adc1_Dma2_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);  // 使能ADC1时钟

	// 先初始化ADC1通道5 IO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;		 // PA5 通道5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 // 初始化

	//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					 // 独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //(此处未使用) 在双重或三重交错模式下使用,表示两个采样阶段之间的延迟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		 // （对于多个 ADC 模式）
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					 // 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									 // 初始化									 // 初始化

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // 12位模式(resolution:分辨率)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		   // 单通道,非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   // 不用定时器触发,连续转换

	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 禁止触发检测，使用软件触发

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;			   // 1个转换在规则序列中 也就是只转换规则序列1(规则组的转换长度)
	ADC_Init(ADC1, &ADC_InitStructure);					   // ADC初始化

	// ADCx, ADC的规则组通道ADC_Channel_x[PAx], 序列号x[1-16], 采样时间ADC_SampleTime_xCycles[3,15,28,56,84,112,144,480]
	// Tcov=3Cycles(采样保持可调周期)+12(量化编码固定周期)=15
	ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 1, ADC_SampleTime_3Cycles);

	//=================================ADC1对应DMA2_Channel_0_Stream0=====================================
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // 使能DMA2时钟

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_0;								// DMA通道选择!!!!!!!!!!!!
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;				// 复制ADC转换结果!!!!!!!!!!!!
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ADC结果16位(12)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 单通道,不递增
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)AD;						// 粘贴到指定数组!!!!!!!!!!!!!!
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// uint16_t AD[i];
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// i++
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = AD_Points; //!!!!!!!!!
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // DMA_Mode_Normal!!!!!!!!!!!!!!!!!
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// 禁止DMA FIFO，使用直连模式
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	// FIFO 大小，FIFO模式禁止时，这个不用配置
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream0, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream0, ENABLE); // DMA选择!!!!!!!!!!!!!!!!!!!!!!!!

	DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE); // 使能DMA的传输完成中断

	ADC_DMACmd(ADC1, ENABLE);
	//================================ADC1对应DMA2_Channel_0_Stream0=========================================

	ADC_Cmd(ADC1, ENABLE); // 开启AD转换器
	ADC_SoftwareStartConv(ADC1);
}

//=============================================ADC2_DMA
void Adc2_Dma2_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC2, ENABLE);  // 使能ADC1时钟

	// 先初始化ADC1通道5 IO口
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;		 // PA5 通道5
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 // 初始化

	//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					 // 独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //(此处未使用) 在双重或三重交错模式下使用,表示两个采样阶段之间的延迟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		 // （对于多个 ADC 模式）
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					 // 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									 // 初始化

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // 12位模式(resolution:分辨率)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		   // 单通道,非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   // 不用定时器触发,连续转换

	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None; // 禁止触发检测，使用软件触发

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;			   // 1个转换在规则序列中 也就是只转换规则序列1(规则组的转换长度)
	ADC_Init(ADC2, &ADC_InitStructure);					   // ADC初始化
														   // ADCx, ADC的规则组通道ADC_Channel_x[PAx], 序列号x[1-16], 采样时间ADC_SampleTime_xCycles[3,15,28,56,84,112,144,480]
	// Tcov=3Cycles(采样保持可调周期)+12(量化编码固定周期)=15
	ADC_RegularChannelConfig(ADC2, ADC_Channel_4, 1, ADC_SampleTime_3Cycles);
	//=================================ADC2对应DMA2_Channel_1_Stream3=====================================
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // 使能DMA2时钟

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_1;								// DMA通道选择!!!!!!!!!!!!
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC2->DR;				// 复制ADC转换结果!!!!!!!!!!!!
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ADC结果16位(12)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 单通道,不递增
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)AD2;						// 粘贴到指定数组!!!!!!!!!!!!!!
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// uint16_t AD[i];
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// i++
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = AD_Points; //!!!!!!!!!
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular; // DMA_Mode_Normal!!!!!!!!!!!!!!!!!
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	// 禁止DMA FIFO，使用直连模式
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	// FIFO 大小，FIFO模式禁止时，这个不用配置
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;

	DMA_Init(DMA2_Stream3, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream3, ENABLE); // DMA选择!!!!!!!!!!!!!!!!!!!!!!!!

	DMA_ITConfig(DMA2_Stream3, DMA_IT_TC, ENABLE); // 使能DMA的传输完成中断

	ADC_DMACmd(ADC2, ENABLE);
	//================================ADC1对应DMA2_Channel_0_Stream0=========================================

	ADC_Cmd(ADC2, ENABLE); // 开启AD转换器
	ADC_SoftwareStartConv(ADC2);
}

//=============================================ADC3_DMA
void Adc3_Dma2_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE); // 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);  // 使能ADC1时钟

	// 先初始化GPIO
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;		 //!!!!!!!!!!
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;	 // 模拟输入
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL; // 不带上下拉
	GPIO_Init(GPIOA, &GPIO_InitStructure);			 // 初始化

	//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,ENABLE);	  //ADC1复位
	//	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC1,DISABLE);	//复位结束

	ADC_CommonInitTypeDef ADC_CommonInitStructure;
	ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;					 // 独立模式
	ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles; //(此处未使用) 在双重或三重交错模式下使用,表示两个采样阶段之间的延迟
	ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;		 // （对于多个 ADC 模式）
	ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;					 // 预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
	ADC_CommonInit(&ADC_CommonInitStructure);									 // 初始化

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b; // 12位模式(resolution:分辨率)
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;		   // 单通道,非扫描模式
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;	   // 不用定时器触发,连续转换
	
	
	 // 禁止触发检测，使用软件触发
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;

	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right; // 右对齐
	ADC_InitStructure.ADC_NbrOfConversion = 1;			   // 1个转换在规则序列中 也就是只转换规则序列1(规则组的转换长度)
	ADC_Init(ADC3, &ADC_InitStructure);					   // ADC初始化

	// ADCx, ADC的规则组通道ADC_Channel_x[PAx], 序列号x[1-16], 采样时间ADC_SampleTime_xCycles[3,15,28,56,84,112,144,480]
	// Tcov=3Cycles(采样保持可调周期)+12(量化编码固定周期)=15
	ADC_RegularChannelConfig(ADC3, ADC_Channel_3, 1, ADC_SampleTime_3Cycles);

	//=================================ADC3对应DMA2_Channel_2_Stream1=====================================
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // 使能DMA2时钟

	DMA_InitTypeDef DMA_InitStructure;
	DMA_InitStructure.DMA_Channel = DMA_Channel_2;								// DMA通道选择!!!!!!!!!!!!
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC3->DR;				// 复制ADC转换结果!!!!!!!!!!!!
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; // ADC结果16位(12)
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			// 单通道,不递增
	DMA_InitStructure.DMA_Memory0BaseAddr = (uint32_t)AD3;						// 粘贴到指定数组!!!!!!!!!!!!!!
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			// uint16_t AD[i];
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						// i++

	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;
	DMA_InitStructure.DMA_BufferSize = AD_Points;		//!!!!!!!!!
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;		//!!!!!!!!!DMA_Mode_Circular
	DMA_InitStructure.DMA_Priority = DMA_Priority_High; // DMA_Priority_Medium
														// 禁止DMA FIFO，使用直连模式
	DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
	// FIFO 大小，FIFO模式禁止时，这个不用配置
	DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull;
	DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA2_Stream1, &DMA_InitStructure);

	DMA_Cmd(DMA2_Stream1, ENABLE); // DMA选择!!!!!!!!!!!!!!!!!!!!!!!!

	DMA_ITConfig(DMA2_Stream1, DMA_IT_TC, ENABLE); // 使能DMA的传输完成中断

	ADC_DMACmd(ADC3, ENABLE); // 选择特定ADC作为DMA的触发信号!!!!!!!!!!!!!!!!!!!!!!
	//================================ADC3对应DMA2_Channel_2_Stream1=========================================

	ADC_Cmd(ADC3, ENABLE);
	ADC_SoftwareStartConv(ADC3);
}
/*
获得ADC值
ch: @ref ADC_Channel_x
通道值 0~16取值范围为：ADC_Channel_0~ADC_Channel_16
返回值:转换结果
*/
uint16_t Get_Adc(uint8_t ch)
{
	ADC_RegularChannelConfig(ADC1, ch, 1, ADC_SampleTime_480Cycles); // 设置指定ADC的规则组通道，一个序列，采样时间,ADC1,ADC通道,480个周期,提高采样时间可以提高精确度

	ADC_SoftwareStartConv(ADC1); // 使能指定的ADC1的软件转换启动功能

	while (!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC))
		; // 等待转换结束

	return ADC_GetConversionValue(ADC1); // 返回最近一次ADC1规则组的转换结果
}

/*
获取通道ch的转换值，取times次,然后平均
ch:通道编号
times:获取次数
返回值:通道ch的times次转换结果平均值
*/

//===============================ADC
uint16_t Get_Adc_Average(uint8_t ch, uint8_t times)
{
	u32 temp_val = 0;
	u8 t;
	delay_init(168);
	for (t = 0; t < times; t++)
	{
		temp_val += Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val / times;
}

void Data_Init(const uint16_t *Input, float32_t *Output, uint16_t size)
{
	while (size--)
	{
		(*Output++) = (*Input++) * 3.3 / 4096;
	}
}

// void DMA2_Stream0_NVIC_Init(void)
// {
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// }

// void DMA_NVIC_Init(void)
// {
// 	NVIC_InitTypeDef NVIC_InitStructure;
// 	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn; // ADC1
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// 	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream3_IRQn; // ADC2
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// 	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream1_IRQn; // ADC3
// 	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
// 	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
// 	NVIC_Init(&NVIC_InitStructure);
// }
// void LED_Init(void)
// {
// 	GPIO_InitTypeDef GPIO_InitStructure;

// 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); // 使能GPIOF时钟

// 	// GPIOF9,F10初始化设置
// 	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_10 | GPIO_Pin_11;
// 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;	   // 普通输出模式
// 	GPIO_InitStructure.GPIO_OType = GPIO_PuPd_NOPULL;  // 推挽输出
// 	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz; // 100MHz
// 	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;	   // 上拉
// 	GPIO_Init(GPIOD, &GPIO_InitStructure);			   // 初始化

// 	GPIO_ResetBits(GPIOA, GPIO_Pin_12 | GPIO_Pin_10 | GPIO_Pin_11);
// 	// GPIO_SetBits(GPIOD,GPIO_Pin_12 |GPIO_Pin_10 |GPIO_Pin_11 );
// }
// void DMA2_Stream0_IRQHandler(void)
// {
// 	if (DMA_GetITStatus(DMA2_Stream0, DMA_IT_TCIF0))
// 	{
// 		DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
// 		{

// 			// GPIO_SetBits(GPIOD, GPIO_Pin_11);
// 			// GPIO_SetBits(GPIOD, GPIO_Pin_12);

// 			// ADC_Cmd(ADC1,DISABLE);
// 			DMA_Cmd(DMA2_Stream0, DISABLE); // 关闭DMA
// 			// Data_Init(AD, AD_Value, AD_Points);
// 			GPIO_SetBits(GPIOD, GPIO_Pin_10);
// 			DMA_Cmd(DMA2_Stream0, ENABLE); // 开启DMA
// 										   // Adc1_Dma2_Init();
// 										   // printf("ADC1 IS OK\r\n");
// 										   // ADC_SoftwareStartConv(ADC1);
// 		}
// 	}
// }

// void DMA2_Stream3_IRQHandler(void)
// {
// 	if (DMA_GetITStatus(DMA2_Stream3, DMA_IT_TCIF0))
// 	{
// 		DMA_ClearITPendingBit(DMA2_Stream3, DMA_IT_TCIF0);
// 		{

// 			// ADC_Cmd(ADC2,DISABLE);
// 			DMA_Cmd(DMA2_Stream3, DISABLE); // 关闭DMA
// 											// Data_Init(AD2, AD2_Value, AD_Points);
// 			GPIO_SetBits(GPIOD, GPIO_Pin_11);
// 			DMA_Cmd(DMA2_Stream3, ENABLE); // 开启DMA
// 										   // Adc2_Dma2_Init();
// 										   // printf("ADC2 IS OK\r\n");
// 		}
// 	}
// }

// void DMA2_Stream1_IRQHandler(void)
// {
// 	if (DMA_GetITStatus(DMA2_Stream1, DMA_IT_TCIF0))
// 	{
// 		DMA_ClearITPendingBit(DMA2_Stream1, DMA_IT_TCIF0);
// 		{

// 			// ADC_Cmd(ADC3,DISABLE);
// 			DMA_Cmd(DMA2_Stream1, DISABLE); // 关闭DMA
// 											// Data_Init(AD3, AD3_Value, AD_Points);
// 			GPIO_SetBits(GPIOD, GPIO_Pin_12);
// 			DMA_Cmd(DMA2_Stream1, ENABLE); // 开启DMA
// 										   // Adc3_Dma2_Init();

// 			// printf("ADC3 IS OK\r\n");
// 		}
// 	}
// }