#ifndef _BYYC_GPIO_H_
#define _BYYC_GPIO_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "stm32f4xx_rcc.h"
#include "stm32f4xx_gpio.h"

   // void gpio_other_en();                   //��������ʹ��
   // void gpio_other_dis();                  //��������ʧ��

   void Pin_PP_Out(GPIO_TypeDef *GPIOx, unsigned char Pin, unsigned char dp); //����x�������y��ƽ
   uint16_t Pin_InUp(GPIO_TypeDef *GPIOx, uint16_t Pin);                      //��ȡGPIOx��pin����������ĵ�ƽ
   uint16_t Pin_InDown(GPIO_TypeDef *GPIOx, uint16_t Pin);                    //��ȡGPIOx��pin����������ĵ�ƽ

//��ʼ������IO���������ָ����ƽ��������ڵ�һ�������������λ������������ʡʱ�䣩
#define paout(pin, dp) Pin_PP_Out(GPIOA, pin, dp)
#define pbout(pin, dp) Pin_PP_Out(GPIOB, pin, dp)
#define pcout(pin, dp) Pin_PP_Out(GPIOC, pin, dp)
#define pdout(pin, dp) Pin_PP_Out(GPIOD, pin, dp)
#define peout(pin, dp) Pin_PP_Out(GPIOE, pin, dp)
#define pfout(pin, dp) Pin_PP_Out(GPIOF, pin, dp)
#define pgout(pin, dp) Pin_PP_Out(GPIOG, pin, dp)

//��ȡָ��IO�����������ƽ��������ڵ�һ�����룬������λ������������ʡʱ�䣩
#define painu(pin) Pin_InUp(GPIOA, pin)
#define pbinu(pin) Pin_InUp(GPIOB, pin)
#define pcinu(pin) Pin_InUp(GPIOC, pin)
#define pdinu(pin) Pin_InUp(GPIOD, pin)
#define peinu(pin) Pin_InUp(GPIOE, pin)
#define pfinu(pin) Pin_InUp(GPIOF, pin)
#define pginu(pin) Pin_InUp(GPIOG, pin)

//��ȡָ��IO�����������ƽ��������ڵ�һ�����룬������λ������������ʡʱ�䣩
#define paind(pin) Pin_InDown(GPIOA, pin)
#define pbind(pin) Pin_InDown(GPIOB, pin)
#define pcind(pin) Pin_InDown(GPIOC, pin)
#define pdind(pin) Pin_InDown(GPIOD, pin)
#define peind(pin) Pin_InDown(GPIOE, pin)
#define pfind(pin) Pin_InDown(GPIOF, pin)
#define pgind(pin) Pin_InDown(GPIOG, pin)

   // void gpio_rcc_en(int x, ...);           //ָ��ʱ��ʹ��
   // void gpio_rcc_dis(int x, ...);          //ָ��ʱ��ʧ��

#ifdef __cplusplus
}
#endif

#endif
