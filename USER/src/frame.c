#include <includes.h>
#include <frame.h>
#include <WAV_C_xiexie.h>
#include <WAV_C_quka.h>

u32 g_uiaInitCardCount[5]    = {1000, 999, 999, 999, 999};    // 卡初始设置值,[0]为总卡数量,发1张卡,减1,[1~4]为每个卡机初始卡数量,发1张卡,减1.
u32 g_uiaSpitCardCount[5]    = {0, 0, 0, 0, 0};    // 出卡数量,[0]为出卡总数量,发1张卡,加1,[1~4]为每个卡机发卡数量,发1张卡,加1.


u8 g_ucSerNum = '0';  // 帧序号   全局

RSCTL_FREME g_tP_RsctlFrame = {'<','0','0','>'};        // 正应答帧
RSCTL_FREME g_tN_RsctlFrame = {'<','1','0','>'};        // 负应答帧

/* 卡机上电信息(41H)帧          4字节 */
CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame = {'<', '0', CARD_MACHINE_POWER_ON, '>'};

/* 状态信息(42H)帧             30字节 */
CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame =    {'<', '0', 'B', '1','3',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '0', '0', '9', '9', '9', '1',
                                                                '>'};

/* 已出卡信息(43H)帧            6字节 */
CARD_MECHINE_TO_PC_FRAME        g_tCardSpitOutFrame = {'<', '0', CARD_SPIT_OUT, '1', '1', '>'};

/* 按钮取卡信息(44H)帧          6字节 */
CARD_MECHINE_TO_PC_FRAME        g_tCardKeyPressFrame = {'<', '0', CARD_KEY_PRESS, '1', '1' ,'>'};

/* 卡被取走信息(45H)帧          6字节 */
CARD_MECHINE_TO_PC_FRAME        g_tCardTakeAwayFrame = {'<', '0', CARD_TAKE_AWAY, '1', '1', '>'};

/* 上报卡夹号编号信息(46H)帧   36字节 */
CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame = {'<', '0', CARD_REPORT_SPIT_STATUES, '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0',
                                                                                                     '0', '0', '0', '0', '0', '0', '0', '0', '>'};



/* 初始化卡机信息(61H)帧        20字节 */
PC_TO_CARD_INIT_FREME           g_tPcToCardInitFrame = {'<', '0', PC_INIT_MECHINE, '9', '9', '9', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0', '0','>'};

/* 出卡信息(62H)帧              5字节 */
PC_TO_CARD_MECHINE_FRAME        g_tPcSpitOutCardFrame = {'<', '0', PC_SPIT_OUT_CARD, '1', '>'};

/* 坏卡信息(63H)帧              5字节 */
PC_TO_CARD_MECHINE_FRAME        g_tPcBadCardFrame = {'<', '0', PC_BAD_CARD, '1', '>'};

/* 查询卡机状态(65H)帧          5字节 */
PC_TO_CARD_MECHINE_FRAME        g_tPcQuetyCardMechineFrame = {'<', '0', PC_QUERY_CARD_MECHINE, '1', '>'};

 /* 查询卡夹(66H)帧              5字节 */
PC_TO_CARD_MECHINE_FRAME        g_tPcQuetyCardCpipFrame = {'<', '0', PC_QUERY_CARD_CLIP, '1', '>'};

/* 设置卡夹卡数(67H)帧          8字节*/
PC_SET_CARD_NUM_FRAME           g_tPcSetCardNumFrame = {'<', '0', PC_SET_CARD_NUM, '1', '9', '9' , '9', '>'};





//#pragma  diag_suppress 870          // 不显示警告

const Print_msg g_taPri_msg[] = {
                            {'$',                           "/* 无效信息 */"},
                            {CARD_MACHINE_POWER_ON,         "/* 卡机上电信息(41H)帧          4字节 */"},
                            {CARD_MACHINE_STATUES,          "/* 状态信息(42H)帧             30字节 */"},
                            {CARD_SPIT_OUT,                 "/* 已出卡信息(43H)帧            6字节 */"},
                            {CARD_KEY_PRESS,                "/* 按钮取卡信息(44H)帧          6字节 */"},
                            {CARD_TAKE_AWAY,                "/* 卡被取走信息(45H)帧          6字节 */"},
                            {CARD_REPORT_SPIT_STATUES,      "/* 上报卡夹号编号信息(46H)帧   36字节 */"},

                            {PC_INIT_MECHINE,               "/* 初始化卡机信息(61H)帧       20字节 */"},
                            {PC_SPIT_OUT_CARD,              "/* 出卡信息(62H)帧              5字节 */"},
                            {PC_BAD_CARD,                   "/* 坏卡信息(63H)帧              5字节 */"},
                            {PC_QUERY_CARD_MECHINE,         "/* 查询卡机状态(65H)帧          5字节 */"},
                            {PC_QUERY_CARD_CLIP,            "/* 查询卡夹(66H)帧              5字节 */"},
                            {PC_SET_CARD_NUM,               "/* 设置卡夹卡数(67H)帧          8字节 */"},
                            {PC_GET_DIST,                   "/* 测距(74H)帧                  5字节 */"},
                            {'0',NULL}
                        };

const Print_msg g_taShow_msg[] = {
                            {0,                             "NULL"},
                            {MACHINE_CHECK_CARD,            "验卡"},
                            {KEY_PRESS,                     "按键"},
                            {CARD_SPIT_NOTICE,              "出卡"},
                            {CARD_TAKE_AWAY_NOTICE,         "取卡"},
                            {CARD_IS_READY,                 "卡就绪"},
                            {0xfe,                          "      "},
                            {0xff,NULL}
                        };

const Print_msg g_taShowStatus_msg[] = {
                            {0,                             "NULL"},
                            {CARD_IS_OK,                    "好卡"},
                            {CARD_IS_BAD,                   "坏卡"},
                            {IS_WORKING,                    "工作"},
                            {IS_BACKING,                    "备用"},
                            {0xfe,                          "    "},
                            {0xff,NULL}
                        };
const Print_msg g_taShowFaultCode_msg[] = {
                            {0,                             "NULL"},
                            {CARD_IS_OK,                    "好卡"},
                            {CARD_IS_BAD,                   "坏卡"},
                            {0xfe,                          "    "},
                            {0xff,NULL}
                        };
// 天线的切换,4个天线,对应着不同的逻辑,需要同时使能不同的继电器
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

// 复制要显示的菜单数据到数组中
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
    g_ucIsUpdateMenu = 1;      // 更新界面
}

// 复制要显示的菜单数据到数组中
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

// 复制要显示的菜单数据到数组中
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

// 找到打印的字符串，并返回其首地址
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

// 找到打印的字符串，并返回其首地址
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

// 找到打印的字符串，并返回其首地址
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

// 找到打印的字符串，并返回其首地址
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

// CAN总线数据处理
 u8 analyzeCANFrame ( CanRxMsg arg )
 {
    CanRxMsg mtRxMessage = arg;                       // can数据接收缓存
    u8 ID_temp = 0;
    static u8 count = 0;
    g_uiSerNum = mtRxMessage.Data[0];                               // 保持帧序号不变,将数据回复

    switch(mtRxMessage.Data[3])
    {
        case KEY_PRESS:                                 // 司机已按键
            if ( (g_ucaDeviceIsSTBY[0] == 1) && (g_ucaDeviceIsSTBY[1] == 1)
              && (g_ucaDeviceIsSTBY[2] == 1) && (g_ucaDeviceIsSTBY[3] == 1) || (mtRxMessage.Data[2] == 0xff ))
            {
                if ( mtRxMessage.Data[4] == HAS_CARD ) // 未进入发卡流程,且有卡
                {
                    // g_ucBadCardCount = 0;
                    if ( g_ucConnectMode == 1 )
                    {
                        antSwitch(mtRxMessage.Data[1]); // 往工控机发送数据的同时,切换天线
                        delayMs (150);
                        g_tCardKeyPressFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                        g_tCardKeyPressFrame.MECHINE_ID = mtRxMessage.Data[1] + '0';		// 将数据转换为字符,然后将数据发送出去
                        g_tCardKeyPressFrame.CARD_MECHINE = mtRxMessage.Data[1] <= 2 ? '1' : '2';   //
                        printf ( "%s", ( char * ) &g_tCardKeyPressFrame );
                    }
                    else
                    {
                        antSwitch(mtRxMessage.Data[1]); // 往工控机发送数据的同时,切换天线
                        myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                    }
                    g_ucaDeviceIsSTBY[mtRxMessage.Data[1] -1] = 0; // 按键发卡流程开始之后，再次按键不再响应
                    copyMenu ( mtRxMessage.Data[1], KEY_PRESS, 0, 8, 4 );
                    DEBUG_printf ( "%s\n", ( char * ) checkPriMsg ( CARD_KEY_PRESS ) );
                }
                else
                {
                    myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, WRITE_CARD_STATUS, 0x10, 0, 0, NO_FAIL );
                    g_ucaDeviceIsSTBY[mtRxMessage.Data[1] -1] = 1;
                }
            }
            else
            {
                myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, WRITE_CARD_STATUS, 0x10, 0, 0, NO_FAIL );
            }

            break;
        case CARD_SPIT_NOTICE:                          // 出卡通知
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, CARD_SPIT_NOTICE_ACK, 0, 0, 0, NO_FAIL );
            copyMenu ( mtRxMessage.Data[1], CARD_SPIT_NOTICE, 0, 8, 4 );
            if ( (mtRxMessage.Data[4] == CARD_IS_OK) && (g_ucConnectMode == 1))
            {
                antSwitch( 0 ); //释放天线
                g_tCardSpitOutFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                g_tCardSpitOutFrame.CARD_MECHINE = mtRxMessage.Data[1] <= 2 ? '1' : '2';
                g_tCardSpitOutFrame.MECHINE_ID = mtRxMessage.Data[1] + '0';
                //dacSet ( DATA_quka, SOUND_LENGTH_quka );
                printf ("%s", (char *)&g_tCardSpitOutFrame);

                g_tCardMechineStatusFrame.CARD_MECHINE1.antHasCard = g_ucaCardIsReady[0] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE1.status = '0';

                g_tCardMechineStatusFrame.CARD_MECHINE2.antHasCard = g_ucaCardIsReady[1] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE2.status = '0';

                g_tCardMechineStatusFrame.CARD_MECHINE3.antHasCard = g_ucaCardIsReady[2] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE3.status = '0';

                g_tCardMechineStatusFrame.CARD_MECHINE4.antHasCard = g_ucaCardIsReady[3] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE4.status = '0';

                TIM_SetCounter(GENERAL_TIM2, 0);      // 定时器清零,2s之后再次上报消息
                g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
                g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';
                printf ( "%s", ( char * ) &g_tCardMechineStatusFrame );
            }
            g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 0;      // 卡翻出去之后,就认为卡未就绪
            break;
        case CARD_TAKE_AWAY_NOTICE:                     // 卡已被取走通知
            //antSwitch( 0 ); // 释放天线
            g_ucaHasBadCard[mtRxMessage.Data[1] - 1] = 0;   // 清除坏卡状态
            g_ucaDeviceIsSTBY[mtRxMessage.Data[1] -1] = 1;  // 表明卡已经被取走,置位状态
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, CARD_TAKE_AWAY_NOTICE_ACK, 0, 0, 0, NO_FAIL );

            switch ( mtRxMessage.Data[1] )
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
            //dacSet ( DATA_xiexie, SOUND_LENGTH_xiexie );
            if (g_ucConnectMode == 1)
            {
                g_tCardTakeAwayFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                g_tCardTakeAwayFrame.MECHINE_ID = mtRxMessage.Data[1] + '0';
                g_tCardTakeAwayFrame.CARD_MECHINE = mtRxMessage.Data[1] < 3 ? '1' : '2';
                printf ( "%s", ( char * ) &g_tCardTakeAwayFrame );
            }
            copyMenu ( mtRxMessage.Data[1], CARD_TAKE_AWAY_NOTICE, 0, 8, 4 );
            g_ucIsUpdateMenu    = 1;                    // 更新界面
            break;
        case CARD_IS_READY:                             // 卡就绪
            //antSwitch( 0 ); //释放天线
            g_ucaHasBadCard[mtRxMessage.Data[1] - 1] = 0;       // 卡就绪,就清除坏卡状态
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, CARD_READY_ACK, 0, 0, 0, NO_FAIL );
            copyMenu ( mtRxMessage.Data[1], CARD_IS_READY, 0, 8, 6 );
            g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 1;      // 卡就绪
            break;
        case MECHINE_WARNING:                           // 报警
            //antSwitch( 0 ); //释放天线
            myCANTransmit ( gt_TxMessage, mtRxMessage.Data[1], 0, FAULT_CODE_ACK, 0, 0, 0, NO_FAIL ); // 回复故障码
            g_ucaDeviceIsSTBY[0] = 1;
            g_ucaDeviceIsSTBY[1] = 1;
            g_ucaDeviceIsSTBY[2] = 1;
            g_ucaDeviceIsSTBY[3] = 1;
            g_ucRepeatKeyMechine = 0;
            if ( ( mtRxMessage.Data[2] != 0xff ) && ( mtRxMessage.Data[4] == 0x21 ) && ( mtRxMessage.Data[7] <= FAULT_CODE11 ) )
            {
                switch ( mtRxMessage.Data[1] )
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
                if(mtRxMessage.Data[7] == FAULT_CODE11)
                {
                    myCANTransmit(gt_TxMessage, mtRxMessage.Data[1], NO_FAIL, CLEAR_FAULT_CODE, CLEAR_FAULT, NO_FAIL, NO_FAIL, FAULT_CODE11);   // 如果报告总线故障码,本机直接处理,2018年4月21日23:52:16
                    return;
                }
                else
                {
                    g_ucaFaultCode[mtRxMessage.Data[1] - 1] = mtRxMessage.Data[7]; // 报告故障码
                }
            }
            else if ( mtRxMessage.Data[7] > FAULT_CODE11 )
            {
                g_ucaFaultCode[mtRxMessage.Data[1] - 1] = FAULT_CODE11 + 1; // 报告故障码
            }
            g_ucIsNewWarningCode = 1;                       // 有新的报警,再次更新界面
            g_ucIsUpdateMenu    = 1;                        // 更新界面
            break;
        case CARD_MACHINE_INIT_ACK:
            g_ucaMechineExist[mtRxMessage.Data[1] - 1] = 1;    // 如果主机开机,对设备进行设置,卡机有回复,则表明几个卡机存在,并通信正常
            break;
        case CYCLE_ACK:                 // 定时轮询回复
            if (mtRxMessage.Data[4] == HAS_CARD)
            {
                g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 1 ;
            }
            else
            {
                g_ucaCardIsReady[mtRxMessage.Data[1] - 1] = 0 ;
            }
            break;
        case SET_MECHINE_STATUS_ACK:    // 如果每次设置卡机的主备机状态成功,卡机则会回复此状态,并置状态位为1
            break;
        default:
            break;
    }
    return 0;
}

u8  analyzeUartFrame ( u8 argv[] , u32 size)
{

    u8 ucaFrame[50] = {0};
    u8 ucSerNum = 0;
    //u8 ucNum = argv[1];
    u8 type_frame = argv[2];

    if (PC_INIT_MECHINE <= type_frame <= PC_SET_CARD_NUM)  // 检测数据合法性
    {
        //g_tP_RsctlFrame.RSCTL = ucNum;
        //printf("%s",(char *)&g_tP_RsctlFrame);   //发送正应答帧
        switch(type_frame)
        {
            case PC_INIT_MECHINE:               /* 初始化卡机信息(61H)帧 */
                break;
            case PC_SPIT_OUT_CARD:              /* 出卡信息(62H)帧 */
                antSwitch( 0 );
                switch (argv[3])
                {
                    case '5':
                        myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                        g_ucaHasBadCard[g_ucUpWorkingID - 1] = 0;   // 清除坏卡状态
                        g_uiaInitCardCount[g_ucUpWorkingID]--;
                        copyMenu ( g_ucUpWorkingID, CARD_SPIT_NOTICE, 0, 8, 4 );
                        copyStatusMsg ( g_ucUpWorkingID, 0xfe, 0, 12, 4 ); //
                        g_ucaCardIsReady[ g_ucUpWorkingID - 1] = 0;
                        break;
                    case '6':
                        myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, WRITE_CARD_STATUS, CARD_IS_OK, 0, 0, NO_FAIL );
                        g_ucaHasBadCard[g_ucDownWorkingID - 1] = 0;   // 清除坏卡状态
                        g_uiaInitCardCount[g_ucDownWorkingID]--;
                        copyMenu ( g_ucDownWorkingID, CARD_SPIT_NOTICE, 0, 8, 4 );
                        copyStatusMsg ( g_ucDownWorkingID, 0xfe, 0, 12, 4 ); //
                        g_ucaCardIsReady[ g_ucDownWorkingID - 1] = 0;
                        break;
                    default:
                        break;
                }
                break;
            case PC_BAD_CARD:                   /* 坏卡信息(63H)帧 */
                antSwitch( 0 );
                g_tCardMechineStatusFrame.CARD_MECHINE1.antHasCard = g_ucaCardIsReady[0] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE1.status = g_ucaHasBadCard[0] ? '2' : '0';

                g_tCardMechineStatusFrame.CARD_MECHINE2.antHasCard = g_ucaCardIsReady[1] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE2.status = g_ucaHasBadCard[1] ? '2' : '0';

                g_tCardMechineStatusFrame.CARD_MECHINE3.antHasCard = g_ucaCardIsReady[2] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE3.status = g_ucaHasBadCard[2] ? '2' : '0';

                g_tCardMechineStatusFrame.CARD_MECHINE4.antHasCard = g_ucaCardIsReady[3] + '0';
                g_tCardMechineStatusFrame.CARD_MECHINE4.status = g_ucaHasBadCard[3] ? '2' : '0';

                switch (argv[3])
                {
                    case '5':
                        myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, WRITE_CARD_STATUS, CARD_IS_BAD, 0, 0, NO_FAIL );
                        g_uiaInitCardCount[g_ucUpWorkingID]--;
                        g_ucaDeviceIsSTBY[g_ucUpWorkingID - 1] = 1;
                        g_ucaCardIsReady[ g_ucUpWorkingID - 1] = 0;

                        if ( g_ucUpWorkingID == 1)
                        {
                            g_ucaHasBadCard[0] = 1;
                            if ( (g_ucaFaultCode[1] == 0) && (g_ucaMechineExist[1] == 1) )   // 无故障,且通信正常
                            {
                                g_ucaMechineExist[0] = 0;
                                g_ucaMechineExist[1] = 0;
                                g_ucUpWorkingID     = 2;
                                g_ucUpBackingID     = 1;
                                myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                myCANTransmit ( gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                                if( g_ucaCardIsReady[1] == 1 )  // 2号卡机已经就绪
                                {
                                    antSwitch( 2 );                          //  切换天线
                                    g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
                                    g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';
                                    TIM_SetCounter(GENERAL_TIM2, 0);      // 定时器清零,2s之后再次上报消息

                                    printf ( "%s", ( char * ) &g_tCardMechineStatusFrame );

                                    delayMs (150);

                                    g_tCardKeyPressFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardKeyPressFrame.CARD_MECHINE = '1';
                                    g_tCardKeyPressFrame.MECHINE_ID = g_ucUpWorkingID + '0';
                                    printf ( "%s", ( char * ) &g_tCardKeyPressFrame );
                                }
                            }
                        }
                        else
                        {
                            g_ucaHasBadCard[1] = 1;
                            if ( (g_ucaFaultCode[0] == 0) && (g_ucaMechineExist[0] == 1) )   // 无故障,且通信正常
                            {
                                g_ucaMechineExist[0] = 0;
                                g_ucaMechineExist[1] = 0;
                                g_ucUpWorkingID     = 1;
                                g_ucUpBackingID     = 2;
                                myCANTransmit ( gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                myCANTransmit ( gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                                if(g_ucaCardIsReady[0] == 1)  // 卡已经就绪
                                {
                                    antSwitch( 1 );                          //  切换天线
                                    g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
                                    g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';
                                    TIM_SetCounter(GENERAL_TIM2, 0);      // 定时器清零,2s之后再次上报消息

                                    printf ( "%s", ( char * ) &g_tCardMechineStatusFrame );

                                    delayMs (150);

                                    g_tCardKeyPressFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardKeyPressFrame.CARD_MECHINE = '1';
                                    g_tCardKeyPressFrame.MECHINE_ID = g_ucUpWorkingID + '0';
                                    printf ( "%s", ( char * ) &g_tCardKeyPressFrame );
                                }
                            }
                        }
                        break;
                    case '6':
                        myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, WRITE_CARD_STATUS, CARD_IS_BAD, 0, 0, NO_FAIL );
                        g_uiaInitCardCount[g_ucDownWorkingID]--;
                        g_ucaDeviceIsSTBY[g_ucDownWorkingID - 1] = 1;
                        g_ucaCardIsReady[ g_ucDownWorkingID - 1] = 0;
                        if ( g_ucDownWorkingID == 3)
                        {
                            g_ucaHasBadCard[2] = 1;
                            if ( (g_ucaFaultCode[3] == 0) && (g_ucaMechineExist[3] == 1) )   // 无故障,且通信正常
                            {
                                g_ucaMechineExist[2] = 0;
                                g_ucaMechineExist[3] = 0;
                                g_ucDownWorkingID   = 4;
                                g_ucDownBackingID   = 3;
                                myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                myCANTransmit ( gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                                if( g_ucaCardIsReady[3] == 1 )
                                {
                                    antSwitch( 4 );                          //  切换天线
                                    g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
                                    g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';
                                    TIM_SetCounter(GENERAL_TIM2, 0);      // 定时器清零,2s之后再次上报消息

                                    printf ( "%s", ( char * ) &g_tCardMechineStatusFrame );

                                    delayMs (150);

                                    g_tCardKeyPressFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardKeyPressFrame.CARD_MECHINE = '2';
                                    g_tCardKeyPressFrame.MECHINE_ID = g_ucDownWorkingID + '0';
                                    printf ( "%s", ( char * ) &g_tCardKeyPressFrame );
                                }
                            }
                        }
                        else
                        {
                            g_ucaHasBadCard[3] = 1;
                            if ( (g_ucaFaultCode[2] == 0) && (g_ucaMechineExist[2] == 1) )   // 无故障,且通信正常
                            {
                                g_ucaMechineExist[2] = 0;
                                g_ucaMechineExist[3] = 0;
                                g_ucDownWorkingID   = 3;
                                g_ucDownBackingID   = 4;
                                myCANTransmit ( gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL );
                                myCANTransmit ( gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL ); // 设置工作态
                                if( g_ucaCardIsReady[2] == 1 )
                                {
                                    antSwitch( 3 );                          //  切换天线
                                    g_tCardMechineStatusFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardMechineStatusFrame.UP_SPIT_IS_OK = g_ucUpWorkingID + '0';
                                    g_tCardMechineStatusFrame.DOWN_SPIT_IS_OK = g_ucDownWorkingID + '0';
                                    TIM_SetCounter(GENERAL_TIM2, 0);      // 定时器清零,2s之后再次上报消息
                                    printf ( "%s", ( char * ) &g_tCardMechineStatusFrame );

                                    delayMs (150);

                                    g_tCardKeyPressFrame.RSCTL = (g_uiSerNumPC++ % 10) + '0';
                                    g_tCardKeyPressFrame.CARD_MECHINE = '2';
                                    g_tCardKeyPressFrame.MECHINE_ID = g_ucDownWorkingID + '0';
                                    printf ( "%s", ( char * ) &g_tCardKeyPressFrame );
                                }
                            }
                        }
                        break;
                    default:
                        g_ucaHasBadCard[0] = 1;
                        g_ucaHasBadCard[1] = 1;
                        g_ucaHasBadCard[2] = 1;
                        g_ucaHasBadCard[3] = 1;
                        g_ucaDeviceIsSTBY[0] = 1;
                        g_ucaDeviceIsSTBY[1] = 1;
                        g_ucaDeviceIsSTBY[2] = 1;
                        g_ucaDeviceIsSTBY[3] = 1;
                        break;
                }
                break;
            case PC_QUERY_CARD_MECHINE:       /* 查询卡机状态(65H)帧 */
                break;
            case PC_QUERY_CARD_CLIP:          /* 查询卡夹(66H)帧 */
                break;
            case PC_SET_CARD_NUM:             /* 设置卡夹卡数(67H)帧 */
                break;
            case PC_GET_DIST:                 /* 测距帧 */
                break;
            case PC_CAR_HAS_COMING:           /* 车已来 */
                break;
            case PC_CAR_HAS_GONE:             /* 车已走 */
                break;
           case MECHINE_CODE_VERSION:
                printf ("the code version %s,%s", __DATE__,__TIME__);
                break;
            default:
                displayGB2312String (0, 0, argv, 1);   /* 无效信息 */
                displayGB2312String (0, 2, "无效信息", 0);
                displayGB2312String (0, 4, "                ", 0);
                displayGB2312String (0, 6, "                ", 0);
                break;
        }
        DEBUG_printf ("%s\n",(char *)checkPriMsg(type_frame));
    }
    ucSerNum = (g_ucSerNum++) % 10 + '0';
    return 0;
}
