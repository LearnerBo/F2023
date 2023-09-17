#include "Cmd.h"

void send__curve_8bit(uint8_t scren_id, uint8_t widget_id, uint8_t channel, uint8_t data) // 画面ID 控件ID 通道 八位数据
{
    sendingData=1;
    uint8_t data_curve_8bit[] = {0xEE, 0xB1, 0x32, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x01, 0x00, 0xFF, 0xFC, 0xFF, 0xFF};
    // 插入特定的数据                                     画面         控件  通道        1byte  数据
    data_curve_8bit[4] = scren_id;
    data_curve_8bit[6] = widget_id;
    data_curve_8bit[7] = channel;
    data_curve_8bit[10] = data;

    for (int i = 0; i < sizeof(data_curve_8bit); i++)
    {
        // 等待前一个字节发送完成
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
            ;
        USART_SendData(USART1, data_curve_8bit[i]);
    }
}

void send_string_data(uint8_t screen_id, uint8_t widget_id, char *str, uint8_t str_length)//画面ID 控件ID 字符串 字符串的长度 使用前先用sprintf(str, "%d", sensor_data);  // 将整数转换为字符串 然后调用 send_string_data
//send_string_data(0x03, 0x04, str, strlen(str));

{
        sendingData=1;
    // 首部和尾部的固定数据
    uint8_t header[] = {0xEE, 0xB1, 0x10, 0x00};
    uint8_t footer[] = {0xFF, 0xFC, 0xFF, 0xFF};

    // 计算总的数据帧长度
    uint8_t frame_length = sizeof(header) + 3 + str_length + sizeof(footer);

    // 创建数据帧
    uint8_t frame[frame_length];

    // 填充数据帧的头部
    memcpy(frame, header, sizeof(header));

    // 填充数据帧的画面ID和控件ID
    frame[4] = screen_id;
    frame[5] = 0x00;
    frame[6] = widget_id;

    // 填充字符串数据
    for (int i = 0; i < str_length; i++)
    {
        frame[7 + i] = str[i];
    }

    // 填充数据帧的尾部
    memcpy(frame + 7 + str_length, footer, sizeof(footer));

    // 通过串口发送数据帧
    for (int i = 0; i < frame_length; i++)
    {
        // 等待前一个字节发送完成
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


