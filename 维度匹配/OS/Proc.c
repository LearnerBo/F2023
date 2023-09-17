#include "Proc.h"
uint8_t SysTick_5ms;
uint8_t SysTick_500ms;
uint8_t SysTick_100ms;
uint8_t SysTick_Mode_Rec_100ms;
uint8_t SysTick_Loc_500ms;
uint8_t SysTick_50ms;
uint8_t SysTick_25ms;
uint8_t SysTick_5ms;
uint8_t SysTick_1s;
uint8_t SysTick_Signal_1s;
uint8_t SysTick_Location_500ms;

uint8_t SysTick_Display_500ms;
uint8_t SysTick_2s;
void Per_Task()
{
	static uint32_t systick_cnt;
	static uint32_t systick_cnt2=0;

	if((Capture_Secure_FLAG1==0 )&& (Capture_Secure_FLAG2==0) && (Capture_Secure_FLAG3==0)&& (Capture_Secure_FLAG4==0) )//
	{
		if(systick_flag2 == 1)
		{
		systick_flag2 = 0;
		systick_cnt2 ++;
		if(systick_cnt2 == 500)
		{
			Capture_Secure_FLAG1=1;
			Capture_Secure_FLAG2=1;
			Capture_Secure_FLAG3=1;
			Capture_Secure_FLAG4=1;
			systick_cnt2 = 0;
		}
		}
		
	}

	if (systick_flag == 1)
	{
		systick_flag = 0;
		systick_cnt++;
		if(systick_cnt % 100 == 3)
		{
			SysTick_Mode_Rec_100ms = 1;
		}
		if (systick_cnt % 5 == 1)
		{
			SysTick_5ms = 1;
		}
		if (systick_cnt % 1000 == 500)
		{
			SysTick_1s = 1;
		}
		if (systick_cnt % 1000 == 10)
		{
			SysTick_Signal_1s = 1;
		}
		if(systick_cnt % 500 == 30)
		{
			SysTick_Location_500ms = 1;
		}
		
		if (systick_cnt % 2000 == 2)
		{
			SysTick_2s = 1;
		}
		if(systick_cnt %500 == 50)
		{
			SysTick_Loc_500ms = 1;
		}
		if (systick_cnt % 500 == 20)
		{
			SysTick_500ms = 1;
		}
		if(systick_cnt % 500 == 1)
		{
			SysTick_Display_500ms = 1;
		}
		if (systick_cnt % 100 == 1)
		{
			SysTick_100ms = 1;
		}
		if (systick_cnt % 50 == 5)
		{
			SysTick_50ms = 1;
		}
		if (systick_cnt % 20 == 5)
		{
			SysTick_25ms = 1;
		}
		if (systick_cnt % 5 == 4)
		{
			SysTick_5ms = 1;
		}
		
	}
}