#ifndef _Kalman_H
#define _Kalman_H
#include "AD7606.h"
#include "arm_math.h"
#include "stdlib.h"
#define Length_Samples ad7606_num  /* ��������  */
#define Block_Size ad7606_num      /*  ����һ��arm_fir_f32 ����Ĳ�������� */
#define Num_Taps  81       /*  �˲���ϵ���ĸ��� */

typedef struct 
{
    float Last_P;//�ϴι����Э���������Ϊ0! ! ! ! ! 
    float Now_P;//��ǰ�����Э����
    float out;//��ǰ��������Ž⣬���˲�������
    float Kg;//��ǰ�Ŀ���������
    float Q;//����������Э����
    float R;//�۲�������Э����
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
