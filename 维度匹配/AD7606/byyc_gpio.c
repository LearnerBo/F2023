#include "byyc_gpio.h"
#include <stdarg.h>
#include "sys.h"
//引脚Pin推挽输出dp电平
void Pin_PP_Out(GPIO_TypeDef *GPIOx, unsigned char Pin, unsigned char dp)
{
   GPIO_InitTypeDef GPIO_InitStruct;

   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;     //设置为输出
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    //设置为推挽模式
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 << Pin;  //设置输出引脚口
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置速度
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;  //不用上下拉（不配置应该也行，不知道内部具体怎么实现，如果在输出模式直接屏蔽了的话，就无所谓，没有屏蔽的话也为所谓，加的是弱上下拉，影响不是特别大，但为了确保消除影响，直接取消上下拉）
   GPIO_Init(GPIOx, &GPIO_InitStruct);            //根据上面数据配置GPIO口的模式

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

// GPIOx:确定是哪一组IO口，x为（A-G）
// Pin:具体IO口
//读取GPIOx的Pin口上拉输入的电平
uint16_t Pin_InUp(GPIO_TypeDef *GPIOx, uint16_t Pin)
{
   uint16_t in;
   GPIO_InitTypeDef GPIO_Initstruct;

   GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;     //设置为输入
   GPIO_Initstruct.GPIO_Pin = GPIO_Pin_0 << Pin; //指定引脚
   GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_UP;     //设为上拉
   GPIO_Initstruct.GPIO_Speed = GPIO_High_Speed; //设置速度
   GPIO_Init(GPIOx, &GPIO_Initstruct);           //初始化IO口

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

// GPIOx:确定是哪一组IO口，x为（A-G）
// Pin:具体IO口
//读取GPIOx的Pin口下拉输入的电平
uint16_t Pin_InDown(GPIO_TypeDef *GPIOx, uint16_t Pin)
{
   uint16_t in;
   GPIO_InitTypeDef GPIO_Initstruct;

   GPIO_Initstruct.GPIO_Mode = GPIO_Mode_IN;     //设置为输入
   GPIO_Initstruct.GPIO_Pin = GPIO_Pin_0 << Pin; //指定引脚
   GPIO_Initstruct.GPIO_PuPd = GPIO_PuPd_DOWN;   //设为下拉
   GPIO_Initstruct.GPIO_Speed = GPIO_Fast_Speed; //设置速度
   GPIO_Init(GPIOx, &GPIO_Initstruct);           //初始化IO口

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

// IO口配置推挽输出模式,并翻转电平
void gpio_rolling_over(GPIO_TypeDef *GPIOx, uint16_t pin)
{
   GPIO_InitTypeDef GPIO_InitStruct;
   int start = 0;

   GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;     //设置为输出
   GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    //设置为推挽模式
   GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0 << pin;  //设置输出引脚口
   GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz; //设置速度
   GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;  //不用上下拉（不配置应该也行，不知道内部具体怎么实现，如果在输出模式直接屏蔽了的话，就无所谓，没有屏蔽的话也为所谓，加的是弱上下拉，影响不是特别大，但为了确保消除影响，直接取消上下拉）
   GPIO_Init(GPIOx, &GPIO_InitStruct);            //根据上面数据配置GPIO口的模式

   start = GPIOx->ODR;
   start = (start >> pin) & 1;

   if (start == 1)
       GPIOx->ODR = GPIOx->ODR | (1u << pin);
   else
       GPIOx->ODR &= ~(1u << pin);
}

