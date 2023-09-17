#include "AD7606.h"
unsigned char ad7606_value_tx_ok_flag;
unsigned char data_conver_ok_flag;
unsigned char spi_rx_ok_flag;
unsigned char ad7606_continue_flag;
char ad7606_value_original[ad7606_num][ad7606_td][2];
int16_t ad7606_value[ad7606_num][ad7606_td];
float32_t ad7606_value_float32[ad7606_num][ad7606_td];
uint8_t AD7606_TX_Flag;
// �ۺ�������AD7606,�����˸�ͨ��,������TD[0]�� TD[0]�� ==>TD[1]�� TD[1]�� �ķ�ʽ��������
// AD7606�����ý��������
// ad7606_td_num 8  //����ͨ������,ͨ���˴�����
// ad7606_size 1024    //��������,ͨ���˴�����
// �������Ͳ������ʽ�ad7606_init();
void AD7606_Init_Synthesis(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    ad7606_continue_flag = 1;                                            // ��0�򲻳�������
    spi3_rx_dma((u32)ad7606_value_original, ad7606_num * ad7606_td * 2); // ����dma����spi���ݵ�����
    dma_enable(DMA1_Stream0, ad7606_num * ad7606_td * 2);                // ʹ��dma,ȷ��dma�������ݵĴ�С
    ad7606_init();                                                       // ��ʼ��AD7606,������������� 100KHZ
    TIM2_AD7606_Init(25, 83);                                           // TIM2,AD7606��ʱ��,̫���Ļ�SPI�������ʸ�����  //f =84M/arr/psc  ����Ƶ�� ����ϼ�ʱ��ʹ��

    AD_CS_LOW();           // �½���,��ʽ��ʼ��������
    TIM_Cmd(TIM2, ENABLE); // ��ʱ��ʹ��
    // Init_EXTI_for_AD7606();
}
// ͨ�ö�ʱ��2�жϳ�ʼ��
//  arr���Զ���װֵ��
//  psc��ʱ��Ԥ��Ƶ��
// ��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=��ʱ������Ƶ��,��λ:Mhz
void TIM2_AD7606_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;              // ��ʱ��2�ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // ��ռ���ȼ�2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;        // �����ȼ�3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            /// ʹ��TIM2ʱ��
    TIM_TimeBaseInitStructure.TIM_Period = arr;                     // �Զ���װ��ֵ
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  // ��ʱ����Ƶ
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // ���ϼ���ģʽ
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             // ��ʼ��TIM2

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // �����־λ
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // �����ж�

    TIM_SetCounter(TIM2, 0); // ����������
    TIM_Cmd(TIM2, DISABLE);
}
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // �жϱ�־λ
    {
        while (busy_is_1());
        if (busy_is_0() && AD7606_TX_Flag == 0)
        {
            for (int i = 0; i < ad7606_td; i++)
            {
                SPI3->DR = 0;
                while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
                    ;
                SPI3->DR = 0;
                while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
                    ;
            }
            ad7606_StartRecord();
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // ����жϱ�־λ
}

void Init_EXTI_for_AD7606(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    // Enable GPIO clock
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);

    // Configure PC13 as input for EXTI_Line13
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOC, &GPIO_InitStruct);

    // Connect EXTI Line13 to PC13
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC, EXTI_PinSource13);

    // Configure EXTI Line13
    EXTI_InitStruct.EXTI_Line = EXTI_Line13;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; // Trigger on falling edge
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    // Enable and set EXTI Line13 to the lowest priority
    NVIC_InitStruct.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}


void DMA1_Stream0_IRQHandler(void)
{
    if ((DMA_GetFlagStatus(DMA1_Stream0, DMA_FLAG_TCIF0) && (data_conver_ok_flag == 1))) //
    {
        if (ad7606_continue_flag == 1)
        {
            TIM_Cmd(TIM2, DISABLE);
            AD_CS_HIGH();
        }
        TIM_Cmd(TIM2, DISABLE);
        real_adc_value();
        TIM_Cmd(TIM2, ENABLE);
        AD_CS_LOW();
    }
    DMA_ClearITPendingBit(DMA1_Stream0, DMA_IT_TCIF0);
}

// ����������ԸĲ������ʺͲ�����Χ
void ad7606_init(void)
{
    GPIO_InitTypeDef GPIO_Initstruct;

    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;     // ����Ϊ����
    GPIO_Initstruct.GPIO_Pin = GPIO_Pin_13;       // ָ������
    GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // ��Ϊ����
    GPIO_Initstruct.GPIO_Speed = GPIO_High_Speed; // �����ٶ�
    GPIO_Init(GPIOC, &GPIO_Initstruct);           // ��ʼ��IO��

    pcout(0, 1);
    pcout(1, 1);
    pcout(2, 1);
    pcout(3, 1);

    peout(4, 1);
    peout(5, 1);
    peout(6, 1);

    /* ���ù�����ģʽ */
    ad7606_SetOS(0); // ����������ù���������0-6
    ad7606_Reset();  /* ��λӲ�� */
    AD_CONVST_HIGH();
    AD_RANGE_5V();
    // AD_RANGE_10V();  //+-10v
    ad7606_StartRecord();
    SPI3_Init();
}

short ad7606_read(void)
{
    short x;

    AD_CS_LOW();
    while (busy_is_1())
        ;
    for (int i = 0; i < 8; i++)
    {
        x = ad7606_read_bety();
        x = x << 8; //+
        ad7606_read_bety();
    }

    AD_CS_HIGH();
    ad7606_StartRecord();
    return x;
}

/*
*********************************************************************************************************
*	�� �� ��: ad7606_StartRecord
*	����˵��: ��ʼ�ɼ�
*	��    �Σ�_ulFreq : ����Ƶ��, ��λ HZ
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ad7606_StartRecord(void)
{
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_HIGH(); /* ����ִ��2�Σ��͵�ƽԼ50ns */
}

/*
*********************************************************************************************************
*	�� �� ��: ad7606_StartConv
*	����˵��: ����AD7606��ADCת��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ad7606_StartConv(void)
{
    /* �����ؿ�ʼת�����͵�ƽ����ʱ������25ns  */
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW(); /* ����ִ��2�Σ��͵�ƽԼ50ns */
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();

    AD_CONVST_HIGH();
}

/*
*********************************************************************************************************
*	�� �� ��: ad7606_Reset
*	����˵��: Ӳ����λAD7606
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ad7606_Reset(void)
{
    /* AD7606�Ǹߵ�ƽ��λ��Ҫ����С����50ns */
    AD_RESET_LOW();

    AD_RESET_HIGH();
    AD_RESET_HIGH();
    AD_RESET_HIGH();
    AD_RESET_HIGH();

    AD_RESET_LOW();
}

/*
*********************************************************************************************************
*	�� �� ��: ad7606_SetOS
*	����˵��: ���ù�����ģʽ�������˲���Ӳ����ƽ��ֵ)
*	��    �Σ�_ucMode : 0-6  0��ʾ�޹�������1��ʾ2����2��ʾ4����3��ʾ8����4��ʾ16��
*				5��ʾ32����6��ʾ64��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void ad7606_SetOS(uint8_t _ucMode)
{
    if (_ucMode == 1)
    {
        AD_OS2_0();
        AD_OS1_0();
        AD_OS0_1();
    }
    else if (_ucMode == 2)
    {
        AD_OS2_0();
        AD_OS1_1();
        AD_OS0_0();
    }
    else if (_ucMode == 3)
    {
        AD_OS2_0();
        AD_OS1_1();
        AD_OS0_1();
    }
    else if (_ucMode == 4)
    {
        AD_OS2_1();
        AD_OS1_0();
        AD_OS0_0();
    }
    else if (_ucMode == 5)
    {
        AD_OS2_1();
        AD_OS1_0();
        AD_OS0_1();
    }
    else if (_ucMode == 6)
    {
        AD_OS2_1();
        AD_OS1_1();
        AD_OS0_0();
    }
    else /* ��0���� */
    {
        AD_OS2_0();
        AD_OS1_0();
        AD_OS0_0();
    }
}

unsigned char ad7606_read_bety(void)
{
    unsigned char read;

    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_TXE) == RESET)
        ;

    /* ����һ���ֽ� */
    SPI_I2S_SendData(SPI3, 0);

    /* �ȴ����ݽ������ */
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
        ;

    /* ��ȡ���յ������� */
    read = SPI_I2S_ReceiveData(SPI3);
    return read;
}


void real_adc_value(void)
{
    uint16_t i, j;
    data_conver_ok_flag = 0;
    int16_t x;
    char y;
    int64_t Conversion;
    for (i = 0; i < ad7606_td; i++)
    {
        for (j = 0; j < ad7606_num; j++)
        {
            x = ad7606_value_original[j][i][0];
            y = ad7606_value_original[j][i][1];
            x = x << 8;
            Conversion = x | y;
            // ad7606_value[j][i] = x | y;
            ad7606_value[j][i] = Conversion;
            // ad7606_value_float32[j][i] = (float32_t)Conversion / 32767 * 5;
             ad7606_value_float32[j][i] = (float32_t)Conversion/16383.75*5  ;///16383.75 * 5
        }
    }
    data_conver_ok_flag = 1;
}

void Debug_AD7606_Value(void)
{
    data_conver_ok_flag = 0;
    for (uint8_t td = 0; td < ad7606_td_num; td++)
    {
        printf("ͨ��:%d\r\n", td);
        for (u16 num = 0; num < ad7606_size; num++)
        {
            printf("%d ", ad7606_value[num][td]);
        }
        printf("\r\n");
    }
    data_conver_ok_flag = 1;
}

void Debug_AD7606_Float32_Value()
{
    data_conver_ok_flag = 0;
    for (uint8_t td = 0; td < ad7606_td_num; td++)
    {
        printf("ͨ��:%d\r\n", td);
        for (u16 num = 0; num < ad7606_size; num++)
        {
            printf("%f ", ad7606_value_float32[num][td]);
        }
        printf("\r\n");
    }
    data_conver_ok_flag = 1;
}

void Debug_AD7606_value_original(void)
{
    data_conver_ok_flag = 0;
    for (uint8_t td = 0; td < ad7606_td_num; td++)
    {
        printf("ͨ��:%d\r\n", td);
        for (u16 num = 0; num < ad7606_size; num++)
        {
            for (uint8_t HorL = 0; HorL < 2; HorL++)
            {
                printf("%f ", ad7606_value_original[num][td][HorL]);
            }
        }
        printf("\r\n");
    }
    data_conver_ok_flag = 1;
}


void Debug_AD7606_Value_Chanel(uint8_t Chanel)
{
        data_conver_ok_flag=0;
        printf("ͨ��:%d\r\n", Chanel);
        for (u16 num = 0; num < ad7606_size; num++)
        {
            printf("%d ", ad7606_value[num][Chanel]);
        }
        printf("\r\n");
    
    data_conver_ok_flag = 1;
}


void Debug_AD7606_Float32_Value_Chanel(uint8_t Chanel)
{
    data_conver_ok_flag = 0;
        printf("ͨ��:%d\r\n", Chanel);
        for (u16 num = 0; num < ad7606_size; num++)
        {
            printf("%f ", ad7606_value_float32[num][Chanel]);
        }
        printf("\r\n");
    
    data_conver_ok_flag = 1;
}


void Debug_AD7606_value_original_Chanel(uint8_t Chanel)
{
    data_conver_ok_flag = 0;
 
        printf("ͨ��:%d\r\n", Chanel);
        for (u16 num = 0; num < ad7606_size; num++)
        {
            for (uint8_t HorL = 0; HorL < 2; HorL++)
            {
                printf("%f ", ad7606_value_original[num][Chanel][HorL]);
            }
        }
        printf("\r\n");
    
    data_conver_ok_flag = 1;
}