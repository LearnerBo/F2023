#ifndef _BYYC_GPIO_H_
#define _BYYC_GPIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

   // void gpio_other_en();                   //特殊引脚使能
   // void gpio_other_dis();                  //特殊引脚失能

   void Pin_PP_Out(GPIO_TypeDef *GPIOx, unsigned char Pin, unsigned char dp); //引脚x推挽输出y电平
   uint16_t Pin_InUp(GPIO_TypeDef *GPIOx, uint16_t Pin);                      //读取GPIOx的pin口上拉输入的电平
   uint16_t Pin_InDown(GPIO_TypeDef *GPIOx, uint16_t Pin);                    //读取GPIOx的pin口下拉输入的电平

//初始化具体IO口推挽输出指定电平（最好用于第一次输出，后面用位带操作，更节省时间）
#define paout(pin, dp) Pin_PP_Out(GPIOA, pin, dp)
#define pbout(pin, dp) Pin_PP_Out(GPIOB, pin, dp)
#define pcout(pin, dp) Pin_PP_Out(GPIOC, pin, dp)
#define pdout(pin, dp) Pin_PP_Out(GPIOD, pin, dp)
#define peout(pin, dp) Pin_PP_Out(GPIOE, pin, dp)
#define pfout(pin, dp) Pin_PP_Out(GPIOF, pin, dp)
#define pgout(pin, dp) Pin_PP_Out(GPIOG, pin, dp)

//读取指定IO口上拉输入电平（最好用于第一次输入，后面用位带操作，更节省时间）
#define painu(pin) Pin_InUp(GPIOA, pin)
#define pbinu(pin) Pin_InUp(GPIOB, pin)
#define pcinu(pin) Pin_InUp(GPIOC, pin)
#define pdinu(pin) Pin_InUp(GPIOD, pin)
#define peinu(pin) Pin_InUp(GPIOE, pin)
#define pfinu(pin) Pin_InUp(GPIOF, pin)
#define pginu(pin) Pin_InUp(GPIOG, pin)

//读取指定IO口下拉输入电平（最好用于第一次输入，后面用位带操作，更节省时间）
#define paind(pin) Pin_InDown(GPIOA, pin)
#define pbind(pin) Pin_InDown(GPIOB, pin)
#define pcind(pin) Pin_InDown(GPIOC, pin)
#define pdind(pin) Pin_InDown(GPIOD, pin)
#define peind(pin) Pin_InDown(GPIOE, pin)
#define pfind(pin) Pin_InDown(GPIOF, pin)
#define pgind(pin) Pin_InDown(GPIOG, pin)

   // void gpio_rcc_en(int x, ...);           //指定时钟使能
   // void gpio_rcc_dis(int x, ...);          //指定时钟失能

#ifdef __cplusplus
}
#endif

#endif
