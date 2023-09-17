#include "byyc_gpio.h"
#include <stdarg.h>
#include "sys.h"
//����Pin�������dp��ƽ
void Pin_PP_Out(GPIO_TypeDef *GPIOx, unsigned char Pin, unsigned char dp)
{
   GPIO_InitTypeDef GPIO_InitStruct;

   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;     //����Ϊ���
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    //����Ϊ����ģʽ
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 << Pin;  //����������ſ�
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�����ٶ�
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;  //������������������Ӧ��Ҳ�У���֪���ڲ�������ôʵ�֣���������ģʽֱ�������˵Ļ���������ν��û�����εĻ�ҲΪ��ν���ӵ�������������Ӱ�첻���ر�󣬵�Ϊ��ȷ������Ӱ�죬ֱ��ȡ����������
   GPIO_Init(GPIOx, &GPIO_InitStruct);            //����������������GPIO�ڵ�ģʽ

   if (GPIOx == GPIOA)
   {
       PAout(Pin) = dp;
   }

   else if (GPIOx == GPIOB)
   {
       PBout(Pin) = dp;
   }

   else if (GPIOx == GPIOC)
   {
       PCout(Pin) = dp;
   }

   else if (GPIOx == GPIOD)
   {
       PDout(Pin) = dp;
   }

   else if (GPIOx == GPIOE)
   {
       PEout(Pin) = dp;
   }

   else if (GPIOx == GPIOF)
   {
       PFout(Pin) = dp;
   }

   else if (GPIOx == GPIOG)
   {
       PGout(Pin) = dp;
   }
}

// GPIOx:ȷ������һ��IO�ڣ�xΪ��A-G��
// Pin:����IO��
//��ȡGPIOx��Pin����������ĵ�ƽ
uint16_t Pin_InUp(GPIO_TypeDef *GPIOx, uint16_t Pin)
{
   uint16_t in;
   GPIO_InitTypeDef GPIO_Initstruct;

   GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;     //����Ϊ����
   GPIO_Initstruct.GPIO_Pin = GPIO_Pin_0 << Pin; //ָ������
   GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_UP;     //��Ϊ����
   GPIO_Initstruct.GPIO_Speed = GPIO_High_Speed; //�����ٶ�
   GPIO_Init(GPIOx, &GPIO_Initstruct);           //��ʼ��IO��

   for (int j = 99; j > 0; j--)
       ;

   if (GPIOx == GPIOA)
   {
       in = PAin(Pin);
   }

   else if (GPIOx == GPIOB)
   {
       in = PBin(Pin);
   }

   else if (GPIOx == GPIOC)
   {
       in = PCin(Pin);
   }

   else if (GPIOx == GPIOD)
   {
       in = PDin(Pin);
   }

   else if (GPIOx == GPIOE)
   {
       in = PEin(Pin);
   }

   else if (GPIOx == GPIOF)
   {
       in = PFin(Pin);
   }

   else if (GPIOx == GPIOG)
   {
       in = PGin(Pin);
   }

   return in;
}

// GPIOx:ȷ������һ��IO�ڣ�xΪ��A-G��
// Pin:����IO��
//��ȡGPIOx��Pin����������ĵ�ƽ
uint16_t Pin_InDown(GPIO_TypeDef *GPIOx, uint16_t Pin)
{
   uint16_t in;
   GPIO_InitTypeDef GPIO_Initstruct;

   GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;     //����Ϊ����
   GPIO_Initstruct.GPIO_Pin = GPIO_Pin_0 << Pin; //ָ������
   GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_DOWN;   //��Ϊ����
   GPIO_Initstruct.GPIO_Speed = GPIO_Fast_Speed; //�����ٶ�
   GPIO_Init(GPIOx, &GPIO_Initstruct);           //��ʼ��IO��

   for (int j = 99; j > 0; j--)
       ;

   if (GPIOx == GPIOA)
   {
       in = PAin(Pin);
   }

   else if (GPIOx == GPIOB)
   {
       in = PBin(Pin);
   }

   else if (GPIOx == GPIOC)
   {
       in = PCin(Pin);
   }
   else if (GPIOx == GPIOD)
   {
       in = PDin(Pin);
   }

   else if (GPIOx == GPIOE)
   {
       in = PEin(Pin);
   }

   else if (GPIOx == GPIOF)
   {
       in = PFin(Pin);
   }

   else if (GPIOx == GPIOG)
   {
       in = PGin(Pin);
   }

   return in;
}

// IO�������������ģʽ,����ת��ƽ
void gpio_rolling_over(GPIO_TypeDef *GPIOx, uint16_t pin)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   int start = 0;

   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;     //����Ϊ���
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    //����Ϊ����ģʽ
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 << pin;  //����������ſ�
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //�����ٶ�
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;  //������������������Ӧ��Ҳ�У���֪���ڲ�������ôʵ�֣���������ģʽֱ�������˵Ļ���������ν��û�����εĻ�ҲΪ��ν���ӵ�������������Ӱ�첻���ر�󣬵�Ϊ��ȷ������Ӱ�죬ֱ��ȡ����������
   GPIO_Init(GPIOx, &GPIO_InitStruct);            //����������������GPIO�ڵ�ģʽ

   start = GPIOx->ODR;
   start = (start >> pin) & 1;

   if (start == 1)
       GPIOx->ODR = GPIOx->ODR | (1u << pin);
   else
       GPIOx->ODR &= ~(1u << pin);
}

