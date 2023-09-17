#ifndef _Kalman_H
#define _Kalman_H
#include "AD7606.h"
#include "arm_math.h"
#include "stdlib.h"
#define Length_Samples ad7606_num  /* 采样点数  */
#define Block_Size ad7606_num      /*  调用一次arm_fir_f32 处理的采样点个数 */
#define Num_Taps  81       /*  滤波器系数的个数 */

typedef struct 
{
    float Last_P;//上次估算的协方差，不可以为0! ! ! ! ! 
    float Now_P;//当前估算的协方差
    float out;//当前估算的最优解，即滤波后的输出
    float Kg;//当前的卡尔曼增益
    float Q;//过程噪声的协方差
    float R;//观测噪声的协方差
}Kalman;

void Kalman_Init(void);
void Kalman_Init2(void);
float32_t KalmanFilter2(Kalman *kfp2,float32_t input);
float32_t KalmanFilter(Kalman *kfp,float32_t input);
void Filter(float32_t input[],float32_t output[]);
float moving_average(float input);
float exponential_moving_average(float input); 

extern Kalman kfp;
extern Kalman kfp2;
#include "stm32f4xx.h"                  // Device header




//float Kalman_Filter(float R);


#endif
