#include "byyc_dma.h"
#include "byyc_gpio.h"


// DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
// DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// td:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
// per:外设地址
// mem:存储器地址
// size:数据传输量
void dma_init_mem_to_Peripheral(DMA_Stream_TypeDef *DMA_Streamx, u32 td, u32 per, u32 mem, u16 size)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((u32)DMA_Streamx > (u32)DMA2) //得到当前stream是属于DMA2还是DMA1
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // DMA2时钟使能
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA1时钟使能
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
        ; //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = td;                                     //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = per;                         // DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = mem;                            // DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 //存储器到外设模式
    DMA_InitStructure.DMA_BufferSize = size;                                //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   //中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA_Streamx, &DMA_InitStructure);                          //初始化DMA Stream
}

// DMAx的各通道配置
//这里的传输形式是固定的,这点要根据不同的情况来修改
//从存储器->外设模式/8位数据宽度/存储器增量模式
// DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// td:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
// size:数据传输量
void dma_init_mem_to_mem(DMA_Stream_TypeDef *DMA_Streamx, u32 td, u32 memout, u32 memin, u16 size)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((u32)DMA_Streamx > (u32)DMA2) //得到当前stream是属于DMA2还是DMA1
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // DMA2时钟使能
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA1时钟使能
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
        ; //等待DMA可配置
    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = td;                                     //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = memout;                      // DMA存储器out地址
    DMA_InitStructure.DMA_Memory0BaseAddr = memin;                          // DMA 存储器in地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;                     //存储器到存储器模式
    DMA_InitStructure.DMA_BufferSize = size;                                //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_MemoryInc_Enable;             //存储器增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //存储器数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;     //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   //中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA_Streamx, &DMA_InitStructure);                          //初始化DMA Stream
}

unsigned char dma_txok_flag = 0;
void dma_init_adc1_to_mem(u32 mem, u16 size)
{

    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // DMA2时钟使能

    DMA_DeInit(DMA2_Stream0);

    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
        ; //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                              //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;                  // DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = mem;                                // DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = size;                                    //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;     //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);                         //初始化DMA Stream
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA2_Stream0, ENABLE);
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}

//开启一次DMA传输
// DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
// size:数据传输量
void dma_enable(DMA_Stream_TypeDef *DMA_Streamx, u16 size)
{
    DMA_Cmd(DMA_Streamx, DISABLE); //关闭DMA传输

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
        ; //确保DMA可以被设置

    DMA_SetCurrDataCounter(DMA_Streamx, size); //数据传输量

    DMA_Cmd(DMA_Streamx, ENABLE); //开启DMA传输
}

void DMA2_Stream0_IRQHandler(void)
{
    static char x = 0;
    if (DMA_GetFlagStatus(DMA2_Stream0, DMA_FLAG_TCIF0))
    {
        paout(7, x);
        x = !x;
        dma_txok_flag = 1;
    }
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);
}

#define SPI3_DR (u32) & SPI3->DR

void spi3_rx_dma(u32 mem, u16 size)
{
    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2时钟使能

    DMA_DeInit(DMA1_Stream0);

    while (DMA_GetCmdStatus(DMA1_Stream0) != DISABLE)
        ; //等待DMA可配置

    /* 配置 DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                          //通道选择
    DMA_InitStructure.DMA_PeripheralBaseAddr = SPI3_DR;                     // DMA外设地址
    DMA_InitStructure.DMA_Memory0BaseAddr = mem;                            // DMA 存储器0地址
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 //外设到存储器模式
    DMA_InitStructure.DMA_BufferSize = size;                                //数据传输量
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //外设非增量模式
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //存储器增量模式
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //外设数据长度:8位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //存储器数据长度:8位
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         // 使用普通模式
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //中等优先级
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //存储器突发单次传输
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //外设突发单次传输
    DMA_Init(DMA1_Stream0, &DMA_InitStructure);                         //初始化DMA Stream
    DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);

	// DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
    DMA_Cmd(DMA1_Stream0, ENABLE);
    DMA_ITConfig(DMA1_Stream0, DMA_IT_TC, ENABLE);

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//0
    NVIC_Init(&NVIC_InitStructure);
}
