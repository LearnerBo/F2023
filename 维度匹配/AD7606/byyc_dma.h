#ifndef _BYYC_DMA_H_
#define _BYYC_DMA_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_dma.h"
#include "misc.h"

    void dma_init_mem_to_Peripheral(DMA_Stream_TypeDef *DMA_Streamx, u32 td, u32 per, u32 mem, u16 size);
    void dma_init_mem_to_mem(DMA_Stream_TypeDef *DMA_Streamx, u32 td, u32 memout, u32 memin, u16 size);
    void dma_init_adc1_to_mem(u32 mem, u16 size);
    void dma_enable(DMA_Stream_TypeDef *DMA_Streamx, u16 size);
    void spi3_rx_dma(u32 mem, u16 size);

#ifdef __cplusplus
}
#endif

#endif
