#include "AD7606.h"
unsigned char ad7606_value_tx_ok_flag;
unsigned char data_conver_ok_flag;
unsigned char spi_rx_ok_flag;
unsigned char ad7606_continue_flag;
char ad7606_value_original[ad7606_num][ad7606_td][2];
int16_t ad7606_value[ad7606_num][ad7606_td];
float32_t ad7606_value_float32[ad7606_num][ad7606_td];
uint8_t AD7606_TX_Flag;
// 综合配置了AD7606,开启八个通道,并且以TD[0]高 TD[0]低 ==>TD[1]高 TD[1]低 的方式搬运数据
// AD7606的设置进入这里改
// ad7606_td_num 8  //开启通道数量,通过此处设置
// ad7606_size 1024    //采样数量,通过此处设置
// 过采样和采样倍率进ad7606_init();
void AD7606_Init_Synthesis(void)
{
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG, ENABLE);
    ad7606_continue_flag = 1;                                            // 置0则不持续传输
    spi3_rx_dma((u32)ad7606_value_original, ad7606_num * ad7606_td * 2); // 设置dma搬运spi数据的数量
    dma_enable(DMA1_Stream0, ad7606_num * ad7606_td * 2);                // 使能dma,确定dma传输数据的大小
    ad7606_init();                                                       // 初始化AD7606,进入这里可设置 100KHZ
    TIM2_AD7606_Init(25, 83);                                           // TIM2,AD7606的时钟,太慢的话SPI发送速率跟不上  //f =84M/arr/psc  采样频率 请配合计时器使用

    AD_CS_LOW();           // 下降沿,正式开始传送数据
    TIM_Cmd(TIM2, ENABLE); // 定时器使能
    // Init_EXTI_for_AD7606();
}
// 通用定时器2中断初始化
//  arr：自动重装值。
//  psc：时钟预分频数
// 定时器溢出时间计算方法:Tout=((arr+1)*(psc+1))/Ft us.
//  Ft=定时器工作频率,单位:Mhz
void TIM2_AD7606_Init(u16 arr, u16 psc)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;              // 定时器2中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02; // 抢占优先级2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;        // 子优先级3
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);            /// 使能TIM2时钟
    TIM_TimeBaseInitStructure.TIM_Period = arr;                     // 自动重装载值
    TIM_TimeBaseInitStructure.TIM_Prescaler = psc;                  // 定时器分频
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 向上计数模式
    TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;     //
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);             // 初始化TIM2

    TIM_ClearFlag(TIM2, TIM_FLAG_Update);      // 清除标志位
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 更新中断

    TIM_SetCounter(TIM2, 0); // 计数器清零
    TIM_Cmd(TIM2, DISABLE);
}
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) // 中断标志位
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
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); // 清除中断标志位
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

// 在这里面可以改采样倍率和采样范围
void ad7606_init(void)
{
    GPIO_InitTypeDef GPIO_Initstruct;

    GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;     // 设置为输入
    GPIO_Initstruct.GPIO_Pin = GPIO_Pin_13;       // 指定引脚
    GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_NOPULL; // 设为浮空
    GPIO_Initstruct.GPIO_Speed = GPIO_High_Speed; // 设置速度
    GPIO_Init(GPIOC, &GPIO_Initstruct);           // 初始化IO口

    pcout(0, 1);
    pcout(1, 1);
    pcout(2, 1);
    pcout(3, 1);

    peout(4, 1);
    peout(5, 1);
    peout(6, 1);

    /* 设置过采样模式 */
    ad7606_SetOS(0); // 在这可以设置过采样倍率0-6
    ad7606_Reset();  /* 复位硬件 */
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
*	函 数 名: ad7606_StartRecord
*	功能说明: 开始采集
*	形    参：_ulFreq : 采样频率, 单位 HZ
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StartRecord(void)
{
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_HIGH(); /* 连续执行2次，低电平约50ns */
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_StartConv
*	功能说明: 启动AD7606的ADC转换
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_StartConv(void)
{
    /* 上升沿开始转换，低电平持续时间至少25ns  */
    AD_CONVST_LOW();
    AD_CONVST_LOW();
    AD_CONVST_LOW(); /* 连续执行2次，低电平约50ns */
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
*	函 数 名: ad7606_Reset
*	功能说明: 硬件复位AD7606
*	形    参：无
*	返 回 值: 无
*********************************************************************************************************
*/
void ad7606_Reset(void)
{
    /* AD7606是高电平复位，要求最小脉宽50ns */
    AD_RESET_LOW();

    AD_RESET_HIGH();
    AD_RESET_HIGH();
    AD_RESET_HIGH();
    AD_RESET_HIGH();

    AD_RESET_LOW();
}

/*
*********************************************************************************************************
*	函 数 名: ad7606_SetOS
*	功能说明: 设置过采样模式（数字滤波，硬件求平均值)
*	形    参：_ucMode : 0-6  0表示无过采样，1表示2倍，2表示4倍，3表示8倍，4表示16倍
*				5表示32倍，6表示64倍
*	返 回 值: 无
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
    else /* 按0处理 */
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

    /* 发送一个字节 */
    SPI_I2S_SendData(SPI3, 0);

    /* 等待数据接收完毕 */
    while (SPI_I2S_GetFlagStatus(SPI3, SPI_I2S_FLAG_RXNE) == RESET)
        ;

    /* 读取接收到的数据 */
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
        printf("通道:%d\r\n", td);
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
        printf("通道:%d\r\n", td);
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
        printf("通道:%d\r\n", td);
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
        printf("通道:%d\r\n", Chanel);
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
        printf("通道:%d\r\n", Chanel);
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
 
        printf("通道:%d\r\n", Chanel);
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