#ifndef AD7606_H_
#define AD7606_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"
#include "byyc_gpio.h"
#include "byyc_spi.h"
#include "byyc_dma.h"
#include "sys.h"
#include "arm_math.h"
#define ad7606_td_num 2  //开启通道数量,通过此处设置
#define ad7606_size 2048    //采样数量,通过此处设置

#define ad7606_yong ad7606_size
#define ad7606_num (ad7606_size)
#define ad7606_start 5

#define ad7606_td ad7606_td_num  //开启通道数量
#define adc7606_data adc_data    //采样数量
typedef struct adc_data_struct
{
int num;  // adc数据通道
int size; // adc单通道数据量
short *data;
} adc_data_struct;

extern  char ad7606_value_original[ad7606_num][ad7606_td][2];
extern int16_t ad7606_value[ad7606_yong][ad7606_td];
extern float32_t ad7606_value_float32[ad7606_num][ad7606_td];

extern int sampling_tx_flag;
extern adc_data_struct adc_use_app_data;
extern unsigned char data_conver_ok_flag;

extern unsigned char ad7606_continue_flag;

extern unsigned char ad7606_value_tx_ok_flag;
extern unsigned char spi_rx_ok_flag;

extern int sampling_period;
extern int sampling_mag;
extern int sampling_datum;
#define AD_RESET_LOW() PEout(4) = 0
#define AD_RESET_HIGH() PEout(4) = 1

#define AD_CONVST_LOW() PEout(5) = 0
#define AD_CONVST_HIGH() PEout(5) = 1

#define AD_RANGE_5V() PEout(6) = 0
#define AD_RANGE_10V() PEout(6) = 1

#define AD_OS0_0() PCout(0) = 0
#define AD_OS0_1() PCout(0) = 1

#define AD_OS1_0() PCout(1) = 0
#define AD_OS1_1() PCout(1) = 1

#define AD_OS2_0() PCout(2) = 0
#define AD_OS2_1() PCout(2) = 1

#define AD_CS_LOW() PCout(3) = 0
#define AD_CS_HIGH() PCout(3) = 1


// void gpio_rcc_en(int x, ...);           //指定时钟使能
// void gpio_rcc_dis(int x, ...);          //指定时钟失能

#define busy_is_1() (PCin(13) == 1)
#define busy_is_0() (PCin(13) == 0)
void AD7606_Init_Synthesis(void);
void Init_EXTI_for_AD7606(void);
void ad7606_init(void);
void ad7606_StartRecord(void);
short ad7606_read(void);
void real_adc_value(void);
void TIM2_AD7606_Init(u16 arr, u16 psc);

void ad7606_Reset(void);
void ad7606_SetOS(uint8_t _ucMode);
void ad7606_StartConv(void);
unsigned char ad7606_read_bety(void);
void Debug_AD7606_Value(void);
void Debug_AD7606_Float32_Value(void);
void Debug_AD7606_value_original(void);
void Debug_AD7606_Value_Chanel(uint8_t Chanel);
void Debug_AD7606_Float32_Value_Chanel(uint8_t Chanel);
void Debug_AD7606_value_original_Chanel(uint8_t Chanel);
void real_adc_value(void);

#ifdef __cplusplus
}
#endif

#endif
