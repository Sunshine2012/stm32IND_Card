/************************************************************
  * 版权所有(C)2014
  * 项目代号:
  * 文件名称:       Queue.c
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


#include <includes.h>

/**********************************************************************
  * 函数名称:   char * strRev (char * str)
  * 功能描述:   字符串反转
  * 输入参数:   要反转的字符串
  * 输出参数:   str
  * 返 回 值:   char * str
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:
  * 其它说明:
************************************************************************/
char * strRev (char * str)
{
    char *p1 = NULL;
    char *p2 = NULL;

    if (!str || !*str)
    {
        return str;
    }

    for (p1 = str, p2 = str + strlen (str) - 1; p2 > p1; ++p1,--p2)
    {
        *p1 ^= *p2;
        *p2 ^= *p1;
        *p1 ^= *p2;
    }
    return str;
}

/**********************************************************************
  * 函数名称:   char * ftoa (int n, char *s, int b)
  * 功能描述:   浮点型数据转换成ASCII
  * 输入参数:   int n:已经转换为整型的浮点数(100倍), char *s:要保存数据的数组, int b:转换的目标进制
  * 输出参数:   char *s
  * 返 回 值:   char *s
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:
  * 其它说明:
************************************************************************/

char * ftoa (int n, char *s, int b)
{
    static char digits[] = "0123456789abcdefghijklmnopqrstuvwxyz";
    int i = 0,sign = 0;
    int temp = 0;
    if ((sign = n) < 0)
    {
        n = -n;
    }
    temp = n;
    do
    {
        // 在这里的数据是被放大了100倍的,所以加上留下2位小数
        if (2 == i)
        {
            s[i++] = '.';
        }
        s[i++] = digits[n % b];

    }while((n /= b) > 0);
    // 如果此时小于10,则人为加上0,和小数点,一个前导0
    if (temp < b)
    {
        s[i++] = '0';
        s[i++] = '.';
        s[i++] = '0';
    }
    // 如果此时数据小于100,大于10,则人为加上小数点,和一个前导0
    if (temp >= b && temp < (b * b))
    {
        s[i++] = '.';
        s[i++] = '0';
    }

    // 如果数据小于0,则也人为在前面加上'-'
    if (sign < 0)
    {
        s[i++] = '-';
    }
    s[i] = '\0';

    return strRev(s);
}

/**********************************************************************
  * 函数名称:   u8 canInitQueue (CanRxQueue * tRxQueue)
  * 功能描述:   初始化队列
  * 输入参数:   CanRxQueue * tRxQueue: 队列指针
  * 输出参数:
  * 返 回 值:   成功为0
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:   20140512
  * 其它说明:   无
************************************************************************/
u8 canInitQueue (CanRxQueue * tRxQueue)
{
    memset (tRxQueue, 0, sizeof (CanRxQueue));
	tRxQueue->empty = 1;    // 队列为空
	tRxQueue->full = 0;     // 队列为满
	tRxQueue->top = 0;      // 队列的下标指针
	tRxQueue->bottom = 0;   // 队列的目标指针
    return 0;
}


/**********************************************************************
  * 函数名称:   u8 canInQueue (CanRxQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
  * 功能描述:   数据入队列,这里设置的为循环队列,队列大小为RX_DATA_BUFF_SIZE,当数据存满了,将队头的数据删除
  * 输入参数:   CanRxQueue * tRxQueue: 队列指针, CanRxMsg * const p_tNewNode:即将入队列的数据指针
  * 输出参数:
  * 返 回 值:   成功为0, 不成功为-1或者-2
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:   20140512
  * 其它说明:   无
************************************************************************/
u8 canInQueue (CanRxQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
{
    int i = 0;
    // 队列地址无效
    if (!p_tQueue)
        return 1;
    // 入队列数据地址无效
    if (!p_tNewNode)
        return 1;
    // 与队列中最后一个元素进行比较,如果里面的数据完全一样,则数据不入队列,直接返回,这样减轻重复数据
    /*
    if (1 != p_tQueue->empty)
    {
        for (i = 0; i < 8; i++)
        {
            // 如果数据相等,则继续比较,直到最后一个数据
            if (p_tQueue->news[p_tQueue->top - 1].Data[i] == p_tNewNode->Data[i])
            {
                continue;
            }
            else
            {
                break;
            }
        }
    }
    // 当比较到最后一个数据还是相等,则直接返回
    if (8 == i)
    {
        return 2;
    }
    */
    if (((p_tQueue->top + 1) % RX_DATA_BUFF_SIZE) == p_tQueue->bottom)
    {
        // 循环队列
       p_tQueue->bottom =  (p_tQueue->bottom + 1) % RX_DATA_BUFF_SIZE;
        // 队列满
        p_tQueue->full = 1;
    }
    // 数据进入队列
    p_tQueue->news[p_tQueue->top] = *p_tNewNode;
    p_tQueue->top = (p_tQueue->top + 1) % RX_DATA_BUFF_SIZE;
    p_tQueue->empty = 0;
    return 0;
}


/**********************************************************************
  * 函数名称:   u8 canOutQueue (CanRxQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
  * 功能描述:   出队列
  * 输入参数:   CanRxQueue * const p_tQueue:队列首地址, CanRxMsg * p_tReturnNode:出队列元素保存地址
  * 输出参数:   出队列元素
  * 返 回 值:   成功为0
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:
  * 其它说明:
************************************************************************/
u8 canOutQueue (CanRxQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
{
    if (!p_tQueue)
        return 1;
    if (!p_tReturnNode)
        return 1;
    if (1 == p_tQueue->empty)
        return 1;
    // 空队列
    if ((p_tQueue->bottom % RX_DATA_BUFF_SIZE) == p_tQueue->top)
    {
        p_tQueue->empty = 1;
        return 1;
    }
    // 数据出队列
    *p_tReturnNode = p_tQueue->news[p_tQueue->bottom];
    p_tQueue->bottom = (p_tQueue->bottom + 1) % RX_DATA_BUFF_SIZE;
    p_tQueue->full = 0;
    return 0;
}
