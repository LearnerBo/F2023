#include "FFT.h"
float FFT_InputBuf[FFT_LENGTH * 2]; // FFT��������
float FFT_OutputBuf[FFT_LENGTH];	// FFT�������
float input[2 * FFT_LENGTH];

arm_cfft_radix2_instance_f32 scfft;

void Init_FFT(u16 FFT_Length)
{
	arm_cfft_radix2_init_f32(&scfft, FFT_Length, 0, 1); // ��ʼ��scfft�ṹ�壬�趨FFT��ز���
}

void Generate_InputArray(float input[], u16 Length)
{
	for (u16 i = 0; i < Length; i++)
	{
		FFT_InputBuf[2 * i] = input[i]; // ʵ������
		FFT_InputBuf[2 * i + 1] = 0;	// ��������
	}
}

/*
@brief ����������±�ֵ������������������
*/

void FFT_GetValue(float input[], float output[], u16 Length)
{
	Init_FFT(Length);
	Generate_InputArray(input, Length);
	arm_cfft_radix2_f32(&scfft, FFT_InputBuf);		 // FFT���㣨��2��
	arm_cmplx_mag_f32(FFT_InputBuf, output, Length); // ��������������ģ�÷�ֵ
}

u16 Max(float input[], u16 Length)
{
	u16 max = 0;
	u16 max_i = 1;
	for (u16 i = 1; i < Length; i++)
	{
		if (max < input[i])
		{
			max = input[i];
			max_i = i;
		}
	}
	return max_i;
}

void test_fft()
{
	u16 i = 0;
	for (i = 0; i < FFT_LENGTH; i++) // ���������ź�
	{
		input[i] = 100 + 10 * arm_sin_f32(2 * PI * i / FFT_LENGTH) + 30 * arm_sin_f32(2 * PI * i * 4 / FFT_LENGTH) + 50 * arm_cos_f32(2 * PI * i * 8 / FFT_LENGTH);
	}
	FFT_GetValue(input, FFT_OutputBuf, FFT_LENGTH);
	//
	//	    arm_cfft_radix4_init_f32(&scfft,FFT_LENGTH,0,1);
	//			for(i=0;i<FFT_LENGTH;i++)//�����ź�����
	//			{
	//				 FFT_InputBuf[2*i]=100+10*arm_sin_f32(2*PI*i/FFT_LENGTH)+30*arm_sin_f32(2*PI*i*4/FFT_LENGTH)+50*arm_cos_f32(2*PI*i*8/FFT_LENGTH);	//���������ź�ʵ��
	//				 FFT_InputBuf[2*i+1]=0;//�鲿ȫ��Ϊ0
	//			}
	//			arm_cfft_radix4_f32(&scfft,FFT_InputBuf);	//FFT���㣨��4��
	//			arm_cmplx_mag_f32(FFT_InputBuf,FFT_OutputBuf,FFT_LENGTH);	//��������������ģ�÷�ֵ
	for (i = 0; i < FFT_LENGTH; i++)
	{
		printf("%f\r\n", FFT_OutputBuf[i]);
	}
}

void arm_rfft_f32_app(void)
{
	float32_t testInput_f32[1024];
	float32_t testOutput_f32[1024];
	float32_t testOutputMag_f32[1024];
	float32_t Phase_f32[1024];
	uint16_t i;
	arm_rfft_fast_instance_f32 S;
	/* ���任 */
	u8 ifftFlag = 0;
	/* ��ʼ���ṹ�� S �еĲ��� */
	arm_rfft_fast_init_f32(&S, 1024);
	for (i = 0; i < 1024; i++)
	{
		/* ��������ֱ��������50Hz ���Ҳ���ɣ����β����� 1024����ʼ��λ 60�� */
		testInput_f32[i] = 1 + cos(2 * 3.1415926f * 50 * i / 1024 + 3.1415926f / 3);
	}
	/* 1024 ��ʵ���п��� FFT */
	arm_rfft_fast_f32(&S, testInput_f32, testOutput_f32, ifftFlag);
	/* Ϊ�˷�������� arm_cfft_f32 ����Ľ�����Աȣ���������� 1024 ��ģֵ��ʵ�ʺ��� arm_rfft_fast_f32
	 ֻ������ 512 ��
	*/
	arm_cmplx_mag_f32(testOutput_f32, testOutputMag_f32, 1024);
	printf("\r\n");
	/* ����Ƶ */
	// PowerPhaseRadians_f32()
	/* ���ڴ�ӡ��ֵ����Ƶ */
	for (i = 0; i < 1024; i++)
	{
		printf("%f, %f\r\n", testOutputMag_f32[i], Phase_f32[i]);
	}
}

// arm_add_f32()
// arm_abs_q15()
// float32_t max_power;
// uint32_t max_power_pos;
// void DATA_Procesess(void)
// {	data_conver_ok_flag = 0;
// 	float32_t input[1024];
// 	for(uint16_t i=0;i<1024;i++)
// 	{
// 	input[i]=ad7606_value_float32[i][0];
// 	}
// 	// arm_max_f32(input,1024,&Read_Wiring_Sum_Power,&max_power_pos);
// 	arm_rms_f32(input,1024,&Read_Wiring_Sum_Power);
// 	// for(uint16_t i=0;i<1024;i++)
// 	// {
// 	// input[i]=ad7606_value_float32[i][1];
// 	// }
// 	data_conver_ok_flag = 1;
// }
float32_t FFT_phase_Pi;
float32_t FFT_phaseDifference;
float32_t FFT_phaseDifference_AB;
/*�źŵ�Ƶ�ʺͲ���Ƶ��
�����С1024
��A�ں�ʱ,��λ��С��180
��B�ں�ʱ,��λ�����180 ==>Ϊ����
������:FFT_phaseDifference
*/
void FFT_Phase_Computation(uint16_t SignalFreq, uint16_t SAMPLE_Rate)
{
	data_conver_ok_flag = 0;
	float32_t inputA[ad7606_num * 2];
	float32_t inputB[ad7606_num * 2];

	for (uint16_t size = 0; size < ad7606_num; size++)
	{
		inputA[2 * size] = ad7606_value_float32[size][0]; // ���ʵ��
		inputA[2 * size + 1] = 0;						  // ����鲿
		inputB[2 * size] = ad7606_value_float32[size][1]; // ���ʵ��
		inputB[2 * size + 1] = 0;						  // ����鲿
	}

	/* Ϊ�����ź�A���ź�B��Ӻ����� */
	// float32_t window[ad7606_num];
	// for (uint16_t i = 0; i < ad7606_num; i++)
	// {
	// 	window[i] = 0.5f * (1.0f - cosf(2.0f * PI * i / (ad7606_num - 1)));
	// 	inputA[2 * i] *= window[i];
	// 	inputB[2 * i] *= window[i];
	// }
	/*����������м���,�ɽ�����ע��ȡ���Ӵ�*/
	// arm_cfft_radix4_instance_f32 fft_inst;
	// arm_cfft_radix4_init_f32(&fft_inst, ad7606_num, 0, 1);
	// /* ���ź�A���ź�Bִ��FFT */
	// arm_cfft_radix4_f32(&fft_inst, inputA);
	// arm_cfft_radix4_f32(&fft_inst, inputB);

	arm_cfft_radix2_instance_f32 fft_inst;
	arm_cfft_radix2_init_f32(&fft_inst, ad7606_num, 0, 1);
	/* ���ź�A���ź�Bִ��FFT */
	arm_cfft_radix2_f32(&fft_inst, inputA);
	arm_cfft_radix2_f32(&fft_inst, inputB);

	/* �ҵ�1kHz��Ӧ������ */
	uint32_t index_1kHz = ad7606_num * SignalFreq / SAMPLE_Rate;

	// uint32_t index_1kHz = 1024 * 1000 / 20000;

	/* ����1kHz������λ�� */
	float32_t phaseA = atan2f(inputA[2 * index_1kHz + 1], inputA[2 * index_1kHz]);
	float32_t phaseB = atan2f(inputB[2 * index_1kHz + 1], inputB[2 * index_1kHz]);
	FFT_phaseDifference = phaseA - phaseB;

	FFT_phase_Pi = FFT_phaseDifference;
	FFT_phaseDifference = round(FFT_phaseDifference / PI * 180);

	if (FFT_phaseDifference < 0)
	{
		FFT_phaseDifference = 360 + FFT_phaseDifference;
	}
	if (FFT_phaseDifference > 180)
	{
		FFT_phaseDifference = -360 + FFT_phaseDifference;
	}
	/* ���1kHz������λ�� */
	// printf("Phase difference at 1kHz: %f\r\n", phaseDifference);
	// printf("%f\r\n", FFT_phaseDifference);
	data_conver_ok_flag = 1;
}

float32_t FFT_frequency_Chanel_0;
float32_t FFT_frequency_Chanel_1;
float32_t FFT_frequency_Chanel_2;
float32_t FFT_frequency_Chanel_3;
float32_t FFT_frequency_Chanel_4;
float32_t FFT_frequency_Chanel_5;
float32_t FFT_frequency_Chanel_6;
float32_t FFT_frequency_Chanel_7;
/*
AD7606��ͨ�� �� AD7606�Ĳ���Ƶ��
FFT���ݴ�С��1024
������
float32_t FFT_frequency_Chanel_0;
float32_t FFT_frequency_Chanel_1;
float32_t FFT_frequency_Chanel_2;
float32_t FFT_frequency_Chanel_3;
float32_t FFT_frequency_Chanel_4;
float32_t FFT_frequency_Chanel_5;
float32_t FFT_frequency_Chanel_6;
float32_t FFT_frequency_Chanel_7;
*/
void FFT_Freq_Computation(uint8_t AD7606_Chanle, uint16_t SAPLE_Rate)
{
	data_conver_ok_flag = 0;
	float32_t Dcoffset = 0.0;
	for (uint16_t size = 0; size < ad7606_num; size++)
	{
		Dcoffset += ad7606_value_float32[size][AD7606_Chanle];
	}
	Dcoffset = Dcoffset / ad7606_num;

	float32_t Input[ad7606_num * 2];
	for (uint16_t size = 0; size < ad7606_num; size++)
	{
		Input[2 * size] = ad7606_value_float32[size][AD7606_Chanle] - Dcoffset; // ���ʵ��
		Input[2 * size + 1] = 0;												// ����鲿
	}

	/* Ϊ�����ź�A���ź�B��Ӻ����� */
	// float32_t window[ad7606_num];
	// for (uint16_t i = 0; i < ad7606_num; i++)
	// {
	// 	window[i] = 0.5f * (1.0f - cosf(2.0f * PI * i / (ad7606_num - 1)));
	// 	Input[2 * i] *= window[i];
	// }
	/*����������м���,�ɽ�����ע��ȡ���Ӵ�*/

	// ��ʼ��FFTʵ��
	// arm_cfft_radix4_instance_f32 fft_inst;
	// arm_cfft_radix4_init_f32(&fft_inst, ad7606_num, 0, 1); // FFT��ʼ��
	// arm_cfft_radix4_f32(&fft_inst, Input); // FFT����

	arm_cfft_radix2_instance_f32 fft_inst;
	arm_cfft_radix2_init_f32(&fft_inst, ad7606_num, 0, 1); // FFT��ʼ��
	arm_cfft_radix2_f32(&fft_inst, Input);				   // FFT��

	// �ҵ������ȵ�Ƶ��
	uint32_t maxIndex = 0;
	float32_t maxValue = 0;
	for (uint32_t i = 0; i < ad7606_num; i++)
	{
		// ���㸴������
		float32_t real = Input[2 * i];
		float32_t imag = Input[2 * i + 1];
		float32_t magnitude = sqrtf(real * real + imag * imag);

		// �����Ƶ��ķ�����ĿǰΪֹ���ģ���¼�õ������
		if (magnitude > maxValue)
		{
			maxValue = magnitude;
			maxIndex = i;
		}
	}

	// FFT������ת��Ϊ��ʵ��Ƶ��
	float32_t FFT_frequency = (float32_t)maxIndex * SAPLE_Rate / (float32_t)ad7606_num;
	// double FFT_frequency = (double)maxIndex * SAPLE_Rate / (double)1024;

	// ��ӡƵ��
	// printf("Signal frequency: %f Hz\r\n", FFT_frequency);
	switch (AD7606_Chanle)
	{
	case 0:
		FFT_frequency_Chanel_0 = FFT_frequency;
		break;
	case 1:
		FFT_frequency_Chanel_1 = FFT_frequency;
		break;
	case 2:
		FFT_frequency_Chanel_2 = FFT_frequency;
		break;
	case 3:
		FFT_frequency_Chanel_3 = FFT_frequency;
		break;
	case 4:
		FFT_frequency_Chanel_4 = FFT_frequency;
		break;
	case 5:
		FFT_frequency_Chanel_5 = FFT_frequency;
		break;
	case 6:
		FFT_frequency_Chanel_6 = FFT_frequency;
		break;
	case 7:
		FFT_frequency_Chanel_7 = FFT_frequency;
		break;

	default:
		break;
	}
	data_conver_ok_flag = 1;
}
