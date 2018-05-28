#ifndef __FRAME_H
#define __FRAME_H

#include <includes.h>

#define FRAME_4             4      /* ֡�ֽڳ��� */
#define FRAME_6             6      /* ֡�ֽڳ��� */

extern u8 g_ucSerNum;      // ֡���    ȫ��

typedef enum FRAME
{
    FRAME_START             = '<',      /* ֡��ʼ */
    POSITIVE_ACK            = '0',      /* ��Ӧ����Ϣ(30H)֡            4�ֽ� */
    NAGATIVE_ACK            = '1',      /* ��Ӧ����Ϣ(31H)֡            4�ֽ� */

    /* �Զ���������PC������Ϣ֡��PC��Ӧ�� */
    CARD_MACHINE_POWER_ON   = 'A',      /* �����ϵ���Ϣ(41H)֡          4�ֽ� */
    CARD_MACHINE_STATUES    = 'B',      /* ״̬��Ϣ(42H)֡             30�ֽ� */
    CARD_SPIT_OUT           = 'C',      /* �ѳ�����Ϣ(43H)֡            6�ֽ� */
    CARD_KEY_PRESS          = 'D',      /* ��ťȡ����Ϣ(44H)֡          6�ֽ� */
    CARD_TAKE_AWAY          = 'E',      /* ����ȡ����Ϣ(45H)֡          6�ֽ� */
    CARD_REPORT_SPIT_STATUES= 'F',      /* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */

    /* PC�����Զ�����������Ϣ֡�뿨��Ӧ�� */
    PC_INIT_MECHINE         = 'a',      /* ��ʼ��������Ϣ(61H)֡       20�ֽ� */
    PC_SPIT_OUT_CARD        = 'b',      /* ������Ϣ(62H)֡              5�ֽ� */
    PC_BAD_CARD             = 'c',      /* ������Ϣ(63H)֡              5�ֽ� */
    PC_QUERY_CARD_MECHINE   = 'e',      /* ��ѯ����״̬(65H)֡          5�ֽ� */
    PC_QUERY_CARD_CLIP      = 'f',      /* ��ѯ����(66H)֡              5�ֽ� */
    PC_SET_CARD_NUM         = 'g',      /* ���ÿ��п���(67H)֡          8�ֽ� */
    PC_CAR_HAS_COMING       = 'h',      /* ��������Ϣ */
    PC_CAR_HAS_GONE         = 'i',      /* ��������Ϣ */
    MECHINE_CODE_VERSION    = 'v',      /* �汾 */

    PC_GET_DIST             = 't',      /* ��� */
    FRAME_END               = '>',      /* ֡���� */
}FRAME;

// Ӧ����Ϣ֡ 4�ֽ�
typedef struct RSCTL_FREME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ���ͣ���(30H), ��(31H) */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}RSCTL_FREME;

///====================================
// �Զ�������->PC
// �����ϵ���Ϣ֡ 4�ֽ�
typedef struct CARD_MACHINE_POWER_ON_FREME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ֡���� */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}CARD_MACHINE_POWER_ON_FREME;

// ������״̬��Ϣ 6�ֽ�
typedef struct CARD_SPIT_STATUES
{
    u8 status;                  /* ����״̬ */
    u8 spitIsExist;             /* �����Ƿ�װ�� */
    u8 cardNum[3];              /* �����п�����ֵ�����������ϵĿ� */
    u8 antHasCard;              /* �����Ƿ��п� */
}CARD_SPIT_STATUES;


// ״̬��Ϣ֡ 30�ֽ�
typedef struct CARD_MACHINE_STATUES_FRAME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ֡���� */
    u8 UP_SPIT_IS_OK;       /* �Ϲ�λ��ǰ�������� */
    u8 DOWN_SPIT_IS_OK;     /* �¹�λ��ǰ�������� */
    CARD_SPIT_STATUES CARD_MECHINE1;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE2;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE3;/* ��λ��Ϣ */
    CARD_SPIT_STATUES CARD_MECHINE4;/* ��λ��Ϣ */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}CARD_MACHINE_STATUES_FRAME;


// 6�ֽ�
typedef struct CARD_MECHINE_TO_PC_FRAME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ֡���� */
    u8 CARD_MECHINE;        /* ��λ��Ϣ */
    u8 MECHINE_ID;          /* ������� */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}CARD_MECHINE_TO_PC_FRAME;

// �ϱ��������к�״̬֡ 29
typedef struct CARD_REPORT_SPIT_STATUES_FRAME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ֡���� */
    u8 CARD_SPIT1[8];       /* 1#�����ڿ��б�� */
    u8 CARD_SPIT2[8];       /* 2#�����ڿ��б�� */
    u8 CARD_SPIT3[8];       /* 3#�����ڿ��б�� */
    u8 CARD_SPIT4[8];       /* 4#�����ڿ��б�� */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}CARD_REPORT_SPIT_STATUES_FRAME;

///====================================
// PC->�Զ�������
// ��ʼ��֡ 20�ֽ�
typedef struct PC_TO_CARD_INIT_FREME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ֡���� */
    u8 CARD_NUM[3];         /* ���������� */
    u8 DATE[14];            /* ʱ�� */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}PC_TO_CARD_INIT_FREME;

// 5�ֽ�
typedef struct PC_TO_CARD_MECHINE_FRAME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ֡���� */
    u8 DATA;                /* ������ */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}PC_TO_CARD_MECHINE_FRAME;

// ���ÿ��п���֡ 8�ֽ�
typedef struct PC_SET_CARD_NUM_FRAME
{
    const u8 STX;           /* ֡��ʼ */
    u8 RSCTL;               /* ֡��� */
    u8 CTL;                 /* ֡���� */
    u8 CARD_SPIT_ID[1];     /* ���к� */
    u8 CARD_NUM[3];         /* ���п��� */
    const u8 ETX;           /* ֡���� */
    const u8 END;
}PC_SET_CARD_NUM_FRAME;
///====================================


// ������Ϣ�ṹ��
typedef struct Print_msg
{
    const u8 CTL;
    char Msg[40];
    const u8 END;
}Print_msg;

// ����
typedef enum CMD
{
    // 00H--0FH����ʼ������ʱ������Ϣ����

    CARD_MACHINE_RESET      = 0x00,                 /* (����--������)��λ���� */
    CARD_MACHINE_INIT       = 0x01,                 /* (����--������)��ʼ��λ������ */
    CARD_MACHINE_INIT_ACK   = 0x02,                 /* (����--������) ��ʼ���ظ���������״̬�͹������� */
    CYCLE_ASK               = 0x03,                 /* ��ʱѯ�� */
    CYCLE_ACK               = 0x04,                 /* ��ʱ�ظ� */
    SET_MECHINE_STATUS      = 0x05,                 /* ��������״̬���� */
    SET_MECHINE_STATUS_ACK  = 0x06,                 /* ��������״̬���ûظ� */

    // 10H--1FH�������巢������������Ϣ����
    SET_MECHINE_ID          = 0x10,                 /* ���ÿ�����ID�� */
    STOP_WORKING            = 0x11,                 /* ֹͣ���� */
    MACHINE_STATUES         = 0x12,                 /* ָ����λ�������������״̬���� */
    WRITE_CARD_STATUS       = 0x13,                 /* д�������������״̬���� */
    CARD_SPIT_NOTICE_ACK    = 0x14,                 /* ����֪ͨ�ظ�  */
    CARD_TAKE_AWAY_NOTICE_ACK= 0x15,                /* ����ȡ�߻ظ�  */
    CARD_READY_ACK          = 0x16,                 /* ������ */
    CLEAR_FAULT_CODE        = 0x17,                 /* �������  */

    FAULT_CODE_ACK          = 0x19,                 /* ������Ϣ�ظ�  */

    // 20H--2FH�������巢������������Ϣ����
    MACHINE_CHECK_CARD      = 0x21,                 /* ָ����λ�鿨 */
    KEY_PRESS               = 0x22,                 /* ˾���Ѱ��� */
    CARD_SPIT_NOTICE        = 0x23,                 /* ����֪ͨ��������״̬�͹������� */
    CARD_TAKE_AWAY_NOTICE   = 0x24,                 /* ���ѱ�ȡ��֪ͨ */
    CARD_IS_READY           = 0x25,                 /* ���Ѿ��� */
    SERCH_CARD_MECHINE_ACK  = 0x26,                 /* ����ѯ�ʻش� */
    MECHINE_WARNING         = 0x27,                 /* ���� */
    BAD_CARD_FULL           = 0x28,                 /* ���������� */

    // 30H--3FH���ܵ���������Ϣ
    DEBUG_ALL               = 0x30,
    ENTER_DEBUG             = 0x31,
    QUIT_DEBUG              = 0x32,

    // 40H--5FH����������������Ϣ
    STOP_DEBUG              = 0x40,                 /* ֹͣ���� */
    MOTOR_FORWARD           = 0x41,                 /* �����ת */
    MOTOR_REVERSAL          = 0x42,                 /* �����ת */
    MOTOR_RESET             = 0x43,                 /* ������� */
    HOOK_CARD               = 0x44,                 /* ��һ�ſ� */

    //
    SPIT_MOTOR_POSITIVE_STEP= 0x45,                 /* ������������� */
    SPIT_MOTOR_NEGATIVE_STEP= 0x46,                 /* ������������� */

    HOOK_MOTOR_POSITIVE_STEP= 0x47,                 /* ������������� */
    HOOK_MOTOR_NEGATIVE_STEP= 0x48,                 /* ������������� */

    SPIT_MOTOR_POSITIVE     = 0x49,                 /* ��������������� */
    SPIT_MOTOR_NEGATIVE     = 0x4A,                 /* ��������������� */
    SPIT_MOTOR_ZERO         = 0x4B,                 /* �����������λ */

    HOOK_MOTOR_POSITIVE     = 0x4C,                 /* ��������������� */
    HOOK_MOTOR_NEGATIVE     = 0x4D,                 /* ��������������� */
    HOOK_MOTOR_ZERO         = 0x4E,                 /* �����������λ */

    READ_KEY_INFO           = 0x4F,                 /* ��ȡ������Ϣ */
    LED_ON                  = 0x50,                 /* ָʾ���� */
    LED_OFF                 = 0x51,                 /* ָʾ���� */
    READ_SENSOR             = 0x52,                 /* ��ȡ������״̬ */
}CMD;

// ������״̬��00--FFH�� (����������������״̬)
typedef enum CARD_AND_MECHINE_STATUS
{
    // 00H--0FH����״̬
    ALL_IS_OK                  = 0x00,                 /* ALL IS OK */

    WORKING_STATUS             = 0x01,                 /* ����״̬ */
    BACKING_STATUS             = 0x02,                 /* ����״̬ */
    DEBUGING_STATUS            = 0x03,                 /* ����״̬ */
                                                       /* ������ */
                                                       /* ������ */
                                                       /* ʹ����ͨ�� */


    // 10H--1FH����״̬
    HAS_CARD                   = 0x10,                 /* �п� */
    HAS_NO_CARD                = 0x11,                 /* �޿� */
    CARD_IS_OK                 = 0x12,                 /* �ÿ� */
    CARD_IS_BAD                = 0x13,                 /* ���� */
    HAS_CHECKED_CARD           = 0x14,                 /* ���п� */
    HAS_WRITE_CARD             = 0x15,                 /* ��д�� */
    IS_WORKING                 = 0x20,
    IS_BACKING                 = 0x21,
    // 20H--2FH����״̬

    // 30H--7FH������

    // 80H--FFH����������״̬
    CYCLE_SPIT_CARD_SUCCESE    = 0x80,                 /* ����ѭ�������ɹ� */
    CYCLE_SPIT_CARD_FAIL       = 0x81,                 /* ����ѭ������ʧ�� */
    SPIT_ONE_CARD_SUCCESS      = 0x82,                 /* ѭ����һ�ſ��ɹ� */
    SPIT_ONE_CARD_FAIL         = 0x83,                 /* ѭ����һ�ſ�ʧ�� */
    TURN_ONE_CARD_SUCCESS      = 0x84,                 /* ��һ�ſ��ɹ� */
    TURN_ONE_CARD_FAIL         = 0x85,                 /* ��һ�ſ�ʧ�� */
    SEND_CARD_SUCCESS          = 0x86,                 /* ��һ�ſ��ɹ� */
    SEND_CARD_FAIL             = 0x87,                 /* ��һ�ſ�ʧ�� */
    SEND_CARD_SENSOR_OK        = 0x88,                 /* �Ϳ�������״̬���� */
    SEND_CARD_SENSOR_FAULT     = 0x89,                 /* �Ϳ�������״̬���� */
    CARD_BOX_SENSOR_OK         = 0x8a,                 /* ���д�����״̬���� */
    CARD_BOX_SENSOR_FAULT      = 0x8b,                 /* ���д�����״̬���� */
    CARD_OUT_SENSOR_OK         = 0x8c,                 /* ȡ��������״̬���� */
    CARD_OUT_SENSOR_FAULT      = 0x8d,                 /* ȡ��������״̬���� */


    DISCONNECTED               = 0xfc,                 /* ���� */

    CONNECTED                  = 0xfe

}CARD_AND_MECHINE_STATUS;

// �����룺00--�޹���
typedef enum FAULT_CODE
{

    NO_FAIL                     = 0x00,                 /* �޹��� */
    FAULT_CODE01                = 0x01,                 /* ��ʼ������������ܻ�λ */
    FAULT_CODE02                = 0x02,                 /* ��ʼ�����������ת���ܻ�λ */
    FAULT_CODE03                = 0x03,                 /* ��ʼ�����������ת���ܻ�λ */
    FAULT_CODE04                = 0x04,                 /* ����ʱ���������ǰ��ת */
    FAULT_CODE05                = 0x05,                 /* ����ʱ�����������ת */
    FAULT_CODE06                = 0x06,                 /* ����ʱ�������ǰ����ת */
    FAULT_CODE07                = 0x07,                 /* ����ʱ�������ǰ���س̶�ת */
    FAULT_CODE08                = 0x08,                 /* ����ʱ�������������ת */
    FAULT_CODE09                = 0x09,                 /* ����ʱ������������س̶�ת */
    FAULT_CODE0A                = 0x0a,                 /* ����ʱ���������ת */
    FAULT_CODE0B                = 0x0b,                 /* ����ʱ���������ת */
    FAULT_CODE0C                = 0x0c,                 /* ͨѶ���� */
    FAULT_CODE0D                = 0x0d,
    FAULT_CODE0E                = 0x0e,
    FAULT_CODE0F                = 0x0f,
    FAULT_CODE10                = 0x10,                 /* �޿� */
    FAULT_CODE11                = 0x11,                 /* CAN���߹��� */

    CLEAR_FAULT                 = 0x21

}FAULT_CODE;

extern const Print_msg                  g_taPri_msg[];                      /**/

extern RSCTL_FREME                      g_tP_RsctlFrame;                    /* ��Ӧ����Ϣ(30H)֡            4�ֽ� */
extern RSCTL_FREME                      g_tN_RsctlFrame;                    /* ��Ӧ����Ϣ(31H)֡            4�ֽ� */

extern CARD_MACHINE_POWER_ON_FREME      g_tCardMechinePowerOnFrame;         /* �����ϵ���Ϣ(41H)֡          4�ֽ� */
extern CARD_MACHINE_STATUES_FRAME       g_tCardMechineStatusFrame;          /* ״̬��Ϣ(42H)֡             30�ֽ� */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardSpitOutFrame;                /* �ѳ�����Ϣ(43H)֡            6�ֽ� */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardKeyPressFrame;               /* ��ťȡ����Ϣ(44H)֡          6�ֽ� */
extern CARD_MECHINE_TO_PC_FRAME         g_tCardTakeAwayFrame;               /* ����ȡ����Ϣ(45H)֡          6�ֽ� */
extern CARD_REPORT_SPIT_STATUES_FRAME   g_tCardReportSpitStatusFrame;       /* �ϱ����кű����Ϣ(46H)֡   36�ֽ� */


extern PC_TO_CARD_INIT_FREME            g_tPcToCardInitFrame;               /* ��ʼ��������Ϣ(61H)֡       20�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcSpitOutCardFrame;              /* ������Ϣ(62H)֡              5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcBadCardFrame;                  /* ������Ϣ(63H)֡              5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardMechineFrame;         /* ��ѯ����״̬(65H)֡          5�ֽ� */
extern PC_TO_CARD_MECHINE_FRAME         g_tPcQuetyCardCpipFrame;            /* ��ѯ����(66H)֡              5�ֽ� */
extern PC_SET_CARD_NUM_FRAME            g_tPcSetCardNumFrame;               /* ���ÿ��п���(67H)֡          8�ֽ� */

extern unsigned int g_uiaInitCardCount[5];    // ����ʼ����ֵ,[0]Ϊ�ܿ�����,��1�ſ�,��1,[1~4]Ϊÿ��������ʼ������,��1�ſ�,��1.
extern unsigned int g_uiaSpitCardCount[5];    // ��������,[0]Ϊ����������,��1�ſ�,��1,[1~4]Ϊÿ��������������,��1�ſ�,��1.

void antSwitch(u8 id);
u8  analyzeCANFrame ( CanRxMsg arg );
u8  analyzeUartFrame ( const u8 argv[], u32 size );
u8 * checkShowFaultCode (u8 ch);
u8 * checkShowStatusMsg (u8 ch);
u8 * checkShowMsg (u8 ch);
u8 * checkPriMsg (u8 ch);
void copyMenu (u8 num, u8 id, u8 values, u8 addr, u8 count);
void copyStatusMsg (u8 num, u8 id, u8 values, u8 addr, u8 count);
void copyWarningMsg (u8 num, u8 cmd, u8 values, u8 addr, u8 count);

#endif /* __FRAME_H */
