#include "includes.h"
#include "menu.h"


Dlg g_dlg[] =           {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                ", 5},

                        {DLG_LOGO,           "  ������ϵͳ    ", "    ��ʼ��...   ", "      �Ժ�      ", "   �汾: V1.0   ", 5},
                        {DLG_STATUS,         "1.�Ż�:         ", "2.�Ż�:         ", "3.�Ż�:         ", "4.�Ż�:         ", 5},
                        {DLG_MAIN,           "1.�ϡ��°����  ", "2.��������      ", "3.��������      ", "                ", 0},
                        {DLG_CARD_COUNT_SET, "1 �Ż�����      ", "2 �Ż�����      ", "3 �Ż�����      ", "4 �Ż�����      ", 5},
                        {DLG_CONNETCT_SET,   "  ����ģʽ����  ", "1.���߷���      ", "2.���߷���      ", "                ", 1},
                        {DLG_CARD_ID,        "   ���ÿ���ID   ", "��������        ", "������:         ", "ͨ��ID��:       ", 5},
                        {DLG_WORKING_SET,    "  ������������  ", "1:����    2:����", "3:����    4:����", "                ", 1},
                        {DLG_EMPLOYEE_MENU,  "  �ϡ��°����  ", "�°� (��������) ", "�ϰ� (��������) ", "                ", 1},

                        {DLG_CARD_MAIN,      "����״̬:1�ſ���", "         2�ſ���", "         3�ſ���", "         4�ſ���", 0},
                        {DLG_STATUS_ONE,     " 1�ų���        ", " 2�ų���        ", " 3�ų���        ", " 4�ų���        ", 5},
                        {DLG_STATUS_TWO,     " 1�ų�����      ", " 2�ų�����      ", " 3�ų�����      ", " 4�ų�����      ", 5},

                        {DLG_DEBUG_MAIN,     "    �ſ�������  ", "1:�������      ", "2:�Ϳ����      ", "3:�����      ", 0},
                        {DLG_DEBUG_ONE,      "��: ǰ����ֹͣ   ", "��: ���ˡ�ֹͣ   ", "��: �������λ   ", "��: ��һ�ſ�     ", 5},
                        {DLG_DEBUG_TWO,      "��: ǰ����ֹͣ   ", "��: ���ˡ�ֹͣ   ", "                ", "                ", 5},
                        {DLG_DEBUG_THREE,    "��: ǰ����ֹͣ   ", "��: ���ˡ�ֹͣ   ", "��: �������λ   ", "                ", 5},

                        {DLG_FAULT_CODE,     "    �ſ�������  ", "������:         ", "˵��:           ", "                ", 5},

                     };
Dlg g_dlg_fault_code[] = {
                        {DLG_CLEAR_LCD,      "                ", "                ", "                ", "                ", 5},

                        {FAULT_CODE01,       "    �ſ�������  ", "������:(01H)    ", "˵��: �������޿�", "��װ��IC��      ", 5},
                        {FAULT_CODE02,       "    �ſ�������  ", "������:(02H)    ", "˵��: �������޿�", "��װ��IC��      ", 5},
                        {FAULT_CODE03,       "    �ſ�������  ", "������:(03H)    ", "˵��: �������޿�", "��װ��IC��      ", 5},
                        {FAULT_CODE04,       "    �ſ�������  ", "������:(04H)    ", "˵��: �������޿�", "��װ��IC��      ", 5},
                        {FAULT_CODE05,       "    �ſ�������  ", "������:(05H)    ", "˵��: ����ʱ����", "�����ת        ", 5},
                        {FAULT_CODE06,       "    �ſ�������  ", "������:(06H)    ", "˵��: ����ʱ����", "�����ת        ", 5},
                        {FAULT_CODE07,       "    �ſ�������  ", "������:(07H)    ", "˵��: ����ʱ����", "�����ת        ", 5},
                        {FAULT_CODE08,       "    �ſ�������  ", "������:(08H)    ", "˵��: ����ʱ����", "�����ת        ", 5},
                        {FAULT_CODE09,       "    �ſ�������  ", "������:(09H)    ", "                ", "                ", 5},
                        {FAULT_CODE0A,       "    �ſ�������  ", "������:(0AH)    ", "                ", "                ", 5},
                        {FAULT_CODE0B,       "    �ſ�������  ", "������:(0BH)    ", "                ", "                ", 5},
                        {FAULT_CODE0C,       "    �ſ�������  ", "������:(0CH)    ", "˵��: ͨѶ����  ", "                ", 5},
                        {FAULT_CODE0D,       "    �ſ�������  ", "������:(δ֪)   ", "                ", "                ", 5},
                        {FAULT_CODE0E,       "    �ſ�������  ", "������:(δ֪)   ", "                ", "                ", 5},
                        {FAULT_CODE0F,       "    �ſ�������  ", "������:(δ֪)   ", "                ", "                ", 5},
                        {FAULT_CODE10,       "    �ſ�������  ", "˵��: �������޿�", "��װ��IC��      ", "                ", 5},
                        {FAULT_CODE11,       "    �ſ�������  ", "������:(11H)    ", "˵��:CAN���߹���", "                ", 5},

                        {FAULT_CODE11+1,     "    �ſ�������  ", "������:(δ֪)   ", "                ", "                ", 5},
                     };

// �ҵ���ʾ�Ĳ˵�ID,���������������е�����
u8 check_menu(u8 ch)
{
    u8 i = 0;
    for (i = 0; i < (sizeof (g_dlg) / sizeof (g_dlg[0])); i++)
    {
        if(g_dlg[i].ID == ch)
        {
            return i;
        }
    }
    return 0;
}

// ��ʾ״̬�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusMenu (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 master1[] = "����";
    u8 master2[] = "����";
    u8 key = KEY_NUL;

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
#if 0
    if(g_ucUpWorkingID == 1 && g_ucUpBackingID == 2)
    {
        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[0][i + 2] = master1[i];
            g_dlg[dlgId].MsgRow[1][i + 2] = master2[i];
        }
    }
    else if (g_ucUpWorkingID == 2 && g_ucUpBackingID == 1)
    {
        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[0][i + 2] = master2[i];
            g_dlg[dlgId].MsgRow[1][i + 2] = master1[i];
        }

    }
    if (g_ucDownWorkingID == 3 && g_ucDownBackingID == 4)
    {

        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[2][i + 2] = master1[i];
            g_dlg[dlgId].MsgRow[3][i + 2] = master2[i];
        }

    }
    else if (g_ucDownWorkingID == 4 && g_ucDownBackingID == 3)
    {
        for (i = 0; i < 4; i++)
        {
            g_dlg[dlgId].MsgRow[2][i + 2] = master2[i];
            g_dlg[dlgId].MsgRow[3][i + 2] = master1[i];
        }
    }
#endif
    if (g_ucConnectMode == 2)
    {
        g_dlg[dlgId].MsgRow[0][15] = 'X';
        g_dlg[dlgId].MsgRow[1][15] = 'X';
        g_dlg[dlgId].MsgRow[2][15] = 'X';
        g_dlg[dlgId].MsgRow[3][15] = 'X';
    }
    else
    {
        g_dlg[dlgId].MsgRow[0][15] = ' ';
        g_dlg[dlgId].MsgRow[1][15] = ' ';
        g_dlg[dlgId].MsgRow[2][15] = ' ';
        g_dlg[dlgId].MsgRow[3][15] = ' ';
    }

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            doShowMainMenu (DLG_MAIN, 0, NULL);         // ��������״̬
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_RIGHT:
            g_ucKeyPressCount++;
            if (g_ucKeyPressCount >= 6) // 2��֮����������6��,�������������
            {
                g_ucKeyPressCount = 0;
                while (1);    // ʲô������,�ȴ���λ
            }
            break;
        default:
            break;
    }

}

// ��ʾ�˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowMainMenu (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 key = KEY_NUL;
    u8 dlgId = check_menu(dlg_id);

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            switch (g_dlg[dlgId].highLightRow)
            {
                case 0:
                    doShowEmployeeMenu (DLG_EMPLOYEE_MENU, 1, NULL);
                    break;
                case 1:
                    doShowConnectModeSet (DLG_CONNETCT_SET, 5, NULL);
                    break;
                case 2:
                    doShowDebugMain (DLG_DEBUG_MAIN, 0, NULL);break;
                case 3:
                    //doShowWorkingSet (DLG_WORKING_SET, 1, NULL);
                    break;
                default:
                    break;
            }
            break;
        case KEY_UP:
            if (0 < g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow--;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_DOWN:
            if (2 > g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow++;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;    // �˳������˵�
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;    // �˳������˵�
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }
}

// Ա���ϰ��������,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowEmployeeMenu (u8 dlg_id, u8 isNotRow, void * p_parm)
{

    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u16 id = 0x7810 | g_ucCurID;                // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;               // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                        // CANͨ�ŵ�ID���ֽ�

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            break;
        case KEY_UP:
            if (1 < g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow--;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_DOWN:
            if (2 > g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow++;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_LEFT:
            break;
        case KEY_RIGHT:
            break;
        case KEY_OK:
            if ( g_ucConnectMode == 1)  // ֻ������ģʽ������һ���°�
            {
                switch (g_dlg[dlgId].highLightRow)
                {
                    case 1:
                        //myCANTransmit( gt_TxMessage, 1, 1, SPIT_ONE_BAD_CARD, 1, 0, 0, NO_FAIL );
                        //myCANTransmit( gt_TxMessage, 2, 2, SPIT_ONE_BAD_CARD, 2, 0, 0, NO_FAIL );
                        //myCANTransmit( gt_TxMessage, 3, 3, SPIT_ONE_BAD_CARD, 3, 0, 0, NO_FAIL );
                        //myCANTransmit( gt_TxMessage, 4, 4, SPIT_ONE_BAD_CARD, 4, 0, 0, NO_FAIL );
                        g_ucCurDlg = DLG_MAIN;
                        g_ucIsUpdateMenu = 1;
                        break;
                    case 2:
                        //myCANTransmit( gt_TxMessage, 1, 1, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL );
                        //myCANTransmit( gt_TxMessage, 2, 2, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL );
                        //myCANTransmit( gt_TxMessage, 3, 3, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL );
                        //myCANTransmit( gt_TxMessage, 4, 4, CARD_MACHINE_INIT, 0, 0, 0, NO_FAIL );
                        g_ucCurDlg = DLG_MAIN;
                        g_ucIsUpdateMenu = 1;
                        break;
                    default:
                        break;
                }
            }
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            //g_dlg[dlgId].highLightRow = 1;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
        case KEY_CANCEL:
            //g_dlg[dlgId].highLightRow = 1;
            g_ucCurDlg = DLG_MAIN;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }
}

// ���ÿ���������,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowCardCountSet (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 isSetMode = 0;   // ���������ģʽ/
    u8 i = 0;
    u8 j = 0;
    u8 seek = 0;        // ָʾ��ǰ���ڵ�����λ��
    u8 temp = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u16 id = 0x7810 | g_ucCurID;                        // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                       // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                                // CANͨ�ŵ�ID���ֽ�
    u8 cardCountShowSeek = 11;                          // �������ַ��Ӵ����еĵڼ�����ʾ,Ĭ�ϴ�11���ֽڿ�ʼ��ʾ
    u16 usaCardCount[4] = {1000,1001,1002,1003};        // ���п�����ʼ������������
    u8 str_aCardCount[4][6] = {0};                      // ���п�����ʼ��������������ʾ�ַ���

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        usaCardCount[i] = g_uiaInitCardCount[i + 1];    // ���Ƴ�ʼ�������������棬�Թ��޸�
    }
    for (i = 0; i < 4; i++)
    {
        sprintf(str_aCardCount[i],"%04d",usaCardCount[i]);      // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
    }

    for (i = 0; i < 4; i++)     // ���Ƴ�ʼ���������ַ����鵽��ʾ����
    {
        for (j = 0; j < 4; j++)
        {
            g_dlg[dlgId].MsgRow[i][j + cardCountShowSeek] = str_aCardCount[i][j];
        }
    }
    for (i = 0; i < 4; i++) // ��ʾ
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            switch (g_dlg[dlgId].highLightRow)
            {
                case 0:
                case 1:
                case 2:
                case 3:
                    isSetMode = 1;      // ��������ģʽ,��ȡ�����˳����ã��ٴΰ�ȡ�������˻���һ���˵�
                    displayGB2312String (0, isNotRow * 2, g_dlg[dlgId].MsgRow[isNotRow], 0);
                    sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);      // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����

                    displayGB2312StringLen (cardCountShowSeek, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0, 4); // ��ʾ4�ֽ�

                    isTurnShow(cardCountShowSeek / 2,isNotRow);
                    key = KEY_NUL;      // ���ٸ��½�����
                    g_ucIsUpdateMenu = 0;
                    break;
                default:
                    break;
            }
            break;
        case KEY_UP:
            if (isSetMode)
            {
                switch(seek)
                {
                    case 0:
                        temp = usaCardCount[isNotRow] / 1000;
                        usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 1000 : usaCardCount[isNotRow];
                        break;
                    case 1:
                        temp = usaCardCount[isNotRow] % 1000 / 100;
                        usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 100 : usaCardCount[isNotRow];
                        break;
                    case 2:
                        temp = usaCardCount[isNotRow] % 100 / 10;
                        usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 10 : usaCardCount[isNotRow];
                        break;
                    case 3:
                        temp = usaCardCount[isNotRow] % 10;
                        usaCardCount[isNotRow] = temp < 9 ? usaCardCount[isNotRow] + 1 : usaCardCount[isNotRow];
                        break;
                }
                sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
                for (i = 0; i < 4; i++)
                {
                    displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                }
                isTurnShow(0,isNotRow);
                key = KEY_NUL;      // ���ٸ��½�����
                g_ucIsUpdateMenu = 0;
            }
            else            // ���������ģʽ,���������ֵ
            {
                if (0 < isNotRow)
                {
                    isNotRow--;
                    isTurnShow(0,isNotRow);
                    key = KEY_NUL;
                }
                else
                {
                    key = KEY_NUL;
                }
            }
            break;
        case KEY_DOWN:
            if (isSetMode)
            {
                switch(seek)
                {
                    case 0:
                        temp = usaCardCount[isNotRow] / 1000;
                        usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 1000 : usaCardCount[isNotRow];
                        break;
                    case 1:
                        temp = usaCardCount[isNotRow] % 1000 / 100;
                        usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 100 : usaCardCount[isNotRow];
                        break;
                    case 2:
                        temp = usaCardCount[isNotRow] % 100 / 10;
                        usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 10 : usaCardCount[isNotRow];
                        break;
                    case 3:
                        temp = usaCardCount[isNotRow] % 10;
                        usaCardCount[isNotRow] = temp > 0 ? usaCardCount[isNotRow] - 1 : usaCardCount[isNotRow];
                        break;
                }
                sprintf(str_aCardCount[isNotRow],"%04d",usaCardCount[isNotRow]);        // ��ʽ����ʼ�����������ַ����飬�Թ���ʾ,�Ҷ���,��λ������0����
                for (i = 0; i < 4; i++)
                {
                    displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                }
                isTurnShow(0,isNotRow);
                key = KEY_NUL;      // ���ٸ��½�����
                g_ucIsUpdateMenu = 0;
            }
            else            // ���������ģʽ,���������ֵ
            {
                if (isNotRow < 3)
                {
                    isNotRow++;
                    isTurnShow(0,isNotRow);
                    key = KEY_NUL;
                }
                else
                {
                    key = KEY_NUL;
                }
            }

            break;
        case KEY_LEFT:
            if (isSetMode && seek > 0)
            {
                seek--;
                for (i = 0; i < 4; i++)
                {
                    displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                }
                isTurnShow(0,isNotRow);
            }
            key = KEY_NUL;
            g_ucIsUpdateMenu = 0;
            break;
        case KEY_RIGHT:
            if (isSetMode && seek < 3)
            {
                seek++;
                for (i = 0; i < 4; i++)
                {
                    displayGB2312String (cardCountShowSeek + i, isNotRow * 2, &str_aCardCount[isNotRow][i], i == seek ? 1 : 0);
                }
                isTurnShow(0,isNotRow);
            }
            key = KEY_NUL;      // ���ٸ��½�����
            g_ucIsUpdateMenu = 0;
            break;
        case KEY_OK:
            isSetMode = 0;      // �˳�����ģʽ
            seek = 0;                   // ���㵱ǰ���ڵ�����λ��
            g_uiaInitCardCount[isNotRow + 1] = usaCardCount[isNotRow];
            for (i = 0; i < 4; i++)     // �����޸Ĺ��Ŀ���������ʾ����
            {
                for (j = 0; j < 4; j++)
                {
                    g_dlg[dlgId].MsgRow[i][j + cardCountShowSeek] = str_aCardCount[i][j];
                }
            }
            break;
        case KEY_CANCEL:
            if (!isSetMode)
            {
                return;                     // ����Ƿ�����ģʽ,��ֱ���˳�
            }
            else
            {
                isSetMode = 0;              // �˳�����ģʽ
                seek = 0;                   // ���㵱ǰ���ڵ�����λ��
            }
            break;
        default:
            break;
    }
}

// ID��������ģʽ,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowConnectModeSet (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            key = KEY_NUL;
            break;
        case KEY_UP:
            if (1 < g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow--;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_DOWN:
            if (2 > g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow++;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_LEFT:
            break;
        case KEY_RIGHT:
            break;
        case KEY_OK:
            switch (g_dlg[dlgId].highLightRow)
            {
                case 1:
                    g_ucConnectMode = 1;
                    STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&g_ucConnectMode,1);
                    g_ucCurDlg = DLG_STATUS;
                    g_ucIsUpdateMenu = 1;
                    g_ucaDeviceStatus[0] = 0;
                    g_ucaDeviceStatus[1] = 0;
                    g_ucaDeviceStatus[2] = 0;
                    g_ucaDeviceStatus[3] = 0;
                    break;
                case 2:
                    g_ucConnectMode = 2;
                    STMFLASH_Write(FLASH_SAVE_ADDR,(u16*)&g_ucConnectMode,1);
                    g_ucCurDlg = DLG_STATUS;
                    g_ucIsUpdateMenu = 1;
                    g_ucaDeviceStatus[0] = 0;
                    g_ucaDeviceStatus[1] = 0;
                    g_ucaDeviceStatus[2] = 0;
                    g_ucaDeviceStatus[3] = 0;
                    break;
                default:
                    break;
            }
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            g_ucCurDlg = DLG_MAIN;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;

    }
}

// ״̬�˵�1,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusOne (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[5] = {0};
    u8 str_id[5] = {0};
    u16 id = 0x7810 | g_ucCurID;                    // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                   // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                            // CANͨ�ŵ�ID���ֽ�

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    //isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_RIGHT:
            doShowStatusTwo (DLG_STATUS_TWO, 5, NULL);
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_MAIN;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }
}

// ״̬�˵�2,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowStatusTwo (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[5] = {0};
    u8 str_id[5] = {0};
    u16 id = 0x7810 | g_ucCurID;                    // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                   // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                            // CANͨ�ŵ�ID���ֽ�

    g_ucCurDlg = dlg_id;                            // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    //isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_LEFT:
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS_ONE;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_CARD_MAIN;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }
}

// ID���ò˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowWorkingSet (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u8 master1[] = "1:����    2:����";
    u8 master2[] = "1:����    2:����";
    u8 master3[] = "3:����    4:����";
    u8 master4[] = "3:����    4:����";

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID
    if(g_ucUpWorkingID == 1 && g_ucUpBackingID == 2)
    {
        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[1][i] = master1[i];
        }
    }
    else if (g_ucUpWorkingID == 2 && g_ucUpBackingID == 1)
    {
        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[1][i] = master2[i];
        }
    }
    if (g_ucDownWorkingID == 3 && g_ucDownBackingID == 4)
    {

        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[2][i] = master3[i];
        }
    }
    else if (g_ucDownWorkingID == 4 && g_ucDownBackingID == 3)
    {
        for (i = 0; i < 16; i++)
        {
            g_dlg[dlgId].MsgRow[2][i] = master4[i];
        }
    }
    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            key = KEY_NUL;
            break;
        case KEY_UP:
            if (1 < g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow--;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_DOWN:
            if (2 > g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow++;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_LEFT:
            switch (g_dlg[dlgId].highLightRow)
            {
                case 1:
                    for (i = 0; i < 16; i++)
                    {
                        g_dlg[dlgId].MsgRow[g_dlg[dlgId].highLightRow][i] = master1[i];
                    }
                    g_ucUpWorkingID = 1;
                    g_ucUpBackingID = 2;
                    break;
                case 2:
                    for (i = 0; i < 16; i++)
                    {
                        g_dlg[dlgId].MsgRow[g_dlg[dlgId].highLightRow][i] = master3[i];
                    }
                    g_ucDownWorkingID = 3;
                    g_ucDownBackingID = 4;
                    break;
                default:
                    break;
            }
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_RIGHT:
            switch (g_dlg[dlgId].highLightRow)
            {
                case 1:
                    for (i = 0; i < 16; i++)
                    {
                        g_dlg[dlgId].MsgRow[g_dlg[dlgId].highLightRow][i] = master2[i];
                    }
                    g_ucUpWorkingID = 2;
                    g_ucUpBackingID = 1;
                    break;
                case 2:
                    for (i = 0; i < 16; i++)
                    {
                        g_dlg[dlgId].MsgRow[g_dlg[dlgId].highLightRow][i] = master4[i];
                    }
                    g_ucDownWorkingID = 4;
                    g_ucDownBackingID = 3;
                default:
                    break;

            }
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_OK:
            for (i = 0; i < 4; i++) // ÿ������������,��λ״̬
            {
                g_ucaDeviceStatus[i] = 0;
            }
            switch (g_dlg[dlgId].highLightRow)
            {
                case 1:
                    myCANTransmit(gt_TxMessage, g_ucUpWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                    myCANTransmit(gt_TxMessage, g_ucUpBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
                    g_dlg[dlgId].highLightRow++;
                    isTurnShow(0,g_dlg[dlgId].highLightRow);
                    break;
                case 2:
                    myCANTransmit(gt_TxMessage, g_ucDownWorkingID, 0, SET_MECHINE_STATUS, WORKING_STATUS, 0, 0, NO_FAIL);   // ���ù���̬
                    myCANTransmit(gt_TxMessage, g_ucDownBackingID, 0, SET_MECHINE_STATUS, BACKING_STATUS, 0, 0, NO_FAIL);   // ���ñ���̬
                    g_dlg[dlgId].highLightRow--;
                    isTurnShow(0,g_dlg[dlgId].highLightRow);
                    break;
                default:
                    break;
            }
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 1;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            g_dlg[dlgId].highLightRow = 1;
            g_ucCurDlg = DLG_MAIN;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;

    }
}

// ���Բ˵���ѡ��,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugMain (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u16 id = 0x7810 | g_ucCurID;                // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;               // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                        // CANͨ�ŵ�ID���ֽ�

    sprintf(str_num,"0%d",g_ucCurID);
    str_num [3] = 0;

    for (i = 0; i < 2; i++)
    {
        g_dlg[dlgId].MsgRow[0][i] = str_num[i];
    }

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == g_dlg[dlgId].highLightRow ? 1 : 0);
    }
    isTurnShow(0,g_dlg[dlgId].highLightRow);

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            switch (g_dlg[dlgId].highLightRow)
            {
                case 0:
                    break;
                case 1:
                    doShowDebugOne (DLG_DEBUG_ONE, 5, &(g_dlg[check_menu(DLG_DEBUG_MAIN)]) );
                    break;
                case 2:
                    doShowDebugTwo (DLG_DEBUG_TWO, 5, &(g_dlg[check_menu(DLG_DEBUG_MAIN)]) );
                    break;
                case 3:
                    doShowDebugThree (DLG_DEBUG_THREE, 5, &(g_dlg[check_menu(DLG_DEBUG_MAIN)]) );
                    break;
                default:
                    break;
            }
            break;
        case KEY_UP:
            if (0 < g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow--;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_DOWN:
            if (3 > g_dlg[dlgId].highLightRow)
            {
                g_dlg[dlgId].highLightRow++;
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_LEFT:
            if ((g_dlg[dlgId].highLightRow == 0) && g_ucCurID > 1 )
            {
                g_ucCurID--;
                sprintf(str_num, "0%d", g_ucCurID);
                str_num [3] = 0;
                for (i = 0; i < 2; i++)
                {
                    g_dlg[dlgId].MsgRow[0][i] = str_num[i];
                }
                displayGB2312String (0, 0, g_dlg[dlgId].MsgRow[0], 0);
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_RIGHT:
            if ((g_dlg[dlgId].highLightRow == 0) && g_ucCurID < 4)
            {
                g_ucCurID++;
                sprintf(str_num, "0%d", g_ucCurID);
                str_num [3] = 0;
                for (i = 0; i < 2; i++)
                {
                    g_dlg[dlgId].MsgRow[0][i] = str_num[i];
                }
                displayGB2312String (0, 0, g_dlg[dlgId].MsgRow[0], 0);
                isTurnShow(0,g_dlg[dlgId].highLightRow);
            }
            break;
        case KEY_QUIT:
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
        case KEY_CANCEL:
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_MAIN;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }
}

// ���Բ˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugOne (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u16 id = 0x7810 | g_ucCurID;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    Dlg *parm = (Dlg *)p_parm;

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);

    for (i = 0; i < 4; i++)
    {
        g_ucaDeviceStatus[i] = 0;
    }

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_UP:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_FORWARD, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 0;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_DOWN:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_REVERSAL, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 1;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_LEFT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_RESET, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 2;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_RIGHT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, HOOK_CARD, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 3;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_QUIT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, STOP_DEBUG, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, STOP_DEBUG, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_DEBUG_MAIN;
            g_dlg[g_ucCurDlg].highLightRow = 0;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }
}

// ���Բ˵�(����),�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugTwo (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u16 id = 0x7810 | g_ucCurID;                          // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                   // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                            // CANͨ�ŵ�ID���ֽ�
    Dlg *parm = (Dlg *)p_parm;

    g_ucCurDlg = dlg_id;                            // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);

    for (i = 0; i < 4; i++)
    {
        g_ucaDeviceStatus[i] = 0;
    }
    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_UP:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_FORWARD, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 0;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_DOWN:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_REVERSAL, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 1;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        /*
        case KEY_LEFT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, HOOK_MOTOR_POSITIVE_STEP, g_ucCurID, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 2;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_RIGHT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, HOOK_MOTOR_NEGATIVE_STEP, g_ucCurID, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 3;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        */
        case KEY_QUIT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, STOP_DEBUG, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, STOP_DEBUG, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_DEBUG_MAIN;
            g_dlg[g_ucCurDlg].highLightRow = 0;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }

}

// ���Բ˵�,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowDebugThree (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 dlgId = check_menu(dlg_id);
    u8 key = KEY_NUL;
    u8 str_num[10] = {0};
    u8 str_id[10] = {0};
    u16 id = 0x7810 | g_ucCurID;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    Dlg *parm = (Dlg *)p_parm;

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        displayGB2312String (0, i * 2, g_dlg[dlgId].MsgRow[i], i == isNotRow ? 1 : 0);
    }
    isTurnShow(0,isNotRow);

    for (i = 0; i < 4; i++)
    {
        g_ucaDeviceStatus[i] = 0;
    }

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_UP:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_FORWARD, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 0;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_DOWN:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_REVERSAL, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 1;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        case KEY_LEFT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, MOTOR_RESET, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 2;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        /*
        case KEY_RIGHT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, CYCLE_SPIT_ONE_CARD, g_ucCurID, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 3;
            isTurnShow(0,g_dlg[dlgId].highLightRow);
            break;
        */
        case KEY_QUIT:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, STOP_DEBUG, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_ucIsSetting = 0;
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            myCANTransmit(gt_TxMessage, g_ucCurID, g_ucCurID, STOP_DEBUG, parm->highLightRow, id_h, id_l, NO_FAIL);
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_DEBUG_MAIN;
            g_dlg[g_ucCurDlg].highLightRow = 0;
            g_ucIsUpdateMenu = 1;
            break;
        default:
            break;
    }
}

// ��ʾ������,�����һ����Ҫ����ʾ,�����õ�ǰ�з���ʾ,���ݲ�����ַ
void doShowFaultCode (u8 dlg_id, u8 isNotRow, void * p_parm)
{
    u8 i = 0;
    u8 j = 0;
    u8 n = 0;
    u8 dlgId = check_menu(dlg_id);
    volatile u8 key = KEY_NUL;
    u8 num = 1;      // ������
    u8 str_num[5] = {0};
    u16 id = 0x7810 | num;       // CANͨ�ŵ�ID
    u8 id_h = ( id >> 8 ) & 0xff;                // CANͨ�ŵ�ID���ֽ�
    u8 id_l = id & 0xff;                         // CANͨ�ŵ�ID���ֽ�
    u8 faultCodeIndex = 0;
    u8 faultCode = 0;

    g_ucCurDlg = dlg_id;    // ��¼��ǰ��ʾ��ID

    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < (sizeof (g_dlg_fault_code) / sizeof (g_dlg_fault_code[0])); j++)
        {
            if (g_ucaFaultCode[i] != 0 && g_ucaFaultCode[i] == g_dlg_fault_code[j].ID) // ���ҹ������Ӧ����ʾ����
            {
                faultCode = g_ucaFaultCode[i];   // ���µ�ǰ��δ����Ĺ��ϺͿ�����
                faultCodeIndex = i;
                num = i + 1;        // ��ס���ĸ������й���,���������
                sprintf(str_num,"%02d",num);
                for (n = 0; n < 2; n++)
                {
                    g_dlg_fault_code[j].MsgRow[0][n + 1] = str_num[n];
                }
                for (n = 0; n < 4; n++)
                {
                    displayGB2312String (0, n * 2, g_dlg_fault_code[j].MsgRow[n], 0);     // ��ʾ���Ͻ���
                }
                goto while_label;
            }
        }
    }

while_label:

    key = g_ucKeyValues;
    g_ucKeyValues = KEY_NUL;
    switch (key)
    {
        case KEY_ENTRY:
            // �ڹ�������ʾ����,���˵������Խ������ý���,�Ҳ��������ֵ
            g_ucIsSetting = 1;
            for (i = 0; i < 4; i++)     // ���Ͻ��֮��,�����־,����ȴ�״̬,�ȴ������ķ�������
            {
                g_ucaDeviceStatus[i] = 0;
            }
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        case KEY_CANCEL:
            g_ucaFaultCode[faultCodeIndex] = 0;
            myCANTransmit(gt_TxMessage, faultCodeIndex + 1, (faultCodeIndex + 1) % 2, CLEAR_FAULT_CODE, CLEAR_FAULT, NO_FAIL, NO_FAIL, faultCode);
            for (i = 0; i < 4; i++)     // ���Ͻ��֮��,�����־,����ȴ�״̬,�ȴ������ķ�������
            {
                g_ucaDeviceStatus[i] = 0;
            }
            g_dlg[dlgId].highLightRow = 0;
            g_ucCurDlg = DLG_STATUS;
            g_ucIsUpdateMenu = 1;
            break;
        default :
            break;
    }
}
