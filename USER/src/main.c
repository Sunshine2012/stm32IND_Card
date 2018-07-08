#include <includes.h>

u8 g_ucConnectMode      = 1;            // 1为联机模式,其他为单机测试模式
u8 g_ucIsSetting        = 0;            // 如果有人在操作界面的时候,新来的报警都处于后台显示,当处理完成之后再显示
u8 g_ucIsUpdateMenu     = 0;            // 更新显示
u8 g_ucCurDlg           = DLG_STATUS;   // 当前显示的菜单ID
u8 g_ucHighLightRow     = 0;            // 当前显示的菜单需要高亮的行
u8 g_ucCurID            = 1;            // 当前通信设备的号
u8 g_ucIsNewWarningCode = 0;            // 有新的报警,再次更新界面,在同一时候,有多个未处理的报警
u8 g_ucUpWorkingID      = 1;            // 上工位工作卡机号
u8 g_ucUpBackingID      = 2;            // 上工位备用卡机号
u8 g_ucDownWorkingID    = 3;            // 下工位工作卡机号
u8 g_ucDownBackingID    = 4;            // 下工位备用卡机号
u8 g_ucCurOutCardId     = 0;            // 当前出卡的卡机号
u8 g_ucLockPressKey     = 0;            // 按键锁定
u8 g_ucRepeatKeyMechine = 0;            // 如果连续出现坏卡,则记录即将发卡的卡机,等待500ms之后,再次检测卡机是否就绪并上报状态
u8 g_ucBadCardCount     = 0;            // 如果连续出现4张坏卡,则记录即将发卡的卡机,则不再发卡
u8 g_ucaCardIsReady[4]  = {0, 0, 0, 0}; // 卡就绪
u8 g_ucaFaultCode[4]    = {0, 0, 0, 0}; // 卡机是否有未处理的故障
u8 g_ucaDeviceStatus[4] = {0, 0, 0, 0}; // 上或下两个卡机处于待机(Standby)状态下,按键按下,主机收到两条按键信息,此时只处理主机的,如果只收到一条按键信息,则直接发卡
u8 g_ucaMechineExist[4] = {0, 0, 0, 0}; // 卡机是否存在并通信正常
u8 g_ucaHasBadCard[4]  = {0, 0, 0, 0};  // 有坏卡
u8 g_ucaMasterStandbyStatus[4]  = {0, 0, 0, 0};  // 卡机的主备机状态
u8 g_ucaStatus[4]  = {0xaa, 0xaa, 0xaa, 0xaa};      // 卡机的工作状态

u8 g_ucP_RsctlFrame = 0;                 // 收到一帧正确的数据
u8 g_ucIsUpdateMsgFlag = 0;              // 向PC上报卡机消息标志位
u8 g_ucIsCycleMsgFlag = 0;               // 向卡机发送定时查询消息标志位

u8 g_ucKeyPressCount = 0;                // 在2秒钟连续按键6次,则重启设备

CanQueue  g_tCanRxQueue = {0};        // CAN接收卡机数据队列
CanQueue  g_tCanTxQueue = {0};        // CAN发送给卡机数据队列

UartQueue g_tUARTRxQueue = {0};       // UART接收PC机数据队列
UartQueue g_tUARTTxQueue = {0};       // UART发送给PC机数据队列

CanRxMsg  g_tCanRxMsg = {0};          // CAN接收数据出队元素
CanRxMsg  g_tCanTxMsg = {0};          // CAN发送数据出队元素

u8 g_ucaUartRxMsg[50] = {0};          // UART接收数据出队元素
u8 g_ucaUartTxMsg[50] = {0};          // UART接收数据出队元素

u32 g_uiSerNum = 0;     // 帧序号,全局,卡机与主机之间的帧序号
u32 g_uiSerNumPC = 0;   // 帧序号,全局,PC与主机之间的帧序号
u32 g_uiCurNum = 0;     // 作为当前帧号的比对,如果帧序号不对,则代表数据丢失


typedef enum {FALSE = 0, TRUE = !FALSE} STATE;


//要写入到STM32 FLASH的字符串数组
//const u8 TEXT_Buffer[]={1};

void bspInit( void )
{
    int i = 0;
    delayInit();                                                                // 定时函数

    LED_Init();                                                                 // 初始化 LED
    antGPIOInit();  // 天线切换引脚初始化

    uartInitQueue( &g_tUARTRxQueue);
    uartInitQueue( &g_tUARTTxQueue);
    USART1_Config();                                                            // 初始化 USART1

    //USART4_Config ();         // 初始化 USART4
    //DAC_init();
    matrixKeyboardInit();
    lcdInit();
    canInitQueue( &g_tCanRxQueue );
    canInitQueue( &g_tCanTxQueue );
    canInit();                                                                  // 初始化CAN通信

    generalTIM2Init();                                                          // 定时器2初始化
    generalTIM3Init();                                                          // 定时器3初始化
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
            if ( ( g_ucaFaultCode[0] != 0 ) || ( g_ucaFaultCode[1] != 0 ) \
              || ( g_ucaFaultCode[2] != 0 ) || ( g_ucaFaultCode[3] != 0 ) )
            {
                if ( ( g_ucIsNewWarningCode == 1 ) || ( g_ucCurDlg != DLG_FAULT_CODE ) \
                  || ( g_ucKeyValues == KEY_CANCEL ) || ( g_ucKeyValues == KEY_ENTRY ) )
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
                doShowStatusMenu ( DLG_STATUS, 5, NULL ); // 显示主界面菜单,当前状态
                break;

            case DLG_EMPLOYEE_MENU:
                doShowEmployeeMenu ( DLG_EMPLOYEE_MENU, 5, NULL ); // 显示主界面菜单,当前状态
                break;

            case DLG_MAIN:
                doShowMainMenu ( DLG_MAIN, 0, NULL );    // 进入设置状态
                break;

            /*case DLG_WORKING_SET:
                doShowWorkingSet ( DLG_WORKING_SET, 1, NULL );
                break;
            */
            case DLG_STATUS_ONE:
                doShowStatusOne ( DLG_STATUS_ONE, 5, NULL );
                break;

            case DLG_STATUS_TWO:
                doShowStatusTwo ( DLG_STATUS_TWO, 5, NULL );
                break;

            case DLG_CARD_COUNT_SET:
                doShowCardCountSet ( DLG_CARD_COUNT_SET, 0, NULL );
                break;

            case DLG_DEBUG_MAIN:
                doShowDebugMain ( DLG_DEBUG_MAIN, 0, NULL );
                break;

            case DLG_DEBUG_ONE:
                doShowDebugOne ( DLG_DEBUG_ONE, 5, &(g_dlg[check_menu(DLG_DEBUG_MAIN)]) );
                break;

            case DLG_DEBUG_TWO:
                doShowDebugTwo ( DLG_DEBUG_TWO, 5, &(g_dlg[check_menu(DLG_DEBUG_MAIN)]) );
                break;

            case DLG_DEBUG_THREE:
                doShowDebugThree ( DLG_DEBUG_THREE, 5, &(g_dlg[check_menu(DLG_DEBUG_MAIN)]) );
                break;

            case DLG_CONNETCT_SET:
                doShowConnectModeSet (DLG_CONNETCT_SET, 5, NULL);
                break;

            default:
                break;
        }
    }
}


void updateMsg(void)
{

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

    uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardMechineStatusFrame ); // 不考虑竞争,所以不设置自旋锁
}

int main( void )

{
    u8 ret = 0;
    u8 i = 0;
    u8 version[50] = {0};
    bspInit();

    STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)&g_ucConnectMode,1);                    // 获取g_ucConnectMode值,默认为上位机离线发卡模式

    g_dlg[check_menu(DLG_CONNETCT_SET)].highLightRow = g_ucConnectMode == 1 ? 1: 2;    // 出厂为离线发卡

    //myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
    //delayMs (1000);
    //myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
    //delayMs (1000);
    //myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
    //delayMs (1000);
    //myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡

    sprintf (version, "the version is %s,%s\n", __DATE__,__TIME__); // 打印当前版本号和编译日期

    uartInQueue( &g_tUARTTxQueue, (char *)version ); // 不考虑竞争,所以不设置自旋锁

    uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardMechinePowerOnFrame ); // 上电初始化    不考虑竞争,所以不设置自旋锁

    //delayMs( 100 ); // 等待卡机回复
    //for ( i = 0; i < 4; i++)
    //{
    //    if (0 == g_ucaMechineExist[i])
    //    {
    //        myCANTransmit( gt_TxMessage, i + 1, 0, CARD_MACHINE_RESET, 0, 0, 0, NO_FAIL );
    //        g_ucaFaultCode[i] = FAULT_CODE11;
    //    }
    //}

    g_siKeyTime = 100;

    // 使能计数器
    TIM_Cmd(GENERAL_TIM2, ENABLE);
    // 使能计数器
    TIM_Cmd(GENERAL_TIM3, ENABLE);
    //g_siSendToPcMsgTime = 5;
    while ( 1 )
    {

        if ( 1 == g_ucIsUpdateMsgFlag )
        {
            g_ucIsUpdateMsgFlag = 0;
            updateMsg();    // 2秒更新一次上位机数据
        }
#if 0
        if ( 1 == g_ucIsCycleMsgFlag )
        {
            g_ucIsCycleMsgFlag = 0;
            //myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
            delayMs ( 1 );
            //myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
            delayMs ( 1 );
            //myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡
            delayMs ( 1 );
            //myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // 查询是否有卡

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
#endif
        memset ( &g_tCanRxMsg,0,sizeof (g_tCanRxMsg) );
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
    }
}

