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
s32 g_siSendToPcMsgTime = 0;    // 通过串口发送数据的间隔

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
        g_ucIsUpdateMsgFlag = 1;

        if ( g_siCycleAskMsgTime > 0 )
        {
            if ( --g_siCycleAskMsgTime == 0 )
            {
                g_siCycleAskMsgTime = 2;
                g_ucIsCycleMsgFlag = 1;
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

        if ( g_siSendToPcMsgTime > 0)
        {
           if ( --g_siSendToPcMsgTime == 0 )
           {
                g_siSendToPcMsgTime = 10;
                memset ( g_ucaUartTxMsg,0,50 );
                if ( 0 == uartOutQueue( &g_tUARTTxQueue, g_ucaUartTxMsg ) )
                {
                    USART1_SendStringFromDMA( (char *)g_ucaUartTxMsg, strlen( (const char *)g_ucaUartTxMsg ) );
                }
            }
        }

        if ( g_siKeyTime > 0 )
        {
            if ( ( --g_siKeyTime == 0) )    // && ( g_ucIsSetting != 0 )
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
                            g_siStatusOverTimeS = 300;
                            myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                        }
                        break;
                    case 6:
                        /*
                        if ( g_siCardTakeMsgTime > 0)
                        {
                            g_siCardTakeMsgTime--;
                            g_siStatusOverTimeL = 1000;
                            g_siStatusOverTimeS = 100;
                            g_tCardTakeAwayFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';

                            g_tCardTakeAwayFrame.MECHINE_ID = g_ucCurOutCardId + '0';
                            g_tCardTakeAwayFrame.CARD_MECHINE = g_ucCurOutCardId < 3 ? '1' : '2';
                            uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardTakeAwayFrame ); // 不考虑竞争,所以不设置自旋锁

                            g_uiCurNum = g_tCardTakeAwayFrame.RSCTL;
                        }
                        else
                        {
                            g_uiCurNum = 0;
                        }
                        */


                    default:
                        break;
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
                        //myCANTransmit( gt_TxMessage, i + 1, 0, CARD_MACHINE_RESET, 0, 0, 0, NO_FAIL );

                        g_ucaFaultCode[i] = FAULT_CODE11;
                        g_ucIsNewWarningCode = 1;
                        g_ucIsSetting = 0;
                        g_ucIsUpdateMenu = 1;

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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // 设置中断来源
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ ;
    // 设置主优先级为 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // 设置抢占优先级为2
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
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
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
