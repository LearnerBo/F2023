#include "Cmd.h"

void send__curve_8bit(uint8_t scren_id, uint8_t widget_id, uint8_t channel, uint8_t data) // ����ID �ؼ�ID ͨ�� ��λ����
{
    sendingData=1;
    uint8_t data_curve_8bit[] = {0xEE, 0xB1, 0x32, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF};
    // �����ض�������                                     ����         �ؼ�  ͨ��        1byte  ����
    data_curve_8bit[4] = scren_id;
    data_curve_8bit[6] = widget_id;
    data_curve_8bit[7] = channel;
    data_curve_8bit[10] = data;

    for (int i = 0; i < sizeof(data_curve_8bit); i++)
    {
        // �ȴ�ǰһ���ֽڷ������
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
        USART_SendData(USART1, data_curve_8bit[i]);
    }
}

void send_string_data(uint8_t screen_id, uint8_t widget_id, char *str, uint8_t str_length)//����ID �ؼ�ID �ַ��� �ַ����ĳ��� ʹ��ǰ����sprintf(str, "%d", sensor_data);  // ������ת��Ϊ�ַ��� Ȼ����� send_string_data
//send_string_data(0x03, 0x04, str, strlen(str));

{
        sendingData=1;
    // �ײ���β���Ĺ̶�����
    uint8_t header[] = {0xEE, 0xB1, 0x10, 0x00};
    uint8_t footer[] = {0xFF, 0xFC, 0xFF, 0xFF};

    // �����ܵ�����֡����
    uint8_t frame_length = sizeof(header) + 3 + str_length + sizeof(footer);

    // ��������֡
    uint8_t frame[frame_length];

    // �������֡��ͷ��
    memcpy(frame, header, sizeof(header));

    // �������֡�Ļ���ID�Ϳؼ�ID
    frame[4] = screen_id;
    frame[5] = 0x00;
    frame[6] = widget_id;

    // ����ַ�������
    for (int i = 0; i < str_length; i++)
    {
        frame[7 + i] = str[i];
    }

    // �������֡��β��
    memcpy(frame + 7 + str_length, footer, sizeof(footer));

    // ͨ�����ڷ�������֡
    for (int i = 0; i < frame_length; i++)
    {
        // �ȴ�ǰһ���ֽڷ������
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
        USART_SendData(USART1, frame[i]);
    }
}

void send_string_source_data(uint8_t scren_id,uint8_t widget_id ,uint16_t source_data)
{
    sendingData=1;
    char str[4];
    uint8_t str_lenth=sprintf(str,"%d",source_data);
    send_string_data(scren_id,widget_id,str,str_lenth);
}


