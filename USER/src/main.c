#include <includes.h>

u8 g_ucIsUpdateMenu     = 0;            // 更新显示
u8 g_ucCurDlg           = 0;            // 当前显示的菜单ID
u8 g_ucHighLightRow     = 0;            // 当前显示的菜单需要高亮的行
u8 g_ucCurID            = 1;            // 当前通信设备的号
u8 g_ucIsNewWarningCode = 0;            // 有新的报警,再次更新界面,在同一时候,有多个未处理的报警
u8 g_ucUpWorkingID      = 1;            // 上工位工作卡机号
u8 g_ucUpBackingID      = 2;            // 上工位备用卡机号
u8 g_ucDownWorkingID    = 3;            // 下工位工作卡机号
u8 g_ucDownBackingID    = 4;            // 下工位备用卡机号
u8 g_ucaCardIsReady[4]  = {1, 1, 1, 1}; // 卡就绪
u8 g_ucaFaultCode[4]    = {0, 0, 0, 0}; // 卡机是否有未处理的故障
u8 g_ucaDeviceIsSTBY[4] = {1, 1, 1, 1}; // 上或下两个卡机处于待机(Standby)状态下,按键按下,主机收到两条按键信息,此时只处理主机的,如果只收到一条按键信息,则直接发卡
u8 g_ucaMechineExist[4] = {0, 0, 0, 0}; // 卡机是否存在并通信正常

CanRxQueue g_tRxQueueCan = {0};         // CAN队列
//RxQueue g_tRxQueueUART = {0};           // UART队列
CanRxMsg g_tRxQueueMsg = {0};           // 出队元素

void bspInit (void)
{
    delayInit();                // 定时函数

    LED_Init ();                // 初始化 LED

    USART1_Config ();           // 初始化 USART1

    //USART4_Config ();         // 初始化 USART4

    DAC_init();

    matrix_keyboard_init ();

    lcdInit();

    canInitQueue(&g_tRxQueueCan);

    canInit();                  // 初始化CAN通信

    //generalTIMInit();
}


void lcdRef()
{
    u8 key = KEY_NUL;

    if ((g_ucKeyValues != KEY_NUL) || g_ucIsUpdateMenu)
    {
        g_ucIsUpdateMenu = 0;
        if ((g_ucaFaultCode[0] != 0) || (g_ucaFaultCode[1] != 0)
        || (g_ucaFaultCode[2] != 0) || (g_ucaFaultCode[3] != 0))
        {
            if ((g_ucIsNewWarningCode == 1) || (g_ucCurDlg != DLG_FAULT_CODE) || (g_ucKeyValues == KEY_CANCEL))
            {
                doShowFaultCode (DLG_FAULT_CODE, 5, NULL);
            }
            g_ucIsNewWarningCode = 0;
            g_ucKeyValues = KEY_NUL;
            return;
        }
        switch (g_ucCurDlg)
        {
            case DLG_STATUS:
                doShowStatusMenu(DLG_STATUS, 5, NULL);      // 显示主界面菜单,当前状态
                break;
            case DLG_MAIN:
                doShowMainMenu (DLG_MAIN, 0, NULL);         // 进入设置状态
                break;
            case DLG_WORKING_SET:
                doShowWorkingSet(DLG_WORKING_SET, 1, NULL);
                break;
            case DLG_STATUS_ONE:
                doShowStatusOne (DLG_STATUS_ONE, 5, NULL);
                break;
            case DLG_STATUS_TWO:
                doShowStatusTwo (DLG_STATUS_TWO, 5, NULL);
                break;
            case DLG_CARD_COUNT_SET:
                doShowCardCountSet (DLG_CARD_COUNT_SET, 0, NULL);
                break;
            case DLG_DEBUG_MAIN:
                doShowDebugMain(DLG_DEBUG_MAIN, 0, NULL);
                break;
            case DLG_DEBUG_ONE:
                doShowDebugOne(DLG_DEBUG_ONE, 5, NULL);
                break;
            case DLG_DEBUG_TWO:
                doShowDebugTwo(DLG_DEBUG_TWO, 5, NULL);
                break;
            default:
                break;

        }
    }
}

int main(void)
{
    u8 ret = 0;

    bspInit();

    myCANTransmit(gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // 设置工作态
    myCANTransmit(gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // 设置备用态
    myCANTransmit(gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // 设置工作态
    myCANTransmit(gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // 设置备用态
    memset( &g_tRxQueueMsg, 0 ,sizeof (g_tRxQueueMsg) );
    doShowStatusMenu(DLG_STATUS, 5, NULL);      // 显示菜单,需要反显示的行
    while (1)
    {

        ret = canOutQueue( &g_tRxQueueCan ,&g_tRxQueueMsg );
        if ( 0 == ret )
        {
            analyzeCANFrame(g_tRxQueueMsg);
        }
        //ret = outQueue (&g_);
        if (0 == ret)
        {
            //analyzeUartFrame(u8 argv [ ], u32 size)
        }
        matrix_update_key();                            // 扫描按键
        lcdRef();
    }
}

