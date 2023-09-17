#ifndef _DMA_H
#define _DMA_H
void Init_DMA(DMA_Stream_TypeDef *DMA_Streamx,u32 chx,u32 par,u32 mar,u16 ndtr);
void DMA_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);

#endif