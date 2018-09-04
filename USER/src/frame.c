#include <includes.h>
#include <frame.h>
//#include <WAV_C_xiexie.h>
//#include <WAV_C_quka.h>

u32 g_uiaInitCardCount[5]    = {1000, 999, 999, 999, 999};    // ����ʼ����ֵ,[0]Ϊ�ܿ�����,��1�ſ�,��1,[1~4]Ϊÿ��������ʼ������,��1�ſ�,��1.
u32 g_uiaSpitCardCount[5]    = {0, 0, 0, 0, 0};    // ��������,[0]Ϊ����������,��1�ſ�,��1,[1~4]Ϊÿ��������������,��1�ſ�,��1.


u8 g_ucSerNum = '0';  // ֡���   ȫ��

RSCTL_FREME g_tP_RsctlFrame = {'<','0','0','>'};        // ��Ӧ��֡
RSCTL_FREME g_tN_RsctlFrame = {'<','0','1','>'};        // ��Ӧ��֡

/* �����ϵ���Ϣ(41H)֡          4�ֽ� */
CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame = {'<', '0', CARD_MACHINE_POWER_ON, '>'};

/* ״̬��Ϣ(42H)֡             30�ֽ� */
CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame =    {'<', '0', 'B', '1','3',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '>'};

/* �ѳ�����Ϣ(43H)֡            6�ֽ� */
CARD_MECHINE_TO_PC_FRAME        g_tCardSpitOutFrame = {'<', '0', CARD_SPIT_OUT, '1', '1', '>'};

/* ��ťȡ����Ϣ(44H)֡          6�ֽ� */
CARD_MECHINE_TO_PC_FRAME        g_tCardKeyPressFrame = {'<', '0', CARD_KEY_PRESS, '1', '1', '>'};

/* ����ȡ����Ϣ(45H)֡          6�ֽ� */
CARD_MECHINE_TO_PC_FRAME        g_tCardTakeAwayFrame = {'<', '0', CARD_TAKE_AWAY, '1', '1', '>'};

/* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */
CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame = {'<', '0', CARD_REPORT_SPIT_STATUES, '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0', '>'};
/* ��ʼ��������Ϣ(61H)֡        20�ֽ� */
PC_TO_CARD_INIT_FREME           g_tPcToCardInitFrame = {'<', '0', PC_INIT_MECHINE, '9', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0','>'};

/* ������Ϣ(62H)֡              5�ֽ� */
PC_TO_CARD_MECHINE_FRAME        g_tPcSpitOutCardFrame = {'<', '0', PC_SPIT_OUT_CARD, '1', '>'};

/* ������Ϣ(63H)֡              5�ֽ� */
PC_TO_CARD_MECHINE_FRAME        g_tPcBadCardFrame = {'<', '0', PC_BAD_CARD, '1', '>'};

/* ��ѯ����״̬(65H)֡          5�ֽ� */
PC_TO_CARD_MECHINE_FRAME        g_tPcQuetyCardMechineFrame = {'<', '0', PC_QUERY_CARD_MECHINE, '1', '>'};

 /* ��ѯ����(66H)֡              5�ֽ� */
PC_TO_CARD_MECHINE_FRAME        g_tPcQuetyCardCpipFrame = {'<', '0', PC_QUERY_CARD_CLIP, '1', '>'};

/* ���ÿ��п���(67H)֡          8�ֽ�*/
PC_SET_CARD_NUM_FRAME           g_tPcSetCardNumFrame = {'<', '0', PC_SET_CARD_NUM, '1', '9', '9' , '9', '>'};





//#pragma  diag_suppress 870          // ����ʾ����

const Print_msg g_taPri_msg[] = {
                            {'$',                           "/* ��Ч��Ϣ */"},
                            {CARD_MACHINE_POWER_ON,         "/* �����ϵ���Ϣ(41H)֡          4�ֽ� */"},
                            {CARD_MACHINE_STATUES,          "/* ״̬��Ϣ(42H)֡             30�ֽ� */"},
                            {CARD_SPIT_OUT,                 "/* �ѳ�����Ϣ(43H)֡            6�ֽ� */"},
                            {CARD_KEY_PRESS,                "/* ��ťȡ����Ϣ(44H)֡          6�ֽ� */"},
                            {CARD_TAKE_AWAY,                "/* ����ȡ����Ϣ(45H)֡          6�ֽ� */"},
                            {CARD_REPORT_SPIT_STATUES,      "/* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */"},

                            {PC_INIT_MECHINE,               "/* ��ʼ��������Ϣ(61H)֡       20�ֽ� */"},
                            {PC_SPIT_OUT_CARD,              "/* ������Ϣ(62H)֡              5�ֽ� */"},
                            {PC_BAD_CARD,                   "/* ������Ϣ(63H)֡              5�ֽ� */"},
                            {PC_QUERY_CARD_MECHINE,         "/* ��ѯ����״̬(65H)֡          5�ֽ� */"},
                            {PC_QUERY_CARD_CLIP,            "/* ��ѯ����(66H)֡              5�ֽ� */"},
                            {PC_SET_CARD_NUM,               "/* ���ÿ��п���(67H)֡          8�ֽ� */"},
                            {PC_GET_DIST,                   "/* ���(74H)֡                  5�ֽ� */"},
                            {'0',NULL}
                        };

const Print_msg g_taShow_msg[] = {
                            {0,                             "NULL"},
                            {MACHINE_CHECK_CARD,            "�鿨"},
                            {KEY_PRESS,                     "����"},
                            {CARD_SPIT_NOTICE,              "����"},
                            {CARD_TAKE_AWAY_NOTICE,         "ȡ��"},
                            {CARD_IS_READY,                 "������"},
                            {DISCONNECTED,                  "����"},
                            {CONNECTED,                     "      "},
                            {0xff,NULL}
                        };

const Print_msg g_taShowStatus_msg[] = {
                            {0,                             "NULL"},
                            {CARD_IS_OK,                    "�ÿ�"},
                            {CARD_IS_BAD,                   "����"},
                            {IS_WORKING,                    "����"},
                            {IS_BACKING,                    "����"},
                            {CONNECTED,                     "    "},
                            {0xff,NULL}
                        };
const Print_msg g_taShowFaultCode_msg[] = {
                            {0,                             "NULL"},
                            {CARD_IS_OK,                    "�ÿ�"},
                            {CARD_IS_BAD,                   "����"},
                            {CONNECTED,                     "    "},
                            {0xff,NULL}
                        };
// ���ߵ��л�,4������,��Ӧ�Ų�ͬ���߼�,��Ҫͬʱʹ�ܲ�ͬ�ļ̵���
void antSwitch(u8 id)
{
    switch (id)
        {
        case 0:
            macANT_IO1_OFF();
            macANT_IO2_OFF();
            macANT_IO3_OFF();
            macANT_IO4_OFF();
            break;
        case 1:
            macANT_IO1_ON();
            macANT_IO2_OFF();
            macANT_IO3_OFF();
            macANT_IO4_OFF();
            break;
        case 2:
            macANT_IO1_ON();
            macANT_IO2_ON();
            macANT_IO3_OFF();
            macANT_IO4_OFF();
            break;
        case 3:
            macANT_IO1_ON();
            macANT_IO2_ON();
            macANT_IO3_ON();
            macANT_IO4_OFF();
            break;
        case 4:
            macANT_IO1_ON();
            macANT_IO2_ON();
            macANT_IO3_ON();
            macANT_IO4_ON();
            break;
        default:
            macANT_IO1_OFF();
            macANT_IO2_OFF();
            macANT_IO3_OFF();
            macANT_IO4_OFF();
            break;
        }
}

// ����Ҫ��ʾ�Ĳ˵����ݵ�������
void copyMenu (u8 num, u8 cmd, u8 values, u8 addr, u8 count)
{
    u8 *str_id = checkShowMsg(cmd);
    u8 i, n;
    n = check_menu (DLG_STATUS);
    for (i = addr; i < 16; i++)
    {
        g_dlg[n].MsgRow[num - 1][i] = ' ';
    }
    for (i = 0; i < count; i++)
    {
        g_dlg[n].MsgRow[num - 1][i + addr] = str_id[i];
    }
    g_ucIsUpdateMenu = 1;      // ���½���
}

// ����Ҫ��ʾ�Ĳ˵����ݵ�������
void copyStatusMsg (u8 num, u8 cmd, u8 values, u8 addr, u8 count)
{
    u8 *str_id = checkShowStatusMsg(cmd);
    u8 i, n;
    //strcpy() = CheckShowMsg(id);
    n = check_menu (DLG_STATUS);
    for (i = 0; i < count; i++)
    {
        g_dlg[n].MsgRow[num - 1][i + addr] = str_id[i];
    }
}

// ����Ҫ��ʾ�Ĳ˵����ݵ�������
void copyFaultMsg (u8 num, u8 cmd, u8 values, u8 addr, u8 count)
{
    u8 *str_id = checkShowStatusMsg(cmd);
    u8 i, n;
    //strcpy() = CheckShowMsg(id);
    n = check_menu (DLG_FAULT_CODE);
    for (i = 0; i < count; i++)
    {
        g_dlg[n].MsgRow[num - 1][i + addr] = str_id[i];
    }
}

// �ҵ���ӡ���ַ��������������׵�ַ
u8 * checkShowFaultCode (u8 ch)
{
    u8 i = 0;
    for (i = 0; i < (sizeof (g_taShowFaultCode_msg) / sizeof (g_taShowFaultCode_msg[0])); i++)
    {
        if(g_taShowFaultCode_msg[i].CTL == ch)
        {
            return (u8 *)g_taShowFaultCode_msg[i].Msg;
        }
    }
    return (u8 *)g_taShowFaultCode_msg[0].Msg;
}

// �ҵ���ӡ���ַ��������������׵�ַ
u8 * checkShowStatusMsg (u8 ch)
{
    u8 i = 0;
    for (i = 0; i < (sizeof (g_taShowStatus_msg) / sizeof (g_taShowStatus_msg[0])); i++)
    {
        if(g_taShowStatus_msg[i].CTL == ch)
        {
            return (u8 *)g_taShowStatus_msg[i].Msg;
        }
    }
    return (u8 *)g_taShowStatus_msg[0].Msg;
}

// �ҵ���ӡ���ַ��������������׵�ַ
u8 * checkShowMsg (u8 ch)
{
    u8 i = 0;
    for (i = 0; i < (sizeof (g_taShow_msg) / sizeof (g_taShow_msg[0])); i++)
    {
        if(g_taShow_msg[i].CTL == ch)
        {
            return (u8 *)g_taShow_msg[i].Msg;
        }
    }
    return (u8 *)g_taShow_msg[0].Msg;
}

// �ҵ���ӡ���ַ��������������׵�ַ
u8 * checkPriMsg (u8 ch)
{
    u8 i = 0;
    for (i = 0; i < (sizeof (g_taPri_msg) / sizeof (g_taPri_msg[0])); i++)
    {
        if(g_taPri_msg[i].CTL == ch)
        {
             return (u8 *)g_taPri_msg[i].Msg;
         }
     }
     return (u8 *)g_taPri_msg[0].Msg;
 }

// CAN�������ݴ���
u8 analyzeCANFrame ( CanRxMsg arg )
{
    CanRxMsg mtRxMessage = arg;                       // can���ݽ��ջ���
    u8 ID_temp = 0;
    static u8 count = 0;
    g_uiSerNum = mtRxMessage.Data[0];                               // ����֡��Ų���,�����ݻظ�

    switch(mtRxMessage.Data[3])
    {
        case KEY_PRESS:                                 // ˾���Ѱ���
            g_ucCurOutCardId = mtRxMessage.Data[1];         // ��ס��ʱ�����Ŀ�����

            if ( g_ucConnectMode == 1 )
            {

                if ( (g_ucaDeviceStatus[0] < 1) && (g_ucaDeviceStatus[1] < 1)
                    && (g_ucaDeviceStatus[2] < 1) && (g_ucaDeviceStatus[3] < 1))
                {
                    if ( mtRxMessage.Data[4] == HAS_CARD )
                    {
                        antSwitch(g_ucCurOutCardId); // �����ػ��������ݵ�ͬʱ,�л�����
                        delayMs (150);
                        g_tCardKeyPressFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                        g_tCardKeyPressFrame.MECHINE_ID = g_ucCurOutCardId + '0';        // ������ת��Ϊ�ַ�,Ȼ�����ݷ��ͳ�ȥ
                        g_tCardKeyPressFrame.CARD_MECHINE = g_ucCurOutCardId <= 2 ? '1' : '2';   //

                        uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardKeyPressFrame ); // �����Ǿ���,���Բ�����������

                        g_siStatusOverTimeL = 1000;         // һ��ʱ�������û�з������,���ϴ���

                        g_ucLockPressKey = 1;

                        g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 1; // �����������̿�ʼ֮���ٴΰ���������Ӧ

                        copyMenu ( g_ucCurOutCardId, KEY_PRESS, 0, 8, 4 );
                        DEBUG_printf ( "%s\n", ( char * ) checkPriMsg ( CARD_KEY_PRESS ) );
                    }
                    else if (mtRxMessage.Data[4] == HAS_NO_CARD)
                    {
                        myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, 0x10, 0, 0, NO_FAIL );
                        g_ucaDeviceStatus[g_ucCurOutCardId -1] = 0;
                    }
                }
            }
            else if (g_ucConnectMode == 2)
            {
                if ( (g_ucaDeviceStatus[0] < 1) && (g_ucaDeviceStatus[1] < 1)
                    && (g_ucaDeviceStatus[2] < 1) && (g_ucaDeviceStatus[3] < 1))
                {
                    if ( mtRxMessage.Data[4] == HAS_CARD )
                    {
                        antSwitch(g_ucCurOutCardId); // �����ػ��������ݵ�ͬʱ,�л�����
                        myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                        g_siStatusOverTimeL = 2000;

                        g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 3; // �����������̿�ʼ֮���ٴΰ���������Ӧ

                        copyMenu ( g_ucCurOutCardId, KEY_PRESS, 0, 8, 4 );
                        DEBUG_printf ( "%s\n", ( char * ) checkPriMsg ( CARD_KEY_PRESS ) );

                    }
                    else if (mtRxMessage.Data[4] == HAS_NO_CARD)
                    {
                        myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, 0x10, 0, 0, NO_FAIL );
                        g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 0;
                    }
                }
                else
                {
                    myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, WRITE_CARD_STATUS, 0x10, 0, 0, NO_FAIL );
                    g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 0;
                }
            }
            break;
        case CARD_SPIT_NOTICE:                          // ����֪ͨ
            g_siStatusOverTimeL = 1000;
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, CARD_SPIT_NOTICE_ACK, 0, 0, 0, NO_FAIL );
            copyMenu ( mtRxMessage.Data[1], CARD_SPIT_NOTICE, 0, 8, 4 );
            g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 0;      // ������ȥ֮��,����Ϊ��δ����
            if ( ( mtRxMessage.Data[4] == CARD_IS_OK ) && ( g_ucConnectMode == 1 ) )
            {
                //dacSet ( DATA_quka, SOUND_LENGTH_quka );
                antSwitch( 0 ); //�ͷ�����

                g_tCardSpitOutFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                g_tCardSpitOutFrame.CARD_MECHINE = mtRxMessage.Data[1] <= 2 ? '1' : '2';
                g_tCardSpitOutFrame.MECHINE_ID = mtRxMessage.Data[1] + '0';

                uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardSpitOutFrame ); // �����Ǿ���,���Բ�����������

                TIM_SetCounter(GENERAL_TIM2, 0);      // ��ʱ������,2s֮���ٴ��ϱ���Ϣ

                g_tCardMechineStatusFrame.CARD_MECHINE1.antHasCard = g_ucaCardIsReady[0] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE1.status = '0';

                g_tCardMechineStatusFrame.CARD_MECHINE2.antHasCard = g_ucaCardIsReady[1] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE2.status = '0';

                g_tCardMechineStatusFrame.CARD_MECHINE3.antHasCard = g_ucaCardIsReady[2] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE3.status = '0';

                g_tCardMechineStatusFrame.CARD_MECHINE4.antHasCard = g_ucaCardIsReady[3] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE4.status = '0';

                g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
                g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';

                uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardMechineStatusFrame ); // �����Ǿ���,���Բ�����������

                g_siStatusOverTimeL = 1000;
                g_siStatusOverTimeS = 0;
                g_siOutCardMsgTime = 0;     // �����ط�
                g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 4;

            }
            else if ( ( mtRxMessage.Data[4] == CARD_IS_BAD ) && ( g_ucConnectMode == 1 ) )
            {
                g_siStatusOverTimeS = 0;
                g_ucaDeviceStatus[0] = 0;
                g_ucaDeviceStatus[1] = 0;
                g_ucaDeviceStatus[2] = 0;
                g_ucaDeviceStatus[3] = 0;
            }
            break;
        case CARD_TAKE_AWAY_NOTICE:                     // ���ѱ�ȡ��֪ͨ
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, CARD_TAKE_AWAY_NOTICE_ACK, 0, 0, 0, NO_FAIL );
            g_siStatusOverTimeL = 1000;
            g_siStatusOverTimeS = 0;
            TIM_SetCounter(GENERAL_TIM2, 0);      // ��ʱ������,2s֮���ٴ��ϱ���Ϣ
            g_ucaHasBadCard[mtRxMessage.Data[1] - 1] = 0;   // �������״̬
            if (g_ucConnectMode == 1)
            {
                g_tCardTakeAwayFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                g_tCardTakeAwayFrame.MECHINE_ID = mtRxMessage.Data[1] + '0';
                g_tCardTakeAwayFrame.CARD_MECHINE = mtRxMessage.Data[1] < 3 ? '1' : '2';

                uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardTakeAwayFrame ); // �����Ǿ���,���Բ�����������

                g_ucaDeviceStatus[mtRxMessage.Data[1] - 1] = 0;  // �������Ѿ���ȡ��,��λ״̬

            }
            else
            {
                g_ucaDeviceStatus[mtRxMessage.Data[1] - 1] = 0;  // �������Ѿ���ȡ��,��λ״̬
            }

            copyMenu ( mtRxMessage.Data[1], CARD_TAKE_AWAY_NOTICE, 0, 8, 4 );
            g_ucIsUpdateMenu    = 1;                    // ���½���
            break;
        case CARD_IS_READY:                             // ������
            g_ucaHasBadCard[mtRxMessage.Data[1] - 1] = 0;       // ������,���������״̬
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, CARD_READY_ACK, 0, 0, 0, NO_FAIL );
            copyMenu ( mtRxMessage.Data[1], CARD_IS_READY, 0, 8, 6 );
            g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 1;      // ������
            break;
        case MECHINE_WARNING:                           // ����
            g_siStatusOverTimeL = 100;
            g_siStatusOverTimeS = 0;
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, FAULT_CODE_ACK, 0, 0, 0, NO_FAIL ); // �ظ�������
            if ( ( mtRxMessage.Data[2] != 0xff ) && ( mtRxMessage.Data[4] == 0x21 ) && ( mtRxMessage.Data[7] <= FAULT_CODE11 ) )
            {

                if ( ( FAULT_CODE05 == mtRxMessage.Data[7] ) \
                  || ( FAULT_CODE06 == mtRxMessage.Data[7] ) \
                  || ( FAULT_CODE07 == mtRxMessage.Data[7] ) \
                  || ( FAULT_CODE08 == mtRxMessage.Data[7] ) \
                  && ( 0 != g_ucaFaultCode[mtRxMessage.Data[1] - 1] ) )    // ���������ת���߷�תʧ�ܵ��µķ���ʧ��
                {
                    g_tCardSpitOutFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                    g_tCardSpitOutFrame.CARD_MECHINE = '3';
                    g_tCardSpitOutFrame.MECHINE_ID = mtRxMessage.Data[1] + '0';
                    uartInQueue( &g_tUARTTxQueue, (char *)&g_tCardSpitOutFrame ); // �����Ǿ���,���Բ�����������
                }

                TIM_SetCounter(GENERAL_TIM2, 0);      // ��ʱ������,2s֮���ٴ��ϱ���Ϣ
                g_ucIsUpdateMsgFlag = 1;
                g_ucaDeviceStatus[ mtRxMessage.Data[1] - 1 ] = 0;

                if(mtRxMessage.Data[7] == FAULT_CODE11)
                {
                    myCANTransmit(gt_TxMessage, mtRxMessage.Data[1], NO_FAIL, CLEAR_FAULT_CODE, CLEAR_FAULT, NO_FAIL, NO_FAIL, FAULT_CODE11);   // ����������߹�����,����ֱ�Ӵ���,2018��4��21��23:52:16
                    return;
                }
                else
                {
                    g_ucaFaultCode[mtRxMessage.Data[1] - 1] = mtRxMessage.Data[7]; // ���������
                }
            }
            else if ( mtRxMessage.Data[7] > FAULT_CODE11 )
            {
                g_ucaFaultCode[mtRxMessage.Data[1] - 1] = FAULT_CODE11 + 1; // ���������
            }

            g_ucIsNewWarningCode = 1;                       // ���µı���,�ٴθ��½���
            g_ucIsUpdateMenu    = 1;                        // ���½���
            break;
        case CARD_MACHINE_INIT_ACK:
            g_ucaMechineExist[mtRxMessage.Data[1] - 1] = 1;    // �����������,���豸��������,�����лظ�,�����������������,��ͨ������

            break;
        case CYCLE_ACK:                 // ��ʱ��ѯ�ظ�
            g_ucaMechineExist[mtRxMessage.Data[1] - 1] = 1;
            if ( mtRxMessage.Data[4] == HAS_CARD )
            {
                g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 1;
            }
            else
            {
                g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 0;
            }
            break;
        case SET_MECHINE_STATUS_ACK:    // ���ÿ�����ÿ�����������״̬�ɹ�,�������ظ���״̬,����״̬λΪ1
            break;
        default:
            break;
    }
    return 0;
}

u8  analyzeUartFrame ( const u8 argv[] , u32 size)
{

    u8 ucaFrame[50] = {0};
    u8 ucSerNum = 0;
    //u8 ucNum = argv[1];
    u8 type_frame = argv[2];

    if (PC_INIT_MECHINE <= type_frame <= PC_SET_CARD_NUM)  // ������ݺϷ���
    {
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* ��ʼ��������Ϣ(61H)֡ */
                break;
            case PC_SPIT_OUT_CARD:              /* ������Ϣ(62H)֡ */
                antSwitch( 0 );
                g_ucBadCardCount = 0;

                myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );

                g_siStatusOverTimeL = 2500;      // ���25s��û����ɷ���������Ϊ�����й�����,�ҿ���20s֮��ŷ��͹��ϱ���
                g_siStatusOverTimeS = 800;
                g_siOutCardMsgTime = 3;              // �ط�3��

                g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 3; // ������������״̬
                g_ucaHasBadCard[g_ucCurOutCardId - 1] = 0;   // �������״̬

                copyMenu ( g_ucCurOutCardId, CARD_SPIT_NOTICE, 0, 8, 4 );
                copyStatusMsg ( g_ucCurOutCardId, 0xfe, 0, 12, 4 ); //
                g_ucaCardIsReady[ g_ucCurOutCardId - 1] = 0;

                break;
            case PC_BAD_CARD:                   /* ������Ϣ(63H)֡ */
                g_ucIsUpdateMsgFlag = 1;

                TIM_SetCounter(GENERAL_TIM2, 0);      // ��ʱ������,2s֮���ٴ��ϱ���Ϣ

                g_ucaHasBadCard[g_ucCurOutCardId - 1] = 1;

                myCANTransmit ( gt_TxMessage, g_ucCurOutCardId, 0, WRITE_CARD_STATUS, CARD_IS_BAD, 0, 0, NO_FAIL );
                g_ucaDeviceStatus[g_ucCurOutCardId - 1] = 0;
                g_ucaCardIsReady[ g_ucCurOutCardId - 1] = 0;

                break;
            case PC_QUERY_CARD_MECHINE:       /* ��ѯ����״̬(65H)֡ */
                break;
            case PC_QUERY_CARD_CLIP:          /* ��ѯ����(66H)֡ */
                break;
            case PC_SET_CARD_NUM:             /* ���ÿ��п���(67H)֡ */
                break;
            case PC_GET_DIST:                 /* ���֡ */
                break;
            case PC_CAR_HAS_COME:             /* ������ */
                myCANTransmit ( gt_TxMessage, 3, 0, CAR_HAS_COME, 0, 0, 0, NO_FAIL );
                break;
            case PC_CAR_HAS_GONE:             /* ������ */
                myCANTransmit ( gt_TxMessage, 3, 0, CAR_HAS_GONE, 0, 0, 0, NO_FAIL );
                break;
            case MECHINE_CODE_VERSION:
                printf ("the code version %s,%s\n", __DATE__,__TIME__);
                break;
            default:
                displayGB2312String (0, 0, argv, 1);   /* ��Ч��Ϣ */
                displayGB2312String (0, 2, "��Ч��Ϣ", 0);
                displayGB2312String (0, 4, "                ", 0);
                displayGB2312String (0, 6, "                ", 0);
                break;
        }
        DEBUG_printf ("%s\n",(char *)checkPriMsg(type_frame));
    }
    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}
