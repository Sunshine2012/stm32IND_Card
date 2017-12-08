/************************************************************
  * 版权所有(C)2014
  * 项目代号:
  * 文件名称:       __QUEUE_H
  * 运行平台:       STM32F103VC
  * 编译环境:       Keil 4.72 for Arm
  * 程序语言:       c
  * 内容摘要:
  *----------------------------------------------------------
  * 作    者:       .彭杰
  * 建立日期:
  * 版 本 号:       v1.0
  * 描    述:
  *----------------------------------------------------------
  * 修 改 者:
  * 修改日期:
  * 描    述:
************************************************************/


#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "includes.h"
// 接收队列缓冲区域大小
#define RX_DATA_BUFF_SIZE       50

// 将两个字节转换成半字
#define MAKE_HALF_WORD(a, b)	(((b & 0xff) << 8) | (a&0xff))
// 获取short类型的数据的低字节
#define LOWBYTE(a)		(a & 0xff)
// 获取short类型的数据的高字节
#define HIGHBYTE(a)		((a & 0xff00) >> 8)
// 测试一个字节中第b位是否为1
#define TESTBIT(a, b)   ((a >> b) & 0x01)
// 置位数据a中的b位
#define SETBIT(a, b)	(a = (a | (1 << b)))
// 清零数据a中的b位
#define CLRBIT(a, b)	(a = (a & (~(1 << b))))

// 接收队列:接收卡机数据
typedef struct CanQueue
{
    CanRxMsg news[RX_DATA_BUFF_SIZE];
    u32 top;
    u32 bottom;
    u32 empty;        //不为零表示为空
    u32 full;         //不为零表示满
}CanQueue;

// 接收队列:接收上位机数据
typedef struct UartQueue
{
    char news[RX_DATA_BUFF_SIZE][50];
    u32 top;
    u32 bottom;
    u32 empty;        //不为零表示为空
    u32 full;         //不为零表示满
}UartQueue;

u8 canInitQueue (CanQueue * p_tQueue);
u8 canInQueue (CanQueue * const p_tQueue, CanRxMsg * const p_tNewNode);
u8 canOutQueue (CanQueue * const p_tQueue, CanRxMsg * p_tReturnNode);

u8 uartInitQueue (UartQueue * p_tQueue);
u8 uartInQueue (UartQueue * const p_tQueue, u8 * const p_ucaNewNode);
u8 uartOutQueue (UartQueue * const p_tQueue, u8 * p_ucaReturnNode);


#endif
