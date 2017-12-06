/************************************************************
  * 版权所有(C)2014, 广州海格通信集团股份有限公司.
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
#define RX_DATA_BUFF_SIZE       20

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

// 接收队列:接收天线数据
typedef struct RxQueue
{   
    CanRxMsg news[RX_DATA_BUFF_SIZE];
    u32 top;
    u32 bottom;
    u32 empty;        //不为零表示为空
    u32  full;        //不为零表示满  
}RxQueue;


u8 initQueue (RxQueue * tRxQueue);
u8 inQueue (RxQueue * const p_tQueue, CanRxMsg * const p_tNewNode);
u8 outQueue (RxQueue * const p_tQueue, CanRxMsg * p_tReturnNode);


#endif
