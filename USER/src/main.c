#include <includes.h>

u8 g_ucConnectMode      = 1;            // 1为联机模式,其他为单机测试模式
u8 g_ucIsSetting        = 0;            // 如果有人在操作界面的时候,新来的报警都处于后台显示,当处理完成之后
u8 g_ucIsUpdateMenu     = 0;            // 更新显示
u8 g_ucCurDlg           = 0;            // 当前显示的菜单ID
u8 g_ucHighLightRow     = 0;            // 当前显示的菜单需要高亮的行
u8 g_ucCurID            = 1;            // 当前通信设备的号
u8 g_ucIsNewWarningCode = 0;            // 有新的报警,再次更新界面,在同一时候,有多个未处理的报警
u8 g_ucUpWorkingID      = 1;            // 上工位工作卡机号
u8 g_ucUpBackingID      = 2;            // 上工位备用卡机号
u8 g_ucDownWorkingID    = 3;            // 下工位工作卡机号
u8 g_ucDownBackingID    = 4;            // 下工位备用卡机号
u8 g_ucaCardIsReady[4]  = {0, 0, 0, 0}; // 卡就绪
u8 g_ucaFaultCode[4]    = {0, 0, 0, 0}; // 卡机是否有未处理的故障
u8 g_ucaDeviceIsSTBY[4] = {1, 1, 1, 1}; // 上或下两个卡机处于待机(Standby)状态下,按键按下,主机收到两条按键信息,此时只处理主机的,如果只收到一条按键信息,则直接发卡
u8 g_ucaMechineExist[4] = {0, 0, 0, 0}; // 卡机是否存在并通信正常
u8 g_ucaHasBadCard[4]  = {0, 0, 0, 0};  // 有坏卡
u8 g_ucRepeatKeyMechine = 0;             // 如果连续出现坏卡,则记录即将发卡的卡机,等待500ms之后,再次检测卡机是否就绪并上报状态
u8 g_ucBadCardCount   = 0;               // 如果连续出现4张坏卡,则记录即将发卡的卡机,则不再发卡


CanQueue  g_tCanRxQueue = {0};        // CAN接收卡机数据队列
UartQueue g_tUARTRxQueue = {0};       // UART接收PC机数据队列
CanRxMsg  g_tCanRxMsg = {0};          // CAN数据出队元素
u8 g_ucaUartRxMsg[50] = {0};          // UART数据出队元素


typedef enum {FALSE = 0, TRUE = !FALSE} STATE;


//要写入到STM32 FLASH的字符串数组
//const u8 TEXT_Buffer[]={1};

void bspInit( void )
{
    int i = 0;
    delayInit();                                                                // 定时函数
    for (i = 0; i < 20; i++)    // 初始化时间延时20秒,避免卡机没有初始化完成
    {
        delayMs (1000);
    }
    LED_Init();                                                                 // 初始化 LED
    antGPIOInit();  // 天线切换引脚初始化

    uartInitQueue( &g_tUARTRxQueue);                                            // 初始化 USART1
    USART1_Config();

    //USART4_Config ();         // 初始化 USART4
    DAC_init();
    matrixKeyboardInit();
    lcdInit();
    canInitQueue( &g_tCanRxQueue );
    canInit();                                                                  // 初始化CAN通信

    generalTIM2Init();          // 定时器初始化,2s定时上报状态信息
    generalTIM3Init();          // 定时器初始化,30s定时无按键按下,退回到主界面
    //I2C_Configuration();

    IWDG_Init( 6, 625 );                                                        // 分频数为256,重载值为625,溢出时间为4s   (1/40000)* 256 * 625  = 4s          40000代表着独立看门狗的RC振荡器为40KHz
}
void lcdRef()
{
    u8 key = KEY_NUL;

    if ( ( g_ucKeyValues != KEY_NUL ) || g_ucIsUpdateMenu )
    {
        g_ucIsUpdateMenu = 0;

        if ( 0 == g_ucIsSetting )
        {
            if ( ( g_ucaFaultCode[0] != 0 ) || ( g_ucaFaultCode[1] != 0 ) || ( g_ucaFaultCode[2] != 0 ) || ( g_ucaFaultCode[3] != 0 ) )
            {
                if ( ( g_ucIsNewWarningCode == 1 ) || ( g_ucCurDlg != DLG_FAULT_CODE ) || ( g_ucKeyValues == KEY_CANCEL ) )
                {
                    doShowFaultCode( DLG_FAULT_CODE, 5, NULL );
                }
                g_ucIsNewWarningCode = 0;
                g_ucKeyValues = KEY_NUL;
                return;
            }
        }

        switch ( g_ucCurDlg )
        {
            case DLG_STATUS:
                doShowStatusMenu( DLG_STATUS, 5, NULL ); // 显示主界面菜单,当前状态
                break;

            case DLG_MAIN:
                doShowMainMenu( DLG_MAIN, 0, NULL );    // 进入设置状态
                break;

            case DLG_WORKING_SET:
                doShowWorkingSet( DLG_WORKING_SET, 1, NULL );
                break;

            case DLG_STATUS_ONE:
                doShowStatusOne( DLG_STATUS_ONE, 5, NULL );
                break;

            case DLG_STATUS_TWO:
                doShowStatusTwo( DLG_STATUS_TWO, 5, NULL );
                break;

            case DLG_CARD_COUNT_SET:
                doShowCardCountSet( DLG_CARD_COUNT_SET, 0, NULL );
                break;

            case DLG_DEBUG_MAIN:
                doShowDebugMain( DLG_DEBUG_MAIN, 0, NULL );
                break;

            case DLG_DEBUG_ONE:
                doShowDebugOne( DLG_DEBUG_ONE, 5, NULL );
                break;

            case DLG_DEBUG_TWO:
                doShowDebugTwo( DLG_DEBUG_TWO, 5, NULL );
                break;

            case DLG_CONNETCT_SET:
                doShowConnectModeSet (DLG_CONNETCT_SET, 5, NULL);
                break;

            default:
                break;
        }
    }
}
int main( void )
{
    u8 ret = 0;

    bspInit();

    printf ("%s","你好,欢迎使用乐为电子板卡系统");

    doShowStatusMenu( DLG_STATUS, 5, NULL );                                    // 显示菜单,需要反显示的行

    STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)&g_ucConnectMode,1);                    // 获取g_ucConnectMode值,默认为上位机离线发卡模式

    g_dlg[check_menu(DLG_CONNETCT_SET)].highLightRow = g_ucConnectMode == 1 ? 1: 2;    // 出厂为离线发卡

    myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL ); // 设置工作态
    myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL ); // 设置备用态
    myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL ); // 设置工作态
    myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL ); // 设置备用态

    myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, 0 ); // 设置工作态
    myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, 0 ); // 设置备用态
    myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, 0 ); // 设置工作态
    myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, 0 ); // 设置备用态

    myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
    myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
    myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
    myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡

    printf ("%s",( char * ) &g_tCardMechinePowerOnFrame);                   // 上电初始化

    // 使能计数器
    TIM_Cmd(GENERAL_TIM2, ENABLE);
    // 使能计数器
    TIM_Cmd(GENERAL_TIM3, ENABLE);

    while ( 1 )
    {
        ret = canOutQueue( &g_tCanRxQueue, &g_tCanRxMsg );

        if ( 0 == ret )
        {
            analyzeCANFrame( g_tCanRxMsg );
        }

        memset ( g_ucaUartRxMsg,0,50 );
        ret = uartOutQueue( &g_tUARTRxQueue, g_ucaUartRxMsg );

        if ( 0 == ret )
        {
            analyzeUartFrame( g_ucaUartRxMsg, strlen( (const char *)g_ucaUartRxMsg ) );
        }

        matrixUpdateKey();          // 扫描按键
        lcdRef();                   // 刷新显示
        IWDG_Feed();                // 如果没有产生硬件错误,喂狗,以防硬件问题造成的司机,程序无响应
        delayMs (10);
    }
}

