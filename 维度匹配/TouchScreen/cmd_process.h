/*! 
*  \file hmi_driver.h
*  \brief 指令分类处理
*  \version 1.0
*  \date 2012-2018
*  \copyright 广州大彩光电科技有限公司
*/
#ifndef CMD_PROCESS_H
#define CMD_PROCESS_H
#include "hmi_driver.h"
#include "stm32f4xx.h"
#include "stdint.h"

extern volatile uint32_t  timer_tick_count;                                               //定时器节拍

extern uint8_t  cmd_buffer[CMD_MAX_SIZE];                                                     //指令缓存
extern  uint16_t current_screen_id;                                                 //当前画面ID
extern  int32_t progress_value;                                                     //进度条测试值
extern  int32_t test_value;                                                         //测试值
extern  uint8_t update_en;                                                          //更新标记
extern  int32_t meter_flag;                                                         //仪表指针往返标志位
extern  int32_t num;                                                                //曲线采样点计数
extern  int sec;                                                                  //时间秒
extern  int32_t curves_type;                                                        //曲线标志位  0为正弦波，1为锯齿波                  
extern  int32_t second_flag;                                                          //时间标志位
extern  int32_t icon_flag;                                                          //图标标志位
extern  uint8_t Select_H ;                                                              //滑动选择小时
extern  uint8_t Select_M ;                                                              //滑动选择分钟 
extern  uint8_t Last_H ;                                                                //上一个选择小时
extern  uint8_t Last_M;                                                                 //上一个选择分钟 
extern  int32_t Progress_Value;                                                     //进度条的值 


#define NOTIFY_TOUCH_PRESS         0X01  //触摸屏按下通知
#define NOTIFY_TOUCH_RELEASE       0X03  //触摸屏松开通知
#define NOTIFY_WRITE_FLASH_OK      0X0C  //写FLASH成功
#define NOTIFY_WRITE_FLASH_FAILD   0X0D  //写FLASH失败
#define NOTIFY_READ_FLASH_OK       0X0B  //读FLASH成功
#define NOTIFY_READ_FLASH_FAILD    0X0F  //读FLASH失败
#define NOTIFY_MENU                0X14  //菜单事件通知
#define NOTIFY_TIMER               0X43  //定时器超时通知
#define NOTIFY_CONTROL             0XB1  //控件更新通知
#define NOTIFY_READ_RTC            0XF7  //读取RTC时间
#define MSG_GET_CURRENT_SCREEN     0X01  //画面ID变化通知
#define MSG_GET_DATA               0X11  //控件数据通知
#define NOTIFY_HandShake           0X55  //握手通知

#define PTR2U16(PTR) ((((uint8_t *)(PTR))[0]<<8)|((uint8_t *)(PTR))[1])  //从缓冲区取16位数据
#define PTR2U32(PTR) ((((uint8_t *)(PTR))[0]<<24)|(((uint8_t *)(PTR))[1]<<16)|(((uint8_t *)(PTR))[2]<<8)|((uint8_t *)(PTR))[3])  //从缓冲区取32位数据


enum CtrlType
{
    kCtrlUnknown=0x0,
    kCtrlButton=0x10,                     //按钮
    kCtrlText,                            //文本
    kCtrlProgress,                        //进度条
    kCtrlSlider,                          //滑动条
    kCtrlMeter,                            //仪表
    kCtrlDropList,                        //下拉列表
    kCtrlAnimation,                       //动画
    kCtrlRTC,                             //时间显示
    kCtrlGraph,                           //曲线图控件
    kCtrlTable,                           //表格控件
    kCtrlMenu,                            //菜单控件
    kCtrlSelector,                        //选择控件
    kCtrlQRCode,                          //二维码
};

#pragma pack(push)
#pragma pack(1)                           //按字节对齐

typedef struct
{
    uint8_t    cmd_head;                    //帧头

    uint8_t    cmd_type;                    //命令类型(UPDATE_CONTROL)    
    uint8_t    ctrl_msg;                    //CtrlMsgType-指示消息的类型
    uint16_t   screen_id;                   //产生消息的画面ID
    uint16_t   control_id;                  //产生消息的控件ID
    uint8_t    control_type;                //控件类型

    uint8_t    param[256];                  //可变长度参数，最多256个字节

    uint8_t  cmd_tail[4];                   //帧尾
}CTRL_MSG,*PCTRL_MSG;


void ClearBuffer(unsigned char *buffer, unsigned short buf_len);  //清空缓冲区
/*! 
*  \brief  握手通知
*/
void NOTIFYHandShake(void);

/*! 
*  \brief  消息处理流程
*  \param msg 待处理消息
*  \param size 消息长度
*/
void ProcessMessage( PCTRL_MSG msg, uint16_t size );
/*! 
*  \brief  画面切换通知
*  \details  当前画面改变时(或调用GetScreen)，执行此函数
*  \param screen_id 当前画面ID
*/
void NotifyScreen(uint16_t screen_id);
/*! 
*  \brief  触摸坐标事件响应
*  \param press 1按下触摸屏，3松开触摸屏
*  \param x x坐标
*  \param y y坐标
*/
void NotifyTouchXY(uint8_t press,uint16_t x,uint16_t y);

/*! 
*  \brief  按钮控件通知
*  \details  当按钮状态改变(或调用GetControlValue)时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param state 按钮状态：0弹起，1按下
*/
void NotifyButton(uint16_t screen_id, uint16_t control_id, uint8_t state);
/*! 
*  \brief  文本控件通知
*  \details  当文本通过键盘更新(或调用GetControlValue)时，执行此函数
*  \details  文本控件的内容以字符串形式下发到MCU，如果文本控件内容是浮点值，
*  \details  则需要在此函数中将下发字符串重新转回浮点值。
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param str 文本控件内容
*/
void NotifyText(uint16_t screen_id, uint16_t control_id, uint8_t *str);
/*!                                                                              
*  \brief  进度条控件通知                                                       
*  \details  调用GetControlValue时，执行此函数                                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/   
void NotifyProgress(uint16_t screen_id, uint16_t control_id, uint32_t value);
/*!                                                                              
*  \brief  滑动条控件通知                                                       
*  \details  当滑动条改变(或调用GetControlValue)时，执行此函数                  
*  \param screen_id 画面ID                                                      
*  \param control_id 控件ID                                                     
*  \param value 值                                                              
*/    
void NotifySlider(uint16_t screen_id, uint16_t control_id, uint32_t value);
/*! 
*  \brief  仪表控件通知
*  \details  调用GetControlValue时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param value 值
*/
void NotifyMeter(uint16_t screen_id, uint16_t control_id, uint32_t value);
/*! 
*  \brief  菜单控件通知
*  \details  当菜单项按下或松开时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 菜单项索引
*  \param state 按钮状态：0松开，1按下
*/
void NotifyMenu(uint16_t screen_id, uint16_t control_id, uint8_t  item, uint8_t  state);

/*! 
*  \brief  选择控件通知
*  \details  当选择控件变化时，执行此函数
*  \param screen_id 画面ID
*  \param control_id 控件ID
*  \param item 当前选项
*/
void NotifySelector(uint16_t screen_id, uint16_t control_id, uint8_t  item);
/*! 
*  \brief  定时器超时通知处理
*  \param screen_id 画面ID
*  \param control_id 控件ID
*/
void NotifyTimer(uint16_t screen_id, uint16_t control_id);
/*! 
*  \brief  读取用户FLASH状态返回
*  \param status 0失败，1成功
*  \param _data 返回数据
*  \param length 数据长度
*/
void NotifyReadFlash(uint8_t status,uint8_t *_data,uint16_t length);

/*! 
*  \brief  写用户FLASH状态返回
*  \param status 0失败，1成功
*/
void NotifyWriteFlash(uint8_t status);
/*! 
*  \brief  读取RTC时间，注意返回的是BCD码
*  \param year 年（BCD）
*  \param month 月（BCD）
*  \param week 星期（BCD）
*  \param day 日（BCD）
*  \param hour 时（BCD）
*  \param minute 分（BCD）
*  \param second 秒（BCD）
*/
void NotifyReadRTC(uint8_t year,uint8_t month,uint8_t week,uint8_t day,uint8_t hour,uint8_t minute,uint8_t second);
#endif
