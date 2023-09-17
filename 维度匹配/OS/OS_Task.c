#include "OS_Task.h"
// ��������ʼ����ǩ����
char xLabel[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L'};
char yLabel[] = {12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
uint8_t Mode=0;
char *xLabels[] = {"AB", "CD", "EF", "GH", "IJ", "KL"};
char *yLabels[] = {"0102", "0304", "0506", "0708", "0910", "1112"};
uint8_t x;               // ����ϵx
uint8_t y;               // ����ϵy
uint8_t Change_Flag = 0; // ���ݸı��־λ,��Ϊ1ʱˢ��һ����Ļ

/*
����ɨƵ
*/
void sweptFresignal_Task(void)
{
    static u32 Freq = 15000;
    if (SysTick_Signal_1s == 1)
    {
        SysTick_Signal_1s = 0;
        SettingPWM_Frequency(Freq);
        SettingPWM(50);
        Freq += 1000;
        if (Freq == 21000)
            Freq = 15000;
    }
}
/*
���Ҳ�ɨƵ
*/
void sweptFreSinsignal_Task(void)
{
    static u16 Freq = 15000;
    static u32 arr;
    if (SysTick_Signal_1s == 1)
    {
        SysTick_Signal_1s = 0;
        TIM6DAC_Init(Freq, 0); // Fre
        Freq += 1000;
        if (Freq == 21000)
            Freq = 15000;
    }
}
/*
��ʾ����
*/
void display_Task(void)
{
    if (SysTick_Display_500ms == 1)
    {
        SysTick_Display_500ms = 0;
        char Click[10]; // �û�����ϵ
        Change_Flag = 0;
        if(Change_Flag == 1)
        {
        Change_Flag = 0;
        sprintf(Click, "(%s,%s)", xLabels[x], yLabels[y]); // �û����潨ϵ
        SetTextValue(0, 1, Click);
        }
    }
}



void modeRecTask(void)
{
    if (SysTick_Mode_Rec_100ms == 1)
    {
        SysTick_Mode_Rec_100ms = 0;
        size = queue_find_cmd(cmd_buffer, CMD_MAX_SIZE); // �ӻ��������ҵ�ָ��
        if (size != 0)
        {
            ProcessMessage((PCTRL_MSG)cmd_buffer, size); // ����ָ��
            switch (cmd_id)
            {
            case 0x0004:
                if (valueh == 0x101FFFC) //����//  ԭ��!����! ������λ
                {
                    Mode = 1;
                    TIM_Cmd(TIM6,DISABLE);
                }
                else if (valueh == 0x100FFFC) // �ɿ�  ɨƵ�ź�
                {
                    Mode = 0;
                    DAC_Signal_Init(); 
                    SetTextValue(0,1," ");
                }
                break;
                cmd_id = 0;
            }
        }
    }
}

void SystemTask(void)
{
    if(Mode == 1)
    {   soundlocationTask();
        LocationStatic_Task();
		display_Task();
    }

    if(Mode == 0)
    {
        sweptFreSinsignal_Task();
    }

}

int32_t error = 0;

void Location_Task(void)
{
    if (SysTick_Loc_500ms == 1)
    {
        SysTick_Loc_500ms = 0;
        uint8_t isFound = 0;                                                                                                          // ���һ������Ĳ�������
        if ((Capture_Secure_FLAG1 == 0) && (Capture_Secure_FLAG2 == 0) && (Capture_Secure_FLAG3 == 0) && (Capture_Secure_FLAG4 == 0)) //
        {

            for (uint8_t X = 0; X < 6 && !isFound; X++)
            {

                for (uint8_t Y = 0; Y < 6 && !isFound; Y++)
                {
                    if (
                        ((SUB0 > Sub[0].grid[X][Y].min_val + error) && (SUB0 < Sub[0].grid[X][Y].max_val - error)) &&
                        ((SUB1 > Sub[1].grid[X][Y].min_val + error) && (SUB1 < Sub[1].grid[X][Y].max_val - error)) &&
                        ((SUB2 > Sub[2].grid[X][Y].min_val + error) && (SUB2 < Sub[2].grid[X][Y].max_val - error)) &&
                        ((SUB3 > Sub[3].grid[X][Y].min_val + error) && (SUB3 < Sub[3].grid[X][Y].max_val - error)) &&
                        ((SUB4 > Sub[4].grid[X][Y].min_val + error) && (SUB4 < Sub[4].grid[X][Y].max_val - error)) &&
                        ((SUB5 > Sub[5].grid[X][Y].min_val + error) && (SUB5 < Sub[5].grid[X][Y].max_val - error)))
                    {
                        x = Y;
                        y = X;
                        printf("X =%d,Y =%d", x, y);
                        Change_Flag = 1;
                        isFound = 1; // ����������ʱ�����ò�������Ϊ��
                    }
                }
            }
        }
    }
}

void soundlocationTask(void)
{
    if (SysTick_Location_500ms == 1)
    {
        SysTick_Location_500ms = 0;

        if ((Capture_Secure_FLAG1 == 0) && (Capture_Secure_FLAG2 == 0) && (Capture_Secure_FLAG3 == 0) && (Capture_Secure_FLAG4 == 0)) //
        {
            SUB0 = capture1 - capture2;
            SUB1 = capture2 - capture3;
            SUB2 = capture3 - capture4;
            SUB3 = capture4 - capture1;
            SUB4 = capture1 - capture3;
            SUB5 = capture2 - capture4;
        }
    }
}

void Close_InterTask(void)
{
    if ((Capture_Secure_FLAG1 == 1) || (Capture_Secure_FLAG2 == 1) || (Capture_Secure_FLAG3 == 1) || (Capture_Secure_FLAG4 == 1)) //
    {
        SysTick->CTRL &= ~SysTick_CTRL_TICKINT_Msk; // �ر� systick �ж�
        USART1->CR1 &= ~USART_CR1_RXNEIE;           // �رմ���1���ж�
    }
    if ((Capture_Secure_FLAG1 == 0) && (Capture_Secure_FLAG2 == 0) && (Capture_Secure_FLAG3 == 0) && (Capture_Secure_FLAG4 == 0)) //
    {
        SysTick->CTRL |= SysTick_CTRL_TICKINT_Msk; // ���� systick �ж�
        USART1->CR1 |= USART_CR1_RXNEIE;           // ��������1���ж�
    }
}

void LocationStatic_Task(void)
{
    if (SysTick_Loc_500ms == 1)
    {
        SysTick_Loc_500ms = 0;
        if ((Capture_Secure_FLAG1 == 0) && (Capture_Secure_FLAG2 == 0) && (Capture_Secure_FLAG3 == 0) && (Capture_Secure_FLAG4 == 0))
        {
            int match_degree[6][6] = {0};
            int max_degree = -1;
            uint8_t maxX = 0, maxY = 0;

            for (uint8_t X = 0; X < 6; X++)
            {
                for (uint8_t Y = 0; Y < 6; Y++)
                {
                    if ((SUB0 > Sub[0].grid[X][Y].min_val + error) && (SUB0 < Sub[0].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB1 > Sub[1].grid[X][Y].min_val + error) && (SUB1 < Sub[1].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB2 > Sub[2].grid[X][Y].min_val + error) && (SUB2 < Sub[2].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB3 > Sub[3].grid[X][Y].min_val + error) && (SUB3 < Sub[3].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB4 > Sub[4].grid[X][Y].min_val + error) && (SUB4 < Sub[4].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB5 > Sub[5].grid[X][Y].min_val + error) && (SUB5 < Sub[5].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }

                    if (match_degree[X][Y] > max_degree)
                    {
                        max_degree = match_degree[X][Y];
                        maxX = X;
                        maxY = Y;
                    }
                }
            }

            int max_sum = -1;
            int dx[4] = {-1, 0, 1, 0};
            int dy[4] = {0, 1, 0, -1};

            for (uint8_t X = 0; X < 6; X++)
            {
                for (uint8_t Y = 0; Y < 6; Y++)
                {
                    if (match_degree[X][Y] == max_degree)
                    {
                        int sum = 0;

                        for (int i = 0; i < 4; i++)
                        {
                            int newX = X + dx[i];
                            int newY = Y + dy[i];

                            if (newX >= 0 && newX < 6 && newY >= 0 && newY < 6)
                            {
                                sum += match_degree[newX][newY];
                            }
                            else
                            {
                                sum += 3; // ��Եƥ��ȵ���3
                            }
                        }

                        if (sum > max_sum)
                        {
                            max_sum = sum;
                            maxX = X;
                            maxY = Y;
                        }
                    }
                }
            }

            x = maxY; // x ��Ӧ Y
            y = maxX; // y ��Ӧ X
            // printf("X = %d, Y = %d\n", x, y);
            Change_Flag = 1;
        }
    }
}

void LocationStaticPlus_Task(void)
{
    if (SysTick_Loc_500ms == 1)
    {
        SysTick_Loc_500ms = 0;
        if ((Capture_Secure_FLAG1 == 0) && (Capture_Secure_FLAG2 == 0) && (Capture_Secure_FLAG3 == 0) && (Capture_Secure_FLAG4 == 0))
        {
            int match_degree[6][6] = {0};
            int max_degree = -1;
            uint8_t maxX = 0, maxY = 0;
            int max_sum1 = -1, max_sum2 = -1;
            uint8_t maxX1 = 0, maxY1 = 0, maxX2 = 0, maxY2 = 0;

            for (uint8_t X = 0; X < 6; X++)
            {
                for (uint8_t Y = 0; Y < 6; Y++)
                {
                    if ((SUB0 > Sub[0].grid[X][Y].min_val + error) && (SUB0 < Sub[0].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB1 > Sub[1].grid[X][Y].min_val + error) && (SUB1 < Sub[1].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB2 > Sub[2].grid[X][Y].min_val + error) && (SUB2 < Sub[2].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB3 > Sub[3].grid[X][Y].min_val + error) && (SUB3 < Sub[3].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB4 > Sub[4].grid[X][Y].min_val + error) && (SUB4 < Sub[4].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }
                    if ((SUB5 > Sub[5].grid[X][Y].min_val + error) && (SUB5 < Sub[5].grid[X][Y].max_val - error))
                    {
                        match_degree[X][Y]++;
                    }

                    if (match_degree[X][Y] > max_degree)
                    {
                        max_degree = match_degree[X][Y];
                        maxX = X;
                        maxY = Y;
                    }
                }
            }

            int dx[4] = {-1, 0, 1, 0};
            int dy[4] = {0, 1, 0, -1};

            for (uint8_t X = 0; X < 6; X++)
            {
                for (uint8_t Y = 0; Y < 6; Y++)
                {
                    if (match_degree[X][Y] == max_degree)
                    {
                        int sum = 0;

                        for (int i = 0; i < 4; i++)
                        {
                            int newX = X + dx[i];
                            int newY = Y + dy[i];

                            if (newX >= 0 && newX < 6 && newY >= 0 && newY < 6)
                            {
                                sum += match_degree[newX][newY];
                            }
                            else
                            {
                                sum += 3; // ��Եƥ��ȵ���3
                            }
                        }

                        if (sum > max_sum1)
                        {
                            max_sum2 = max_sum1;
                            maxX2 = maxX1;
                            maxY2 = maxY1;

                            max_sum1 = sum;
                            maxX1 = X;
                            maxY1 = Y;
                        }
                        else if (sum > max_sum2)
                        {
                            max_sum2 = sum;
                            maxX2 = X;
                            maxY2 = Y;
                        }
                    }
                }
            }

            if (max_sum1  = max_sum2)
            {
                x = maxY1; // ע�� x ��Ӧ Y
                y = maxX1; // y ��Ӧ X
                printf("=X = %d, =Y = %d\n", x, y);
                Change_Flag = 1;
            }
            else
            {
                x = maxY1; // ע�� x ��Ӧ Y
                y = maxX1; // y ��Ӧ X
                printf("1X = %d, 1Y = %d\n", x, y);
                Change_Flag = 1;
            }
        }
    }
}

// static int8_t angle;
// float32_t Sum_phasic = 0;
// float32_t average_Phasic = 0;
// uint8_t average_cnt = 1;
// uint8_t Location_Ready_Flag = 0;

// /*
// ͨ����βɼ���λ,���˲�,���һ������ȶ���average_Phasic
// */

// void Angle_Location_Task()
// {
//     static uint8_t read_flag= 0;
//     static int32_t sub_captureA;
//     static int32_t sub_captureB;
//      if(SysTick_50ms == 1)
//   {
//     SysTick_50ms = 0;
//     if(read_flag == 0)
//     {
//         sub_captureA = sub_capture;
//         // Capture_Secure_FLAG = 0;
//         // printf("A%d\r\n",sub_captureA);
//         // Capture_Secure_FLAG = 1;
//     }
//     else if(read_flag == 1)
//     {
//         sub_captureB = sub_capture;
//         // Capture_Secure_FLAG = 0;
//         // printf("B%d\r\n",sub_captureB);
//         // Capture_Secure_FLAG = 1;
//     }
//     read_flag = !read_flag;
//     if(abs(sub_captureA - sub_captureB)<1)
//     {
//         TIM5phasic_float = (float32_t)((-sub_captureA - sub_captureB)/2)/6.313;
//         if(TIM5phasic_float > 180)
//         {
//             TIM5phasic_float = -360 + TIM5phasic_float;
//         }

//         Capture_Secure_FLAG =0;
//         printf("%f\r\n",TIM5phasic_float);
//         Capture_Secure_FLAG= 1;
//         Location_Ready_Flag = 1;
//     }
//   }
// }
// /*
// �������ȶ�����λ��,������λ���ö���Ƕ�
// */
// void Steering_Task()
// {

//     if (SysTick_100ms == 1)
//     {
//         SysTick_100ms = 0;
//         if (Location_Ready_Flag == 1)
//         {
//             Location_Ready_Flag = 0;
//             // �ڴ˴�����һ����������λ��Ƕȵ�ӳ��
//           if(TIM5phasic_float>-30&&TIM5phasic_float<-20)   //1
//           angle = -25;
//            if(TIM5phasic_float>-60&&TIM5phasic_float<-50)  //2
//           angle = -20;
//            if(TIM5phasic_float>-40&&TIM5phasic_float<-30)  //3
//           angle = -18;
//            if(TIM5phasic_float>80&&TIM5phasic_float<90)  //4
//           angle = -14;
//            if(TIM5phasic_float>-180&&TIM5phasic_float<-150)  //5
//           angle = -9;
//            if(TIM5phasic_float>-10&&TIM5phasic_float<0)  //6
//           angle = -5;
//            if(TIM5phasic_float>0&&TIM5phasic_float<10)  //7
//           angle = 0;
//            if(TIM5phasic_float>40&&TIM5phasic_float<50)  //8
//           angle = -3;
//            if(TIM5phasic_float>-180&&TIM5phasic_float<-170)  //9
//          angle = 8;
//            if(TIM5phasic_float>-160&&TIM5phasic_float<-150)//10
//           angle = 12;
//            if(TIM5phasic_float>30&&TIM5phasic_float<40)//11
//           angle = 15;
//            if(TIM5phasic_float>10&&TIM5phasic_float<20)//12
//           angle = 17;
//            if(TIM5phasic_float>-30&&TIM5phasic_float<-20)//13
//           angle = -22;

//             // angle = round((average_Phasic - 40) * (-37) / 110);
//             Steering_Turn(angle);
//             // printf("%d\r\n", angle);
//         }
//     }
// }
