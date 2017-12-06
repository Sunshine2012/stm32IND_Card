/************************************************************
  * 版权所有(C)2014, 广州海格通信集团股份有限公司.
  * 项目代号:       XX设备显控软件
  * 文件名称:       CanCmd.c
  * 运行平台:       STM32F107VC
  * 编译环境:       Keil 4.72 for Arm
  * 程序语言:       c
  * 内容摘要:  
  *----------------------------------------------------------
  * 作    者:       .彭杰
  * 建立日期:       20140507
  * 版 本 号:       v1.0
  * 描    述:
  *---------------------------------------------------------- 
  * 修 改 者:  
  * 修改日期:
  * 描    述:
************************************************************/



#include "CanCmd.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_can.h"
#include "stdlib.h"
#include "View.h"
#include "stdio.h"
#include "stdlib.h"

// 开关机状态
u8 g_PowerState = 0;

// GPS状态
u8 g_GPSState = 0;

// 惯导状态
u8 g_IMUState = 0;

// 系统工作模式1
u8 g_WorkMode1 = 0;

// 系统工作模式2
u8 g_WorkMode2 = 0;

// 系统工作状态
u8 g_WorkState = 0;

// 系统错误标志
u8 g_FlagErr = 0;

// 信号强度
u8 g_Strength = 0;


// 所有接收到的信息的缓存
CanRxMsg RxMessage  = {0};

// 接收到命令为0x14的队列:卫星各状态
RxQueue g_tRxQueue_0x14;
// 接收到命令为0x36的队列:方位角,俯仰角
RxQueue g_tRxQueue_0x36;
// 接收到命令为0x49的队列
// RxQueue g_tRxQueue_0x49;
// 接收到命令为0x1b的队列:卫星经度
RxQueue g_tRxQueue_0x1b;
// 接收到命令为0x1f的队列:GPS经度与纬度
RxQueue g_tRxQueue_0x1f;

// 接收到命令为0x1f的队列:GPS经度与纬度
RxQueue g_tRxQueue_0x49;


//RxQueue aa = 
//{
//    .top = 1
//};


/**********************************************************************
  * 函数名称:   char * StrRev (char * str)
  * 功能描述:   字符串反转
  * 输入参数:   要反转的字符串
  * 输出参数:   str
  * 返 回 值:   char * str
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:   
  * 其它说明:   
************************************************************************/
char * StrRev (char * str)
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
    
    return StrRev(s);
}

// 接收数据解析,数据来自队列中的,也可以收取之后直接进行解析
u8 RxDataCmdPro (CanRxMsg tRxTemp)
{
    static unsigned short int siAzi = 0; 
    static unsigned short int siEle = 0;
    static short int siSatLongitude = 0;    // 卫星经度
    static short int siGPS_Longitude = 0;   // GPS经度
    static short int siGPS_Latitude = 0;    // GPS纬度
    static short int siStrength = 0;        // 信号强度    
    
    // 整数转换成ASCII的字符串缓冲,大小为128
    char cAtemp[128] = " ";
    switch (tRxTemp.Data[0])
    {
        case (0x14):
            siStrength = MAKE_HALF_WORD (tRxTemp.Data[1],tRxTemp.Data[2]);
            
            // 信号强度最大为100,这是与20取整
            g_Strength = siStrength / 20;
        
            // PowerState = 1  为开机状态
            g_PowerState = (TESTBIT (tRxTemp.Data[3], 3)) & 0x01;
            if ((g_PowerState == 0) && (0x01 == g_tSvv[2].select[0]))// 选择了开机, 电机运行,系统没有运行,显示待机,或者寻星
            {
                strcpy ((char *)StatusPara[5],(char *)StatusPara[7]);
            }
            else 
            {
                strcpy ((char *)StatusPara[5],(char *)ShutDown[(~g_PowerState & 0x01) + 1]);
            }
            
            g_FlagErr = tRxTemp.Data[3];
            g_WorkMode1 = tRxTemp.Data[5] & 0x07;
            g_WorkMode2 = tRxTemp.Data[4] & 0x03;
           // g_WorkState = 

            g_GPSState = tRxTemp.Data[3] >> 2 & 0x01;
            
            break;
        case (0x1b):// 卫星经度
            siSatLongitude = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);// 
            // 将已经放大100倍的float数据转换成10进制的字符串
            ftoa (siSatLongitude, cAtemp, 10);
            // 
            strcpy ((char *)StatusPara[6],cAtemp);
            break;
        case (0x1f):// GPS参数
            // 经度
            siGPS_Longitude = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);
            // 纬度
            siGPS_Latitude = MAKE_HALF_WORD(tRxTemp.Data[3],tRxTemp.Data[4]);
            // 转换并更新GPS 经纬度
            if (siGPS_Longitude > -18000 && siGPS_Longitude < 18000)
            {
                ftoa (siGPS_Longitude, cAtemp, 10);
                strcpy ((char *)StatusPara[3],cAtemp);
            }
            if (siGPS_Latitude > 0 && siGPS_Latitude < 9000)
            {
                ftoa (siGPS_Latitude, cAtemp, 10);
                strcpy ((char *)StatusPara[4],cAtemp);
            }
            break;
        case (0x36):

            // 方位角
            siAzi = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);
            // 俯仰角
            siEle = MAKE_HALF_WORD(tRxTemp.Data[3],tRxTemp.Data[4]);
            if (siEle > 32768)
            {
                siEle -= 65536;
            }
            // 将已经放大100倍的float数据转换成10进制的字符串并更新卫星状态
            ftoa (siAzi, cAtemp, 10);
            strcpy ((char *)StatusPara[1],cAtemp);
            // 将已经放大100倍的float数据转换成10进制的字符串
            ftoa (siEle, cAtemp, 10);
            strcpy ((char *)StatusPara[2],cAtemp);
            
            // 将收到的方位角与俯仰角更新到手动控制菜单 
//            if (g_tSvv[2].select[0] != 3)
//            {
//                int temp = atoi(cAtemp);
//            }
                
            
            break;
#if 0 
            case (0x49):
            // 方位角
            siAzi = MAKE_HALF_WORD(tRxTemp.Data[1],tRxTemp.Data[2]);
            // 俯仰角
            siEle = MAKE_HALF_WORD(tRxTemp.Data[3],tRxTemp.Data[4]);
            if (siEle > 32768)
            {
                siEle -= 65536;
            }
            // 将已经放大100倍的float数据转换成10进制的字符串并更新卫星状态
            ftoa (siAzi, cAtemp, 10);
            strcpy ((char *)StatusPara[1],cAtemp);
            // 将已经放大100倍的float数据转换成10进制的字符串
            ftoa (siEle, cAtemp, 10);
            strcpy ((char *)StatusPara[2],cAtemp);
            
            // 将收到的方位角与俯仰角更新到手动控制菜单 
//            if (g_tSvv[2].select[0] != 3)
//            {
//                int temp = atoi(cAtemp);
//            }
            
            break;
#endif 
        default :
            break;
        
    }
    return 0;
    
}


/**********************************************************************
  * 函数名称:   u8 InitQueue (RxQueue * tRxQueue)
  * 功能描述:   初始化队列
  * 输入参数:   RxQueue * tRxQueue: 队列指针
  * 输出参数:   
  * 返 回 值:   成功为0
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:   20140512
  * 其它说明:   无
************************************************************************/
u8 InitQueue (RxQueue * tRxQueue)
{
    memset (tRxQueue, 0, sizeof (RxQueue));
	tRxQueue->empty = 1;    // 队列为空 
	tRxQueue->full = 0;     // 队列为满 
	tRxQueue->top = 0;      // 队列的下标指针
	tRxQueue->bottom = 0;   // 队列的目标指针
    return 0;
}


/**********************************************************************
  * 函数名称:   u8 InQueue (RxQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
  * 功能描述:   数据入队列,这里设置的为循环队列,队列大小为RX_DATA_BUFF_SIZE,当数据存满了,将队头的数据删除
  * 输入参数:   RxQueue * tRxQueue: 队列指针, CanRxMsg * const p_tNewNode:即将入队列的数据指针
  * 输出参数:   
  * 返 回 值:   成功为0, 不成功为-1或者-2
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:   20140512
  * 其它说明:   无
************************************************************************/
u8 InQueue (RxQueue * const p_tQueue, CanRxMsg * const p_tNewNode)
{
    int i = 0;
    // 队列地址无效
    if (!p_tQueue)
        return -1;
    // 入队列数据地址无效
    if (!p_tNewNode)
        return -1;    
    // 与队列中最后一个元素进行比较,如果里面的数据完全一样,则数据不入队列,直接返回,这样减轻重复数据
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
        return -2;
    }
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
  * 函数名称:   u8 OutQueue (RxQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
  * 功能描述:   出队列
  * 输入参数:   RxQueue * const p_tQueue:队列首地址, CanRxMsg * p_tReturnNode:出队列元素保存地址
  * 输出参数:   出队列元素
  * 返 回 值:   成功为0
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:   
  * 其它说明:   
************************************************************************/
u8 OutQueue (RxQueue * const p_tQueue, CanRxMsg * p_tReturnNode)
{
    if (!p_tQueue)
        return -1;  
    if (!p_tReturnNode)
        return -1;
    if (1 == p_tQueue->empty)
        return -1;
    // 空队列
    if ((p_tQueue->bottom % RX_DATA_BUFF_SIZE) == p_tQueue->top)
    {
        p_tQueue->empty = 1;
        return -1;
    }    
    // 数据出队列
    *p_tReturnNode = p_tQueue->news[p_tQueue->bottom];
    p_tQueue->bottom = (p_tQueue->bottom + 1) % RX_DATA_BUFF_SIZE; 
    p_tQueue->full = 0;
    return 0;
}


/**********************************************************************
  * 函数名称:   void CANTransmit (CanTxMsg * pTxd)
  * 功能描述:   发送数据
  * 输入参数:   CanTxMsg * pTxd:被打包的数据的地址
  * 输出参数:   无
  * 返 回 值:   无
  * 创 建 者:   彭杰
  * 创建时间:   20140511
  * 修改时间:   
  * 其它说明:   
************************************************************************/
void CANTransmit (CanTxMsg * pTxd)
{  
    u32 i = 0;
    u8 TransmitMailbox;
    // 在这里我们的数据都是标准帧,所以在这里统一设置成如下
    pTxd->IDE = CAN_ID_STD;
    pTxd->RTR = CAN_RTR_DATA;

    TransmitMailbox=CAN_Transmit(CAN1,pTxd);
    
    // 等待数据发送完成
    i = 0;
    while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending(CAN1,CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
    
    
}



/**********************************************************************
  * 函数名称:   u8 RptSatPara (StructViewValue *gp_tSvv)
  * 功能描述:   打包要发送的数据
  * 输入参数:   StructViewValue *gp_tSvv:要发送数据的结构
  * 输出参数:   
  * 返 回 值:   成功为0
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:
  * 其它说明:
************************************************************************/
u8 RptSatPara()
{
    
    return 0;
}

/**********************************************************************
  * 函数名称:   u8 ViewValueSendSelect (StructViewValue *gp_tSvv)
  * 功能描述:   打包要发送的数据
  * 输入参数:   StructViewValue *gp_tSvv:要发送数据的结构
  * 输出参数:   
  * 返 回 值:   成功为0
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:
  * 其它说明:
************************************************************************/
u8 ViewValueSendSelect (StructViewValue *gp_tSvv)
{
    int i = 0;
    int j = 0;
    int temp = 0;
    int  iTtemp = 0;
    char cAtemp[20] = " ";
    CanTxMsg TxMessage;
    memset (&TxMessage, 0, sizeof (CanRxMsg));
    
    switch (g_RealView.ID)
    {
        case VIEW_ID_SATSELECT:                 // 卫星选择
            switch (g_RealView.Cursor)
            {
                case 1:
                    // 将选定的卫星的名称复制到监控界面
                    strcpy (StatusPara[0], g_tSvv[0].para[0][g_tSvv[0].select[0]]);
                    break;
                case 2:
                    // 选择卫星经度
                    gp_tSvv->select[0] == 0x01 ? (cAtemp [0] = '+') : (cAtemp [0] = '-');
                    // 复制经度的后6位
                    for (i = 1; i <= 6; i++)
                    {
                        cAtemp [i] = gp_tSvv->select[i] - 1 + '0';
                    }
                    // 加上小数点与结束符
                    cAtemp[4] = '.';     
                    cAtemp[7] = '\0';
                    
                    // 将经度转换成浮点型并放大100成为整数
                    iTtemp = atof (cAtemp) * 100;
                    // 经度在180与-180之间
                    if (iTtemp > -18000 && iTtemp < 18000)
                    {                            
                        ftoa (iTtemp, cAtemp, 10);
                        // 将改好的经度复制给状态显示缓存
                        strcpy ((char *)StatusPara[6],cAtemp);

                        TxMessage.StdId = CAN_TX_STD_ID;
                        TxMessage.DLC = SEND_SIZE_8;
                        TxMessage.Data[0] = ACU_STA_CMD; 
                        //TxMessage.Data[0] = ACU_PARAMETER; 
                        TxMessage.Data[1] = LOWBYTE(iTtemp);
                        TxMessage.Data[2] = HIGHBYTE(iTtemp);
                        CANTransmit (&TxMessage);
                        return 0;
                    }
                    else
                    {
                        return 1;
                    }
                    break;
                case 3:
                    
                    // 以下是打包并发送开(关)机命令
                    TxMessage.StdId = CAN_TX_STD_ID;
                    TxMessage.DLC = SEND_SIZE_2;
                    // 如果选择关机(即ShutDown[2]),则发送0x00,则发送0x01
                    TxMessage.Data[0] = ACU_SHUTDOWN_CMD;
                    (gp_tSvv->select[0] == 2) ?  (TxMessage.Data[1] = ACU_TURN_DOWN) : \
                                                (TxMessage.Data[1] = ACU_TURN_ON);
                    // 发送三次开机命令
                    while (j < 3)
                    {
                        CANTransmit (&TxMessage);
                        j++;
                    }  
                    
                    // 如果选择的不是手动,将不能修改预置方位角,预置俯仰角
                    if (3 == gp_tSvv->select[0])
                    {
                        g_tAngleSet[0].isvalue = 1;
                        g_tAngleSet[1].isvalue = 1; 
                        
                        TxMessage.StdId = CAN_TX_STD_ID;
                        TxMessage.DLC = SEND_SIZE_4;
                        TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                        TxMessage.Data[1] = ACU_HAND;   // 手动
                        CANTransmit (&TxMessage);        
                        
                    }
                    else
                    {
                        g_tAngleSet[0].isvalue = 0;
                        g_tAngleSet[1].isvalue = 0; 

                        TxMessage.StdId = CAN_TX_STD_ID;
                        TxMessage.DLC = SEND_SIZE_4;
                        TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                        TxMessage.Data[1] = ACU_AUTO;   // 自动
                        CANTransmit (&TxMessage);
                    }
                    
                    return 0;
                    break;
                default :
                    break;
            }
            break;
            
        case VIEW_ID_MANUAL_CONTROL:            // 手动控制
             switch (g_RealView.Cursor)
                {
                    case 1:
                        // 复制方位角的6位
                        for (i = 0; i <= 6; i++)
                        {
                            cAtemp [i] = gp_tSvv->select[i] - 1 + '0';
                        }
                        // 加上小数点与结束符
                        cAtemp[3] = '.';     
                        cAtemp[6] = '\0';
                        
                        // 将方位角转换成浮点型并放大100成为整数
                        iTtemp = atof (cAtemp) * 100;
                        // 方位角在0与360之间
                        if (iTtemp > 0 && iTtemp < 36000)
                        {                            
                            ftoa (iTtemp, cAtemp, 10);
                            // 将改好的经度复制给状态显示缓存
                            strcpy ((char *)StatusPara[1],cAtemp);

                            TxMessage.StdId = CAN_TX_STD_ID;
                            TxMessage.DLC = SEND_SIZE_8;
                            TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                            TxMessage.Data[1] = ACU_AZI;
                            TxMessage.Data[2] = LOWBYTE(500/10);
                            TxMessage.Data[3] = HIGHBYTE(500/10);
                            TxMessage.Data[4] = CMD_0x55;
                            
                            CANTransmit (&TxMessage);
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                        break;
                    case 2:
                        // 复制俯仰角的6位
                        for (i = 0; i <= 6; i++)
                        {
                            cAtemp [i] = gp_tSvv->select[i] - 1 + '0';
                        }
                        // 加上小数点与结束符
                        cAtemp[3] = '.';     
                        cAtemp[6] = '\0';
                        
                        // 将方位角转换成浮点型并放大100成为整数
                        iTtemp = atof (cAtemp) * 100;
                        // 俯仰角在23与79度之间
                        if (iTtemp > 2300 && iTtemp < 7900)
                        {                            
                            ftoa (iTtemp, cAtemp, 10);
                            // 将改好的经度复制给状态显示缓存
                            strcpy ((char *)StatusPara[2],cAtemp);

                            TxMessage.StdId = CAN_TX_STD_ID;
                            TxMessage.DLC = SEND_SIZE_8;
                            TxMessage.Data[0] = ACU_HAND_MOVEMENT; 
                            TxMessage.Data[1] = ACU_ELE;
                            TxMessage.Data[2] = LOWBYTE(100/10);
                            TxMessage.Data[3] = HIGHBYTE(100/10);
                            TxMessage.Data[4] = CMD_0x55;
                            
                            CANTransmit (&TxMessage);
                            return 0;
                        }
                        else
                        {
                            return 1;
                        }
                        break;
                    default :
                        break;
                }
            break;
            
        default:
        
            break;
    
    }
	
    return 0;
}


/**********************************************************************
  * 函数名称:   void CANTransmitTest (void)
  * 功能描述:   发送测试函数
  * 输入参数:   无
  * 输出参数:   无
  * 返 回 值:   无
  * 创 建 者:   彭杰
  * 创建时间:   20140512
  * 修改时间:   
  * 其它说明:   
************************************************************************/
void CANTransmitTest (void)
{  
    u32 i = 0;
    u8 TransmitMailbox;
    CanTxMsg TxMessage;
    // 清零
    memset (&TxMessage, 0, sizeof (CanRxMsg));
    /* transmit */
    TxMessage.StdId = 0x6060 >> 5;
    TxMessage.ExtId = 0x00;
    TxMessage.RTR = CAN_RTR_DATA;
    TxMessage.IDE = CAN_ID_STD;;
    TxMessage.DLC = SEND_SIZE_8;
    TxMessage.Data[0] = 0x14;
    TxMessage.Data[1] = 0x00;
    TxMessage.Data[2] = 0x00;
    TxMessage.Data[3] = 0x81;
    TxMessage.Data[4] = 0x00;
    TxMessage.Data[5] = 0x00;
    TxMessage.Data[6] = 0xaa;
    TxMessage.Data[7] = 0xf0;
    TransmitMailbox = CAN_Transmit (CAN1,&TxMessage);
    i = 0;
    while((CAN_TransmitStatus (CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending (CAN1,CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
}


#if 0
/*******************************************************************************
* Function Name  : CAN_Polling
* Description    : Configures the CAN and transmit and receive by polling
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
*******************************************************************************/
TestStatus CAN_Polling(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;

    /* CAN register init */
    CAN_DeInit(CAN1);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
    
    //波特率 CAN时钟 = 36MHz / 5 = 7.2MHz, CAN波特率 = 7.2MHz / (1 + 6 + 5) = 500Kb/s
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Prescaler = 6;
    CAN_Init(CAN1, &CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = 0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);
}
#endif


/**********************************************************************
* Function Name  : CAN_Interrupt
* Description    : Configures the CAN and transmit and receive by interruption
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
***********************************************************************/
TestStatus CAN_Interrupt(void)
{
    CAN_InitTypeDef        CAN_InitStructure;
    CAN_FilterInitTypeDef  CAN_FilterInitStructure;
    CanTxMsg TxMessage;
    u32 i = 0;

    /* CAN register init */
    CAN_DeInit(CAN1);

    /* CAN cell init */
    CAN_InitStructure.CAN_TTCM = DISABLE;
    CAN_InitStructure.CAN_ABOM = DISABLE;
    CAN_InitStructure.CAN_AWUM = DISABLE;
    CAN_InitStructure.CAN_NART = DISABLE;
    CAN_InitStructure.CAN_RFLM = DISABLE;
    CAN_InitStructure.CAN_TXFP = DISABLE;
    CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

    // 波特率 CAN时钟 = 36MHz / 6 = 6MHz, CAN波特率 = 6MHz / (1 + 6 + 5) = 500Kb/s
    CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
    CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;
    CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;
    CAN_InitStructure.CAN_Prescaler = 6;  

    CAN_Init(CAN1,&CAN_InitStructure);

    /* CAN filter init */
    CAN_FilterInitStructure.CAN_FilterNumber = 0;
    CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
    CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
    CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
    CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
    CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
    CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
    CAN_FilterInit(&CAN_FilterInitStructure);

    /* CAN FIFO0 message pending interrupt enable */ 
    CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);

    return (TestStatus)0;
}


/**********************************************************************
  * 函数名称:   void NVIC_Configuration(void)
  * 功能描述:   CAN设置,系统中断管理设置 
  * 输入参数:   无
  * 输出参数:   无
  * 返 回 值:   无
  * 创 建 者:   彭杰
  * 创建时间:   20140509
  * 修改时间:   
  * 其它说明:   
************************************************************************/
void NVIC_Configuration(void)
{ 
	NVIC_InitTypeDef NVIC_InitStructure;

  	/* Configure the NVIC Preemption Priority Bits */  
  	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

	#ifdef  VECT_TAB_RAM  
	  /* Set the Vector Table base location at 0x20000000 */ 
	  //NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0); 
	#else  /* VECT_TAB_FLASH  */
	  /* Set the Vector Table base location at 0x08000000 */ 
	  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);   
	#endif

	/* enabling interrupt */
    // CAN中断向量
    NVIC_InitStructure.NVIC_IRQChannel = 20;
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  	NVIC_Init(&NVIC_InitStructure);
}


/**********************************************************************
  * 函数名称:   void GPIO_Configuration(void)
  * 功能描述:   设置CAN口的GPIO功能,LED指示的功能
  * 输入参数:   无
  * 输出参数:   无
  * 返 回 值:   无
  * 创 建 者:   彭杰
  * 创建时间:   20140509
  * 修改时间:   
  * 其它说明:   
************************************************************************/
void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

  	/* Configure PD.02, PD.03, PD.04 and PD.07 as Output push-pull */
  	// For STM3210B-LK1 use PD.02 -PC.07
  	// GPD.2为CAN芯片的使能引脚,低电平有效
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_7;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  	GPIO_Init(GPIOE, &GPIO_InitStructure);
    
    
  	/* Configure CAN pin: RX */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);
  
  	/* Configure CAN pin: TX */
  	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
  	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  	GPIO_Init(GPIOD, &GPIO_InitStructure);

	GPIO_PinRemapConfig(GPIO_Remap2_CAN, ENABLE);	   //重影射CAN IO脚到 PD0，PD1
   
    GPIO_SetBits (GPIOE, GPIO_Pin_0);// 关闭四个LED
    GPIO_SetBits (GPIOE, GPIO_Pin_1);
    GPIO_SetBits (GPIOE, GPIO_Pin_2);
    GPIO_SetBits (GPIOE, GPIO_Pin_3);
}


/**********************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler    void CAN1_RX0_IRQHandler(void)
* Description    : This function handles USB Low Priority or CAN RX0 interrupts 
*                  requests.中断服务子程序
* Input          : None
* Output         : None
* Return         : None
************************************************************************/
void USB_LP_CAN_RX0_IRQHandler(void)
{
    // 关闭接收中断
    CAN_ITConfig (CAN1,CAN_IT_FMP0, DISABLE);
    memset (&RxMessage, 0, sizeof (RxMessage));
    
    CAN_Receive(CAN1,CAN_FIFO0, &RxMessage);
    
    switch (RxMessage.Data[0])
    {
        case (0x14):    // 系统状态
            InQueue (&g_tRxQueue_0x14, &RxMessage);
            break;
        case (0x1b):    // 卫星参数接收
            InQueue (&g_tRxQueue_0x1b, &RxMessage);
            break; 
        case (0x1f):    // GPS参数
            InQueue (&g_tRxQueue_0x1f, &RxMessage);
            break; 
        case (0x22):    // 陀螺零点
            PEout(2) = 0;
            break;
        case (0x36):    // 接收角度参数
            InQueue (&g_tRxQueue_0x36, &RxMessage);
            break;
        
        case (0x49):
            InQueue (&g_tRxQueue_0x49, &RxMessage);
            break;
        default :
            break;
         
    }

    // 开中断
    CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);
}


/**********************************************************************
  * 函数名称:   void CanCMD(void)
  * 功能描述:   CAN通信函数
  * 输入参数:   无
  * 输出参数:   无
  * 返 回 值:   无
  * 创 建 者:   彭杰
  * 创建时间:   20140508
  * 修改时间:   
  * 其它说明:   
************************************************************************/
void CanCMD(void)
{
    int i = 0;
    CanRxMsg tMegTemp;
	// CAN口GPIO设置
    GPIO_Configuration ();
    // CAN中断设置
	NVIC_Configuration();
	// CAN波特率,过滤器设置,打开中断,并等待接收中断
    CAN_Interrupt ();   
	// 初始化接收队列
    InitQueue(&g_tRxQueue_0x14);
    InitQueue(&g_tRxQueue_0x1b);
    InitQueue(&g_tRxQueue_0x1f);
    InitQueue(&g_tRxQueue_0x36);
    InitQueue(&g_tRxQueue_0x49);
    
    // 因数据在CAN上面以2ms一帧的速率传输,20ms会收到10帧数据,进入到队列里面,20ms检查一次接收队列,并处理
	while (1)
    {
        //CANTransmitTest ();
        if (!OutQueue (&g_tRxQueue_0x14, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x1b, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x1f, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x36, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        if (!OutQueue (&g_tRxQueue_0x49, &tMegTemp))
        {
          RxDataCmdPro (tMegTemp);
        }
        OSTimeDly (20);
    }
}