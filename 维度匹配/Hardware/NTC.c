#include "NTC.h"

//�ο�����;https://www.cnblogs.com/ct20150811/p/11276595.html

/**
  ******************************************************************************
  * �ļ�����: NTC.c 
  * ��    ��: ����
  * ��    ��: V1.0
  * ��д����: 2021/10/06
  * ��    ��: ��ȡ����������¶�
  ******************************************************************************
  * ˵����
  *       ͨ����ѹ����������������һ��10K�ĵ��贮����Ȼ�����3.3V�ĵ�Դ��
	* ͨ��ADCȥ��ȡ�����������˵ĵ�ѹ��Ȼ�����������������ֵ��ͨ����ֵ-�¶ȱ�
	* ȥ��ѯ�¶ȡ�
	* ע�⣺
	        ����������������ϵ��Ϊ3950�ĸ��¶�ϵ��10K��������
  ******************************************************************************
  */
uint32_t RT_Table[131]={
0xEE3BB,0xE0C14,0xD41FD,0xC8488,0xBD2D7,0xB2C1C,0xA8F9B,0x9FCA3,0x97292,0x8F0CE,//-20--11
0x876CD,0x8042A,0xAA552,0x7326E,0x6D2B7,0x6788E,0x62396,0x5D37C,0x587F1,0x540AD,//-10--1
0x4FD6B,0x4BDEC,0x481F4,0x4494C,0x413C1,0x3E122,0x3B143,0x383FA,0x3591E,0x3308C,//0-9
0x30A27,0x2E5BC,0x2C33F,0x2A290,0x28392,0x2662D,0x24A49,0x22FD2,0x216B2,0x1FED6,//10-19
0x1E82D,0x1D2A5,0x1BE2E,0x1AAB9,0x19839,0x186A0,0x175E1,0x165F1,0x155C4,0x14852,//20-29
0x13A8F,0x12D72,0x120F3,0x1150A,0x109AE,0xFED9,0xF483,0xEAA6,0xE13D,0xD840,//30-39
0xCFAB,0xC779,0xBFA4,0xB828,0xB101,0xAA2A,0xA39F,0x9D5E,0x9761,0x91A7,//40-49
0x8C2C,0x86EC,0x81E5,0x7D15,0x7878,0x740D,0x6FED,0x6BC2,0x67DD,0x6421,//50-59
0x608D,0x5D1D,0x59D1,0x56A7,0x539E,0x50B3,0x4DE6,0x4B35,0x48A0,0x4624,//60-69
0x43C1,0x4175,0x3F41,0x3D21,0x3B17,0x3921,0x373D,0x356C,0x33AC,0x31FD,//70-79
0x305E,0x2ECE,0x2D4D,0x2BDB,0x2A75,0x291D,0x27D1,0x2691,0x255D,0x2433,//80-89
02314,0x2200,0x20F5,0x1FF3,0x1EFB,0x1E0B,0x1D23,0x1C44,0x1B6C,0x1A9B, //90-99
0x19D2,0x190F,0x1853,0x179D,0x16EE,0x1644,0x15A0,0x1501,0x1468,0x13D3,0x1343
};//100-109

u32 Get_Res(u8 ADC_channel)//��ȡNTC�ĵ���ֵ
{
	u32 adc_value=Get_Adc(ADC_channel);
	u32 R_Value,V_Value;
	V_Value=(3300*adc_value)/4095;
	R_Value=(100000*V_Value)/(3300-V_Value);
	return R_Value;
}

u16 Get_ADC(u8 ADC_channel)//��ȡADCͨ����ADCֵ
{
	  u32 ADC_Value=0;
	  ADC_Value =Get_Adc(ADC_channel);
		return ADC_Value;
}
u8 Get_Temp(u8 ADC_channel)//��ȡ�¶ȣ�������������
{
	u32 adc_value=Get_Res(ADC_channel);
	u8 low=0,high=131,mid;//value,

	while(low<=high){
		mid=(low+high)/2;
		if(RT_Table[mid]==adc_value) break;			
				
		if(adc_value<RT_Table[mid]) low=mid+1;//ֵС����ֵ,����λ�ұ�,low �ƶ�					
		
		if(adc_value>RT_Table[mid]) high=mid-1;	//ֵ������ֵ,����λ���,High �ƶ�	
				
	}
	if(adc_value<RT_Table[mid]) mid=mid+1;//

	return mid-20;
}

double Get_Temp_Precise(u8 ADC_channel)//��ȡ�¶ȣ�ͨ�������-�¶ȱ���
{
  	u32 adc_value=Get_Res(ADC_channel);
    float temp_decimal;
    u32 TpValue;
	  u8 low=0,high=131,mid;//value,

	while(low<=high){
		mid=(low+high)/2;
		//�½�����
		if(RT_Table[mid]==adc_value) break;					
		if(adc_value<RT_Table[mid]) low=mid+1;//ֵС����ֵ,����λ�ұ�,low �ƶ�					
		if(adc_value>RT_Table[mid]) high=mid-1;	//ֵ������ֵ,����λ���,High �ƶ�	
	}
	if(adc_value<RT_Table[mid]){ 
        temp_decimal=mid+((adc_value-RT_Table[mid+1])/(RT_Table[mid]-RT_Table[mid+1]));
    }else{
        temp_decimal=mid-1+((adc_value-RT_Table[mid])/(RT_Table[mid-1]-RT_Table[mid]));
    }
    
    TpValue=((temp_decimal-20)*10);
    return (TpValue/10.0);
}

/**********************************************
 *     ��ȡ�߾����¶ȣ���Ҫ�õ��������㣬����
 * ����Ҫ�õ��Ϸ����¶�-��ֵ�����ܻ���һ����
 * ���ȶ�ʧ��
 * 
***********************************************/
double Get_Temp_PJL(u8 ADC_channel)
{
	u16 adc_value=Get_ADC(ADC_channel);
	double R_Value,T_Value;
	double B_Value=3950.0,//NTCϵ��
		     R0_Value=10000.0;//��������25���϶��µ���ֵ
	R_Value=(adc_value*R0_Value/(4096.0-adc_value));
	T_Value=1.0/((1.0/298.15)+((log(R_Value/R0_Value)/log(2.71828))/B_Value));
	return T_Value-273.15;
}
