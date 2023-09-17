#include "byyc_spi.h"

#if spi_software_flag

void spi_init(void)
{
   peout(10, 1);
   peout(12, 1);
   peout(14, 1);
   pbout(10, 1);
   pbout(12, 1);
   pbout(14, 1);
}

void spi_write_byte(u8 data)
{
   unsigned char i = 0;
   for (i = 8; i > 0; i--)
   {
       if (data & 0x80)
           SPI_MOSI_SET; //输出数据
       else
           SPI_MOSI_CLR;
       SPI_SCLK_CLR;
       SPI_SCLK_SET;
       data <<= 1;
   }
}

#else

/*****************************************************************************
* @name       :u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 Byte)
* @date       :2018-08-09
* @function   :Write a byte of data using STM32's hardware SPI
* @parameters :SPIx: SPI type,x for 1,2,3
               Byte:Data to be written
* @retvalue   :Data received by the bus
******************************************************************************/
u8 SPI_WriteByte(SPI_TypeDef *SPIx, u8 Byte)
{
   while ((SPIx->SR & SPI_I2S_FLAG_TXE) == RESET)
       ;            //等待发送区空
   SPIx->DR = Byte; //发送一个byte

   while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == RESET)
       ;            //等待接收完一个byte
   return SPIx->DR; //返回收到的数据
}

void SPI_Only_WriteByte(SPI_TypeDef *SPIx, u8 Byte)
{
   while ((SPIx->SR & SPI_I2S_FLAG_TXE) == RESET)
       ;            //等待发送区空
   SPIx->DR = Byte; //发送一个byte

   for (int i = 0; i < 15; i++)
       ;
}

void SPI_SetSpeed(SPI_TypeDef *SPIx, u8 SpeedSet)
{
   SPIx->CR1 &= 0XFFC7;
   if (SpeedSet == 1) //高速
   {
       SPIx->CR1 |= SPI_BaudRatePrescaler_2; // Fsck=Fpclk/2
   }
   else //低速
   {
       SPIx->CR1 |= SPI_BaudRatePrescaler_32; // Fsck=Fpclk/32
   }
   SPIx->CR1 |= 1 << 6; // SPI设备使能
}

// SPI1速度设置函数
// SPI速度=fAPB2/分频系数
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
// fAPB2时钟一般为84Mhz：
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
   SPI1->CR1 &= 0XFFC7;                //位3-5清零，用来设置波特率
   SPI1->CR1 |= SPI_BaudRatePrescaler; //设置SPI1速度
   SPI_Cmd(SPI1, ENABLE);              //使能SPI1
}

// SPI1 读写一个字节
// TxData:要写入的字节
//返回值:读取到的字节
u8 SPI1_ReadWriteByte(u8 TxData)
{

   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
       ; //等待发送区空

   SPI_I2S_SendData(SPI1, TxData); //通过外设SPIx发送一个byte  数据

   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
       ; //等待接收完一个byte

   return SPI_I2S_ReceiveData(SPI1); //返回通过SPIx最近接收的数据
}

void SPI1_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   SPI_InitTypeDef SPI_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //使能SPI1时钟

   GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); // PB3复用为 SPI1
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); // PB4复用为 SPI1
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); // PB5复用为 SPI1

   // GPIOFB3,4,5初始化设置
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; // PB3~5复用功能输出
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                        //复用功能
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                  // 100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //上拉
   GPIO_Init(GPIOB, &GPIO_InitStructure);                              //初始化

   //这里只针对SPI口初始化
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);  //复位SPI1
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE); //停止复位SPI1

   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //设置SPI工作模式:设置为主SPI
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //设置SPI的数据大小:SPI发送接收8位帧结构
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                        //串行同步时钟的空闲状态为高电平
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                       //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //定义波特率预分频的值:波特率预分频值为256
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
   SPI_InitStructure.SPI_CRCPolynomial = 7;                           // CRC值计算的多项式
   SPI_Init(SPI1, &SPI_InitStructure);                                //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
   SPI_Cmd(SPI1, ENABLE);                                             //使能SPI外设
}

void SPI2_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   SPI_InitTypeDef SPI_InitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //使能SPI2时钟

   // GPIOFB3,4,5初始化设置
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; // PB3~5复用功能输出
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                           //复用功能
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                         //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                     // 100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //上拉
   GPIO_Init(GPIOB, &GPIO_InitStructure);                                 //初始化

   GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); // PB3复用为 SPI2
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); // PB4复用为 SPI2
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); // PB5复用为 SPI2

   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //设置SPI工作模式:设置为主SPI
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //设置SPI的数据大小:SPI发送接收8位帧结构
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //串行同步时钟的空闲状态为高电平
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //定义波特率预分频的值:波特率预分频值为256
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
   SPI_InitStructure.SPI_CRCPolynomial = 0;                           // CRC值计算的多项式
   SPI_Init(SPI2, &SPI_InitStructure);                                //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

   SPI_CalculateCRC(SPI2, DISABLE);
   SPI_ClearFlag(SPI2, SPI_FLAG_TXE);
   SPI_ClearFlag(SPI2, SPI_FLAG_RXNE);
   SPI_Cmd(SPI2, ENABLE); //使能SPI外设
}

void SPI3_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   SPI_InitTypeDef SPI_InitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE); //使能SPI3时钟

   // GPIOFB3,4,5初始化设置
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; // PB3~5复用功能输出
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                           //复用功能
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                         //推挽输出
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                     // 100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //上拉
   GPIO_Init(GPIOC, &GPIO_InitStructure);                                 //初始化

   GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3); // PB3复用为 SPI3
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3); // PB4复用为 SPI3
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3); // PB5复用为 SPI3

   //这里只针对SPI口初始化
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);  //复位SPI3
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, DISABLE); //停止复位SPI3

   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //设置SPI工作模式:设置为主SPI
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //设置SPI的数据大小:SPI发送接收8位帧结构
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //串行同步时钟的空闲状态为高电平
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       //串行同步时钟的第二个跳变沿（上升或下降）数据被采样
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //定义波特率预分频的值:波特率预分频值为256
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
   SPI_InitStructure.SPI_CRCPolynomial = 0;                           // CRC值计算的多项式
   SPI_Init(SPI3, &SPI_InitStructure);                                //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器

   SPI_CalculateCRC(SPI3, DISABLE);
   SPI_ClearFlag(SPI3, SPI_FLAG_TXE);
   SPI_ClearFlag(SPI3, SPI_FLAG_RXNE);
   SPI_Cmd(SPI3, ENABLE); //使能SPI外设
   SPI_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
}

#endif
