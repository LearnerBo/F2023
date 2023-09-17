/*! 
*  \file cmd_queue.h
*  \brief ����ָ�����
*  \version 1.0
*  \date 2012-2018
*  \copyright ���ݴ�ʹ��Ƽ����޹�˾
*/

#ifndef _CMD_QUEUE_H
#define _CMD_QUEUE_H
#include "hmi_driver.h"
#include "stdint.h"
typedef unsigned char qdata;
typedef unsigned short qsize;

/*! 
*  \brief  ���ָ������
*/
extern void queue_reset(void);

/*! 
* \brief  ���ָ������
* \detial ���ڽ��յ����ݣ�ͨ���˺�������ָ����� 
*  \param  _data ָ������
*/
extern void queue_push(qdata _data);

/*! 
*  \brief  ��ָ�������ȡ��һ��������ָ��
*  \param  cmd ָ����ջ�����
*  \param  buf_len ָ����ջ�������С
*  \return  ָ��ȣ�0��ʾ������������ָ��
*/
extern qsize queue_find_cmd(qdata *cmd,qsize buf_len);

#endif

