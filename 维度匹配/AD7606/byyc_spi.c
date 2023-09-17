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
           SPI_MOSI_SET; //�������
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
       ;            //�ȴ���������
   SPIx->DR = Byte; //����һ��byte

   while ((SPIx->SR & SPI_I2S_FLAG_RXNE) == RESET)
       ;            //�ȴ�������һ��byte
   return SPIx->DR; //�����յ�������
}

void SPI_Only_WriteByte(SPI_TypeDef *SPIx, u8 Byte)
{
   while ((SPIx->SR & SPI_I2S_FLAG_TXE) == RESET)
       ;            //�ȴ���������
   SPIx->DR = Byte; //����һ��byte

   for (int i = 0; i < 15; i++)
       ;
}

void SPI_SetSpeed(SPI_TypeDef *SPIx, u8 SpeedSet)
{
   SPIx->CR1 &= 0XFFC7;
   if (SpeedSet == 1) //����
   {
       SPIx->CR1 |= SPI_BaudRatePrescaler_2; // Fsck=Fpclk/2
   }
   else //����
   {
       SPIx->CR1 |= SPI_BaudRatePrescaler_32; // Fsck=Fpclk/32
   }
   SPIx->CR1 |= 1 << 6; // SPI�豸ʹ��
}

// SPI1�ٶ����ú���
// SPI�ٶ�=fAPB2/��Ƶϵ��
//@ref SPI_BaudRate_Prescaler:SPI_BaudRatePrescaler_2~SPI_BaudRatePrescaler_256
// fAPB2ʱ��һ��Ϊ84Mhz��
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
   SPI1->CR1 &= 0XFFC7;                //λ3-5���㣬�������ò�����
   SPI1->CR1 |= SPI_BaudRatePrescaler; //����SPI1�ٶ�
   SPI_Cmd(SPI1, ENABLE);              //ʹ��SPI1
}

// SPI1 ��дһ���ֽ�
// TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 TxData)
{

   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET)
       ; //�ȴ���������

   SPI_I2S_SendData(SPI1, TxData); //ͨ������SPIx����һ��byte  ����

   while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET)
       ; //�ȴ�������һ��byte

   return SPI_I2S_ReceiveData(SPI1); //����ͨ��SPIx������յ�����
}

void SPI1_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   SPI_InitTypeDef SPI_InitStructure;

   RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //ʹ��SPI1ʱ��

   GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_SPI1); // PB3����Ϊ SPI1
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_SPI1); // PB4����Ϊ SPI1
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_SPI1); // PB5����Ϊ SPI1

   // GPIOFB3,4,5��ʼ������
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5; // PB3~5���ù������
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                        //���ù���
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                      //�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                  // 100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                        //����
   GPIO_Init(GPIOB, &GPIO_InitStructure);                              //��ʼ��

   //����ֻ���SPI�ڳ�ʼ��
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, ENABLE);  //��λSPI1
   RCC_APB2PeriphResetCmd(RCC_APB2Periph_SPI1, DISABLE); //ֹͣ��λSPI1

   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //����SPI����ģʽ:����Ϊ��SPI
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;                        //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;                       //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
   SPI_InitStructure.SPI_CRCPolynomial = 7;                           // CRCֵ����Ķ���ʽ
   SPI_Init(SPI1, &SPI_InitStructure);                                //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
   SPI_Cmd(SPI1, ENABLE);                                             //ʹ��SPI����
}

void SPI2_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   SPI_InitTypeDef SPI_InitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE); //ʹ��SPI2ʱ��

   // GPIOFB3,4,5��ʼ������
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15; // PB3~5���ù������
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                           //���ù���
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                         //�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                     // 100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //����
   GPIO_Init(GPIOB, &GPIO_InitStructure);                                 //��ʼ��

   GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_SPI2); // PB3����Ϊ SPI2
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_SPI2); // PB4����Ϊ SPI2
   GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_SPI2); // PB5����Ϊ SPI2

   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //����SPI����ģʽ:����Ϊ��SPI
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
   SPI_InitStructure.SPI_CRCPolynomial = 0;                           // CRCֵ����Ķ���ʽ
   SPI_Init(SPI2, &SPI_InitStructure);                                //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

   SPI_CalculateCRC(SPI2, DISABLE);
   SPI_ClearFlag(SPI2, SPI_FLAG_TXE);
   SPI_ClearFlag(SPI2, SPI_FLAG_RXNE);
   SPI_Cmd(SPI2, ENABLE); //ʹ��SPI����
}

void SPI3_Init(void)
{
   GPIO_InitTypeDef GPIO_InitStructure;
   SPI_InitTypeDef SPI_InitStructure;

   RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE); //ʹ��SPI3ʱ��

   // GPIOFB3,4,5��ʼ������
   GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11 | GPIO_Pin_12; // PB3~5���ù������
   GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;                           //���ù���
   GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;                         //�������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;                     // 100MHz
   GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;                           //����
   GPIO_Init(GPIOC, &GPIO_InitStructure);                                 //��ʼ��

   GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_SPI3); // PB3����Ϊ SPI3
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_SPI3); // PB4����Ϊ SPI3
   GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_SPI3); // PB5����Ϊ SPI3

   //����ֻ���SPI�ڳ�ʼ��
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);  //��λSPI3
   // RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, DISABLE); //ֹͣ��λSPI3

   SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex; //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��
   SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                      //����SPI����ģʽ:����Ϊ��SPI
   SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                  //����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
   SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //����ͬ��ʱ�ӵĿ���״̬Ϊ�ߵ�ƽ
   SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                       //����ͬ��ʱ�ӵĵڶ��������أ��������½������ݱ�����
   SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                          // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
   SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2; //���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ256
   SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
   SPI_InitStructure.SPI_CRCPolynomial = 0;                           // CRCֵ����Ķ���ʽ
   SPI_Init(SPI3, &SPI_InitStructure);                                //����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���

   SPI_CalculateCRC(SPI3, DISABLE);
   SPI_ClearFlag(SPI3, SPI_FLAG_TXE);
   SPI_ClearFlag(SPI3, SPI_FLAG_RXNE);
   SPI_Cmd(SPI3, ENABLE); //ʹ��SPI����
   SPI_DMACmd(SPI3, SPI_I2S_DMAReq_Rx, ENABLE);
}

#endif
