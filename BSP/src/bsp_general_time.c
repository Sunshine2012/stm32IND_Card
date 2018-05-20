#include <includes.h>
#include "bsp_general_time.h"

s32 g_siKeyTime = 0;  // 按键状态计时变量
s32 g_siCycleAskMsgTime = 0;    // ms 计时变量
//s32 g_siKeyMsgLockTime = 0;     // 卡机按键上报状态的锁定时间
s32 g_siStatusOverTimeS = 0;    // 发卡机状态短超时
s32 g_siStatusOverTimeL = 0;    // 发卡机状态长超时
s32 g_siKeyPressTime = 0;       // 锁定按键的时间
s32 g_siOutCardMsgTime = 0;     // 重复发送翻卡信息的次数
s32 g_siCardTakeMsgTime = 0;    // 重复发送取卡信息的次数
s32 g_siCheckStatus = 0;        // 检查卡机的状态时间间隔
s32 g_siaCheck[4] = {0};        // 检查卡机的通讯状态时间间隔


/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void  GENERAL_TIM2_IRQHandler (void)
{
    // 2中断一次,上报给上位机状态
    if ( TIM_GetITStatus( GENERAL_TIM2, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(GENERAL_TIM2 , TIM_FLAG_Update);       // 清中断
        g_tCardMechineStatusFrame.CARD_MECHINE1.cardNum[0] = g_uiaInitCardCount[1] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.cardNum[1] = g_uiaInitCardCount[1] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.cardNum[2] = g_uiaInitCardCount[1] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.antHasCard = g_ucaCardIsReady[0] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.status = g_ucaHasBadCard[0] ? '2' : '0';
        g_tCardMechineStatusFrame.CARD_MECHINE1.status = g_ucaFaultCode[0] > 0 ? '1' : '0';

        g_tCardMechineStatusFrame.CARD_MECHINE2.cardNum[0] = g_uiaInitCardCount[2] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.cardNum[1] = g_uiaInitCardCount[2] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.cardNum[2] = g_uiaInitCardCount[2] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.antHasCard = g_ucaCardIsReady[1] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.status = g_ucaHasBadCard[1] ? '2' : '0';
        g_tCardMechineStatusFrame.CARD_MECHINE2.status = g_ucaFaultCode[1] > 0 ? '1' : '0';

        g_tCardMechineStatusFrame.CARD_MECHINE3.cardNum[0] = g_uiaInitCardCount[3] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.cardNum[1] = g_uiaInitCardCount[3] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.cardNum[2] = g_uiaInitCardCount[3] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.antHasCard = g_ucaCardIsReady[2] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.status = g_ucaHasBadCard[2] ? '2' : '0';
        g_tCardMechineStatusFrame.CARD_MECHINE3.status = g_ucaFaultCode[2] > 0 ? '1' : '0';

        g_tCardMechineStatusFrame.CARD_MECHINE4.cardNum[0] = g_uiaInitCardCount[4] / 100 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.cardNum[1] = g_uiaInitCardCount[4] / 10 % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.cardNum[2] = g_uiaInitCardCount[4] % 10 + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.antHasCard = g_ucaCardIsReady[3] + '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.status = g_ucaHasBadCard[3] ? '2' : '0';
        g_tCardMechineStatusFrame.CARD_MECHINE4.status = g_ucaFaultCode[3] > 0 ? '1' : '0';

        g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
        g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
        g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';
        printf ( "%s\n", ( char * ) &g_tCardMechineStatusFrame );// 2秒上报一次系统消息


        if ( g_siCycleAskMsgTime > 0 )
        {
            if ( --g_siCycleAskMsgTime == 0 )
            {
                g_siCycleAskMsgTime = 2;
                myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
                //delayMs ( 1 );
                myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
                //delayMs ( 1 );
                myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
                //delayMs ( 1 );
                myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡

                g_ucaMasterStandbyStatus[0] = 0;
                g_ucaMasterStandbyStatus[1] = 0;
                g_ucaMasterStandbyStatus[2] = 0;
                g_ucaMasterStandbyStatus[3] = 0;
                g_ucaStatus[0] = 0x0a;
                g_ucaStatus[1] = 0x0a;
                g_ucaStatus[2] = 0x0a;
                g_ucaStatus[3] = 0x0a;
                g_siCheckStatus = 5;        // 收到定时轮询的信息之后,50ms发送一次检验主备机的状态
            }
        }

    }
}
/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void  GENERAL_TIM3_IRQHandler (void)
{
    // 10ms中断一次
    static char i = 0;
    if ( TIM_GetITStatus( GENERAL_TIM3, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(GENERAL_TIM3 , TIM_FLAG_Update);       // 清中断

        if ( g_siKeyTime > 0 )
        {
            if ( ( --g_siKeyTime == 0) )//&& ( g_ucIsSetting != 0 )
            {
                g_ucIsSetting = 0;
                g_ucCurDlg = DLG_STATUS;
                g_ucIsUpdateMenu = 1;
            }
        }

        if ( g_siKeyPressTime > 0 )
        {
            if ( --g_siKeyPressTime == 0 )
            {
                g_siKeyPressTime = 200;
                g_ucKeyPressCount = 0;
            }
        }

        if ( g_siStatusOverTimeL > 0)
        {
            if ( --g_siStatusOverTimeL == 0 )
            {
                g_ucaDeviceStatus[0] = 0;
                g_ucaDeviceStatus[1] = 0;
                g_ucaDeviceStatus[2] = 0;
                g_ucaDeviceStatus[3] = 0;
            }
        }
        if ( g_siStatusOverTimeS > 0)
        {
            if ( --g_siStatusOverTimeS == 0 )
            {

                switch (g_ucaDeviceStatus[g_ucCurOutCardId - 1])
                {
                    //case 1:
                    //    printf ( "%s", ( char * ) &g_tCardKeyPressFrame );
                    //    break;
                    case 2:
                        //myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, CARD_IS_BAD, 0, 0, NO_FAIL );
                        break;
                    case 3:
                        if ( g_siOutCardMsgTime > 0)
                        {
                            g_siOutCardMsgTime--;
                            g_siStatusOverTimeL = 1000;
                            g_siStatusOverTimeS = 150;
                            myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                        }
                        break;
                    case 6:
                        if ( g_siCardTakeMsgTime > 0)
                        {
                            g_siCardTakeMsgTime--;
                            g_siStatusOverTimeL = 1000;
                            g_siStatusOverTimeS = 100;
                            g_tCardTakeAwayFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';

                            g_tCardTakeAwayFrame.MECHINE_ID = g_ucCurOutCardId + '0';
                            g_tCardTakeAwayFrame.CARD_MECHINE = g_ucCurOutCardId < 3 ? '1' : '2';
                            printf ( "%s\n", ( char * ) &g_tCardTakeAwayFrame );

                            g_uiCurNum = g_tCardTakeAwayFrame.RSCTL;
                        }
                        else
                        {
                            g_uiCurNum = 0;
                        }
                        break;

                    default:
                        break;
                }

            }
        }
        if ( g_siCheckStatus > 0 )
        {
            if ( --g_siCheckStatus == 0 )
            {
                if ( ( WORKING_STATUS != g_ucaMasterStandbyStatus [0] ) \
                  && ( WORKING_STATUS != g_ucaMasterStandbyStatus [1] ) ) // 都为备机
                {
                    if ( ( g_ucaStatus[0] == 0 )
                      && ( g_ucaStatus[1] == 0 ) )
                    {
                        if ( 1 == g_ucUpWorkingID )
                        {
                            myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL );
                        }
                        else if ( 2 == g_ucUpWorkingID )
                        {
                            myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL );
                        }
                    }

                }
                if ( ( WORKING_STATUS != g_ucaMasterStandbyStatus [2] ) \
                  && ( WORKING_STATUS != g_ucaMasterStandbyStatus [3] ) ) // 都为备机
                {
                    if ( ( g_ucaStatus[2] == 0 )
                      && ( g_ucaStatus[3] == 0 ) )
                    {
                        if ( 3 == g_ucDownWorkingID )
                        {
                            myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL );
                        }
                        else if ( 4 == g_ucDownWorkingID )
                        {
                            myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                            myCANTransmit ( gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL );
                        }
                    }

                }
            }
        }
        for ( i = 0; i < 4; i++)
        {
            if ( g_siaCheck[i] > 0)
            {
               if ( --g_siaCheck[i] == 0 )
               {
                    g_siaCheck[i] = 1200;
                    if ( 0x0a == g_ucaStatus [i] )
                    {
                        myCANTransmit( gt_TxMessage, i + 1, 0, CARD_MACHINE_RESET, 0, 0, 0, NO_FAIL );

                        g_ucaFaultCode[i] = FAULT_CODE11;
                        g_ucIsNewWarningCode = 1;
                        g_ucIsSetting = 0;
                        g_ucIsUpdateMenu = 1;

                        switch ( i + 1 )
                        {
                            case 1:
                                if ( (g_ucaFaultCode[1] == 0) && (g_ucaMechineExist[1] == 1) )   // 无故障,且通信正常
                                {
                                    g_ucaMechineExist[0] = 0;
                                    g_ucaMechineExist[1] = 0;
                                    g_ucUpWorkingID     = 2;
                                    g_ucUpBackingID     = 1;
                                    myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                    myCANTransmit ( gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                                }
                                break;
                            case 2:
                                if ( (g_ucaFaultCode[0] == 0) && (g_ucaMechineExist[0] == 1) )   // 无故障,且通信正常
                                {
                                    g_ucaMechineExist[0] = 0;
                                    g_ucaMechineExist[1] = 0;
                                    g_ucUpWorkingID     = 1;
                                    g_ucUpBackingID     = 2;
                                    myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                    myCANTransmit ( gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                                }
                                break;
                            case 3:
                                if ( (g_ucaFaultCode[3] == 0) && (g_ucaMechineExist[3] == 1) )   // 无故障,且通信正常
                                {
                                    g_ucaMechineExist[2] = 0;
                                    g_ucaMechineExist[3] = 0;
                                    g_ucDownWorkingID   = 4;
                                    g_ucDownBackingID   = 3;
                                    myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                    myCANTransmit ( gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                                }
                                break;
                            case 4:
                                if ( (g_ucaFaultCode[2] == 0) && (g_ucaMechineExist[2] == 1) )   // 无故障,且通信正常
                                {
                                    g_ucaMechineExist[2] = 0;
                                    g_ucaMechineExist[3] = 0;
                                    g_ucDownWorkingID   = 3;
                                    g_ucDownBackingID   = 4;
                                    myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                    myCANTransmit ( gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态

                                 }
                                break;
                            default:
                                break;
                        }
                    }
                }
            }
        }

    }
}

// 中断优先级配置
static void GENERAL_TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ ;
    // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *  TIM_CounterMode          TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */
static void GENERAL_TIM2_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // 开启定时器时钟,即内部时钟CK_INT=72M
    GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK, ENABLE);
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM2_Period;
    // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM2_Prescaler;
    // 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM2, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(GENERAL_TIM2, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(GENERAL_TIM2,TIM_IT_Update,ENABLE);

    // 使能计数器
    // TIM_Cmd(GENERAL_TIM2, ENABLE);
}

// 中断优先级配置
static void GENERAL_TIM3_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // 设置中断组为0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM3_IRQ ;
    // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // 设置抢占优先级为3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * 注意：TIM_TimeBaseInitTypeDef结构体里面有5个成员，TIM6和TIM7的寄存器里面只有
// * TIM_Prescaler和TIM_Period，所以使用TIM6和TIM7的时候只需初始化这两个成员即可，
// * 另外三个成员是通用定时器和高级定时器才有.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            都有
// *  TIM_CounterMode          TIMx,x[6,7]没有，其他都有
// *  TIM_Period               都有
// *  TIM_ClockDivision        TIMx,x[6,7]没有，其他都有
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]才有
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */
static void GENERAL_TIM3_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // 开启定时器时钟,即内部时钟CK_INT=72M
    GENERAL_TIM3_APBxClock_FUN(GENERAL_TIM3_CLK, ENABLE);
    // 自动重装载寄存器的值，累计TIM_Period+1个频率后产生一个更新或者中断
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM3_Period;
    // 时钟预分频数
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM3_Prescaler;
    // 时钟分频因子 ，没用到不用管
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // 计数器计数模式，设置为向上计数
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // 重复计数器的值，没用到不用管
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // 初始化定时器
    TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);

    // 清除计数器中断标志位
    TIM_ClearFlag(GENERAL_TIM3, TIM_FLAG_Update);

    // 开启计数器中断
    TIM_ITConfig(GENERAL_TIM3,TIM_IT_Update,ENABLE);

    // 使能计数器
    // TIM_Cmd(GENERAL_TIM3, ENABLE);
}

void generalTIM2Init(void)
{
    GENERAL_TIM2_NVIC_Config();
    GENERAL_TIM2_Mode_Config();
}
void generalTIM3Init(void)
{
    GENERAL_TIM3_NVIC_Config();
    GENERAL_TIM3_Mode_Config();
}

/*********************************************END OF FILE**********************/
