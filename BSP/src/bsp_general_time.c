#include <includes.h>
#include "bsp_general_time.h"

s32 g_siKeyTime = 0;  // ����״̬��ʱ����
s32 g_siCycleAskMsgTime = 0;    // ms ��ʱ����
//s32 g_siKeyMsgLockTime = 0;     // ���������ϱ�״̬������ʱ��
s32 g_siStatusOverTimeS = 0;    // ������״̬�̳�ʱ
s32 g_siStatusOverTimeL = 0;    // ������״̬����ʱ
s32 g_siKeyPressTime = 0;       // ����������ʱ��
s32 g_siOutCardMsgTime = 0;     // �ظ����ͷ�����Ϣ�Ĵ���
s32 g_siCardTakeMsgTime = 0;    // �ظ�����ȡ����Ϣ�Ĵ���
s32 g_siCheckStatus = 0;        // ��鿨����״̬ʱ����
s32 g_siaCheck[4] = {0};        // ��鿨����ͨѶ״̬ʱ����
s32 g_siSendToPcMsgTime = 0;    // ͨ�����ڷ������ݵļ��

/**
  * @brief  This function handles TIM interrupt request.
  * @param  None
  * @retval None
  */
void  GENERAL_TIM2_IRQHandler (void)
{
    // 2�ж�һ��,�ϱ�����λ��״̬
    if ( TIM_GetITStatus( GENERAL_TIM2, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(GENERAL_TIM2 , TIM_FLAG_Update);       // ���ж�
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
    // 10ms�ж�һ��
    static char i = 0;
    if ( TIM_GetITStatus( GENERAL_TIM3, TIM_IT_Update) != RESET )
    {
        TIM_ClearITPendingBit(GENERAL_TIM3 , TIM_FLAG_Update);       // ���ж�

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
                            uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardTakeAwayFrame ); // �����Ǿ���,���Բ�����������

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

// �ж����ȼ�����
static void GENERAL_TIM2_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM2_IRQ ;
    // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // ������ռ���ȼ�Ϊ2
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *  TIM_CounterMode          TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */
static void GENERAL_TIM2_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    GENERAL_TIM2_APBxClock_FUN(GENERAL_TIM2_CLK, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM2_Period;
    // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM2_Prescaler;
    // ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM2, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM2, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(GENERAL_TIM2,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
    // TIM_Cmd(GENERAL_TIM2, ENABLE);
}

// �ж����ȼ�����
static void GENERAL_TIM3_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    // �����ж���Ϊ0
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // �����ж���Դ
    NVIC_InitStructure.NVIC_IRQChannel = GENERAL_TIM3_IRQ ;
    // ���������ȼ�Ϊ 0
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    // ������ռ���ȼ�Ϊ3
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

///*
// * ע�⣺TIM_TimeBaseInitTypeDef�ṹ��������5����Ա��TIM6��TIM7�ļĴ�������ֻ��
// * TIM_Prescaler��TIM_Period������ʹ��TIM6��TIM7��ʱ��ֻ���ʼ����������Ա���ɣ�
// * ����������Ա��ͨ�ö�ʱ���͸߼���ʱ������.
// *-----------------------------------------------------------------------------
// *typedef struct
// *{ TIM_Prescaler            ����
// *  TIM_CounterMode          TIMx,x[6,7]û�У���������
// *  TIM_Period               ����
// *  TIM_ClockDivision        TIMx,x[6,7]û�У���������
// *  TIM_RepetitionCounter    TIMx,x[1,8,15,16,17]����
// *}TIM_TimeBaseInitTypeDef;
// *-----------------------------------------------------------------------------
// */
static void GENERAL_TIM3_Mode_Config(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    // ������ʱ��ʱ��,���ڲ�ʱ��CK_INT=72M
    GENERAL_TIM3_APBxClock_FUN(GENERAL_TIM3_CLK, ENABLE);
    // �Զ���װ�ؼĴ�����ֵ���ۼ�TIM_Period+1��Ƶ�ʺ����һ�����»����ж�
    TIM_TimeBaseStructure.TIM_Period=GENERAL_TIM3_Period;
    // ʱ��Ԥ��Ƶ��
    TIM_TimeBaseStructure.TIM_Prescaler= GENERAL_TIM3_Prescaler;
    // ʱ�ӷ�Ƶ���� ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
    // ����������ģʽ������Ϊ���ϼ���
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;
    // �ظ���������ֵ��û�õ����ù�
    TIM_TimeBaseStructure.TIM_RepetitionCounter=0;
    // ��ʼ����ʱ��
    TIM_TimeBaseInit(GENERAL_TIM3, &TIM_TimeBaseStructure);

    // ����������жϱ�־λ
    TIM_ClearFlag(GENERAL_TIM3, TIM_FLAG_Update);

    // �����������ж�
    TIM_ITConfig(GENERAL_TIM3,TIM_IT_Update,ENABLE);

    // ʹ�ܼ�����
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
