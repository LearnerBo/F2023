#include "byyc_dma.h"
#include "byyc_gpio.h"


// DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
// DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// td:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
// per:�����ַ
// mem:�洢����ַ
// size:���ݴ�����
void dma_init_mem_to_Peripheral(DMA_Stream_TypeDef *DMA_Streamx, u32 td, u32 per, u32 mem, u16 size)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((u32)DMA_Streamx > (u32)DMA2) //�õ���ǰstream������DMA2����DMA1
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // DMA2ʱ��ʹ��
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA1ʱ��ʹ��
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
        ; //�ȴ�DMA������

    /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel = td;                                     //ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = per;                         // DMA�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = mem;                            // DMA �洢��0��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToPeripheral;                 //�洢��������ģʽ
    DMA_InitStructure.DMA_BufferSize = size;                                //���ݴ�����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //���������ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ʹ����ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   //�е����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ�����δ���
    DMA_Init(DMA_Streamx, &DMA_InitStructure);                          //��ʼ��DMA Stream
}

// DMAx�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
// DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// td:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
// size:���ݴ�����
void dma_init_mem_to_mem(DMA_Stream_TypeDef *DMA_Streamx, u32 td, u32 memout, u32 memin, u16 size)
{

    DMA_InitTypeDef DMA_InitStructure;

    if ((u32)DMA_Streamx > (u32)DMA2) //�õ���ǰstream������DMA2����DMA1
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // DMA2ʱ��ʹ��
    }
    else
    {
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA1ʱ��ʹ��
    }
    DMA_DeInit(DMA_Streamx);

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
        ; //�ȴ�DMA������
    /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel = td;                                     //ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = memout;                      // DMA�洢��out��ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = memin;                          // DMA �洢��in��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_MemoryToMemory;                     //�洢�����洢��ģʽ
    DMA_InitStructure.DMA_BufferSize = size;                                //���ݴ�����
    DMA_InitStructure.DMA_PeripheralInc = DMA_MemoryInc_Enable;             //�洢������ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_Byte;     //�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;                           // ʹ����ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                   //�е����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Enable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ�����δ���
    DMA_Init(DMA_Streamx, &DMA_InitStructure);                          //��ʼ��DMA Stream
}

unsigned char dma_txok_flag = 0;
void dma_init_adc1_to_mem(u32 mem, u16 size)
{

    DMA_InitTypeDef DMA_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2, ENABLE); // DMA2ʱ��ʹ��

    DMA_DeInit(DMA2_Stream0);

    while (DMA_GetCmdStatus(DMA2_Stream0) != DISABLE)
        ; //�ȴ�DMA������

    /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                              //ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)&ADC1->DR;                  // DMA�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = mem;                                // DMA �洢��0��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                     //���赽�洢��ģʽ
    DMA_InitStructure.DMA_BufferSize = size;                                    //���ݴ�����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;            //���������ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                     //�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�������ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_PeripheralDataSize_HalfWord;     //�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                             // ʹ����ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;                       //�е����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ�����δ���
    DMA_Init(DMA2_Stream0, &DMA_InitStructure);                         //��ʼ��DMA Stream
    DMA_ClearITPendingBit(DMA2_Stream0, DMA_IT_TCIF0);

    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
    NVIC_Init(&NVIC_InitStructure);

    DMA_Cmd(DMA2_Stream0, ENABLE);
    DMA_ITConfig(DMA2_Stream0, DMA_IT_TC, ENABLE);
}

//����һ��DMA����
// DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
// size:���ݴ�����
void dma_enable(DMA_Stream_TypeDef *DMA_Streamx, u16 size)
{
    DMA_Cmd(DMA_Streamx, DISABLE); //�ر�DMA����

    while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE)
        ; //ȷ��DMA���Ա�����

    DMA_SetCurrDataCounter(DMA_Streamx, size); //���ݴ�����

    DMA_Cmd(DMA_Streamx, ENABLE); //����DMA����
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

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE); // DMA2ʱ��ʹ��

    DMA_DeInit(DMA1_Stream0);

    while (DMA_GetCmdStatus(DMA1_Stream0) != DISABLE)
        ; //�ȴ�DMA������

    /* ���� DMA Stream */
    DMA_InitStructure.DMA_Channel = DMA_Channel_0;                          //ͨ��ѡ��
    DMA_InitStructure.DMA_PeripheralBaseAddr = SPI3_DR;                     // DMA�����ַ
    DMA_InitStructure.DMA_Memory0BaseAddr = mem;                            // DMA �洢��0��ַ
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;                 //���赽�洢��ģʽ
    DMA_InitStructure.DMA_BufferSize = size;                                //���ݴ�����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;        //���������ģʽ
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;                 //�洢������ģʽ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte; //�������ݳ���:8λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;         //�洢�����ݳ���:8λ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                         // ʹ����ͨģʽ
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;                 //�е����ȼ�
    DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;
    DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_Full;
    DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;         //�洢��ͻ�����δ���
    DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single; //����ͻ�����δ���
    DMA_Init(DMA1_Stream0, &DMA_InitStructure);                         //��ʼ��DMA Stream
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
