#ifndef _BYYC_SPI_H_
#define _BYYC_SPI_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "sys.h"
#include "byyc_gpio.h"
#include "stm32f4xx_spi.h"

#define spi_software_flag 0
 
#if spi_software_flag

#define SPI_MOSI_SET PBout(12) = 1 //数据线
#define SPI_MOSI_CLR PBout(12) = 0

#define SPI_SCLK_SET PBout(14) = 1 //时钟线
#define SPI_SCLK_CLR PBout(14) = 0

   void spi_init(void);
   void spi_write_byte(u8 data);

#else

u8 SPI_WriteByte(SPI_TypeDef *SPIx, u8 Byte);
void SPI_Only_WriteByte(SPI_TypeDef *SPIx, u8 Byte);
u8 SPI1_ReadWriteByte(u8 TxData);

void SPI1_Init(void);
void SPI2_Init(void);
void SPI3_Init(void);

void SPI_SetSpeed(SPI_TypeDef *SPIx, u8 SpeedSet);
void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler);

#endif

#ifdef __cplusplus
}
#endif

#endif
