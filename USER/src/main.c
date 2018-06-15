#include <includes.h>

u8 g_ucConnectMode      = 1;            // 1Ϊ����ģʽ,����Ϊ��������ģʽ
u8 g_ucIsSetting        = 0;            // ��������ڲ��������ʱ��,�����ı��������ں�̨��ʾ,���������֮������ʾ
u8 g_ucIsUpdateMenu     = 0;            // ������ʾ
u8 g_ucCurDlg           = DLG_STATUS;   // ��ǰ��ʾ�Ĳ˵�ID
u8 g_ucHighLightRow     = 0;            // ��ǰ��ʾ�Ĳ˵���Ҫ��������
u8 g_ucCurID            = 1;            // ��ǰͨ���豸�ĺ�
u8 g_ucIsNewWarningCode = 0;            // ���µı���,�ٴθ��½���,��ͬһʱ��,�ж��δ����ı���
u8 g_ucUpWorkingID      = 1;            // �Ϲ�λ����������
u8 g_ucUpBackingID      = 2;            // �Ϲ�λ���ÿ�����
u8 g_ucDownWorkingID    = 3;            // �¹�λ����������
u8 g_ucDownBackingID    = 4;            // �¹�λ���ÿ�����
u8 g_ucCurOutCardId     = 0;            // ��ǰ�����Ŀ�����
u8 g_ucLockPressKey     = 0;            // ��������
u8 g_ucRepeatKeyMechine = 0;            // ����������ֻ���,���¼���������Ŀ���,�ȴ�500ms֮��,�ٴμ�⿨���Ƿ�������ϱ�״̬
u8 g_ucBadCardCount     = 0;            // �����������4�Ż���,���¼���������Ŀ���,���ٷ���
u8 g_ucaCardIsReady[4]  = {0, 0, 0, 0}; // ������
u8 g_ucaFaultCode[4]    = {0, 0, 0, 0}; // �����Ƿ���δ����Ĺ���
u8 g_ucaDeviceStatus[4] = {0, 0, 0, 0}; // �ϻ��������������ڴ���(Standby)״̬��,��������,�����յ�����������Ϣ,��ʱֻ����������,���ֻ�յ�һ��������Ϣ,��ֱ�ӷ���
u8 g_ucaMechineExist[4] = {0, 0, 0, 0}; // �����Ƿ���ڲ�ͨ������
u8 g_ucaHasBadCard[4]  = {0, 0, 0, 0};  // �л���
u8 g_ucaMasterStandbyStatus[4]  = {0, 0, 0, 0};  // ������������״̬
u8 g_ucaStatus[4]  = {0x0a, 0x0a, 0x0a, 0x0a};      // �����Ĺ���״̬

u8 g_ucP_RsctlFrame = 0;                 // �յ�һ֡��ȷ������
u8 g_ucIsUpdateMsgFlag = 0;              // ��PC�ϱ�������Ϣ��־λ
u8 g_ucIsCycleMsgFlag = 0;               // �򿨻����Ͷ�ʱ��ѯ��Ϣ��־λ

u8 g_ucKeyPressCount = 0;                // ��2������������6��,�������豸

CanQueue  g_tCanRxQueue = {0};        // CAN���տ������ݶ���
UartQueue g_tUARTRxQueue = {0};       // UART����PC�����ݶ���
CanRxMsg  g_tCanRxMsg = {0};          // CAN���ݳ���Ԫ��
u8 g_ucaUartRxMsg[50] = {0};          // UART���ݳ���Ԫ��

u32 g_uiSerNum = 0;     // ֡���,ȫ��,����������֮���֡���
u32 g_uiSerNumPC = 0;   // ֡���,ȫ��,PC������֮���֡���
u32 g_uiCurNum = 0;     // ��Ϊ��ǰ֡�ŵıȶ�,���֡��Ų���,��������ݶ�ʧ


typedef enum {FALSE = 0, TRUE = !FALSE} STATE;


//Ҫд�뵽STM32 FLASH���ַ�������
//const u8 TEXT_Buffer[]={1};

void bspInit( void )
{
    int i = 0;
    delayInit();                                                                // ��ʱ����
    for (i = 0; i < 20; i++)    // ��ʼ��ʱ����ʱ20��,���⿨��û�г�ʼ�����
    {
        //delayMs (1000);
    }
    LED_Init();                                                                 // ��ʼ�� LED
    antGPIOInit();  // �����л����ų�ʼ��

    uartInitQueue( &g_tUARTRxQueue);                                            // ��ʼ�� USART1
    USART1_Config();

    //USART4_Config ();         // ��ʼ�� USART4
    DAC_init();
    matrixKeyboardInit();
    lcdInit();
    canInitQueue( &g_tCanRxQueue );
    canInit();                                                                  // ��ʼ��CANͨ��

    generalTIM2Init();          // ��ʱ����ʼ��,2s��ʱ�ϱ�״̬��Ϣ
    generalTIM3Init();          // ��ʱ����ʼ��,30s��ʱ�ް�������,�˻ص�������
    //I2C_Configuration();

    IWDG_Init( 6, 625 );                                                        // ��Ƶ��Ϊ256,����ֵΪ625,���ʱ��Ϊ4s   (1/40000)* 256 * 625  = 4s          40000�����Ŷ������Ź���RC����Ϊ40KHz
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
                doShowStatusMenu ( DLG_STATUS, 5, NULL ); // ��ʾ������˵�,��ǰ״̬
                break;

            case DLG_EMPLOYEE_MENU:
                doShowEmployeeMenu ( DLG_EMPLOYEE_MENU, 5, NULL ); // ��ʾ������˵�,��ǰ״̬
                break;

            case DLG_MAIN:
                doShowMainMenu ( DLG_MAIN, 0, NULL );    // ��������״̬
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

    USART1_SendStringFromDMA ((char *)&g_tCardMechineStatusFrame , strlen ((char *)&g_tCardMechineStatusFrame)); // 2���ϱ�һ��ϵͳ��Ϣ

}

int main( void )

{
    u8 ret = 0;
    u8 i = 0;

    bspInit();

    STMFLASH_Read(FLASH_SAVE_ADDR,(u16*)&g_ucConnectMode,1);                    // ��ȡg_ucConnectModeֵ,Ĭ��Ϊ��λ�����߷���ģʽ

    g_dlg[check_menu(DLG_CONNETCT_SET)].highLightRow = g_ucConnectMode == 1 ? 1: 2;    // ����Ϊ���߷���

    //myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�
    //delayMs (1000);
    //myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�
    //delayMs (1000);
    //IWDG_Feed();                // ���û�в���Ӳ������,ι��,�Է�Ӳ��������ɵ�˾��,��������Ӧ
    //myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�
    //delayMs (1000);
    //myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�
    //printf ("%s\n","���, ��ӭʹ����Ϊ���Ӱ忨ϵͳ");
    //delayMs (1000);
    printf ("the code version %s,%s\n", __DATE__,__TIME__); // ��ӡ��ǰ�汾�źͱ�������

    //printf ("%s\n",( char * ) &g_tCardMechinePowerOnFrame);                   // �ϵ��ʼ��
    USART1_SendStringFromDMA ((char *)&g_tCardMechinePowerOnFrame , strlen ((char *)&g_tCardMechinePowerOnFrame)); // ������Ϣ

    //delayMs( 100 ); // �ȴ������ظ�
    //for ( i = 0; i < 4; i++)
    //{
    //    if (0 == g_ucaMechineExist[i])
    //    {
    //        myCANTransmit( gt_TxMessage, i + 1, 0, CARD_MACHINE_RESET, 0, 0, 0, NO_FAIL );
    //        g_ucaFaultCode[i] = FAULT_CODE11;
    //    }
    //}

    g_siKeyTime = 100;

    //g_siaCheck[0] = 1200;
    //g_siaCheck[1] = 1200;
    //g_siaCheck[2] = 1200;
    //g_siaCheck[3] = 1200;

    //g_siCycleAskMsgTime = 2;      // 4���ѯһ�ο���״̬
    // ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM2, ENABLE);
    // ʹ�ܼ�����
    TIM_Cmd(GENERAL_TIM3, ENABLE);

    while ( 1 )
    {

        if ( 1 == g_ucP_RsctlFrame )
        {
            g_ucP_RsctlFrame = 0;
            USART1_SendStringFromDMA ((char *)&g_tP_RsctlFrame , strlen ((char *)&g_tP_RsctlFrame));
        }

        if ( 1 == g_ucIsUpdateMsgFlag )
        {
            g_ucIsUpdateMsgFlag = 0;
            updateMsg();    // 2�����һ����λ������
        }
#if 0
        if ( 1 == g_ucIsCycleMsgFlag )
        {
            g_ucIsCycleMsgFlag = 0;
            //myCANTransmit( gt_TxMessage, g_ucUpWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�
            delayMs ( 1 );
            //myCANTransmit( gt_TxMessage, g_ucUpBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�
            delayMs ( 1 );
            //myCANTransmit( gt_TxMessage, g_ucDownWorkingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�
            delayMs ( 1 );
            //myCANTransmit( gt_TxMessage, g_ucDownBackingID, 0, CYCLE_ASK, 0, 0, 0, 0 ); // ��ѯ�Ƿ��п�

            g_ucaMasterStandbyStatus[0] = 0;
            g_ucaMasterStandbyStatus[1] = 0;
            g_ucaMasterStandbyStatus[2] = 0;
            g_ucaMasterStandbyStatus[3] = 0;
            g_ucaStatus[0] = 0x0a;
            g_ucaStatus[1] = 0x0a;
            g_ucaStatus[2] = 0x0a;
            g_ucaStatus[3] = 0x0a;
            g_siCheckStatus = 5;        // �յ���ʱ��ѯ����Ϣ֮��,50ms����һ�μ�����������״̬
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

        matrixUpdateKey();          // ɨ�谴��
        lcdRef();                   // ˢ����ʾ
        IWDG_Feed();                // ���û�в���Ӳ������,ι��,�Է�Ӳ��������ɵ�˾��,��������Ӧ

    }
}

