#ifndef __MENU_H
#define __MENU_H
#include <includes.h>

// dlg ID
typedef enum DLG_ID
{
    DLG_CLEAR_LCD                = 0,
    DLG_LOGO                     = 1,                 /*  */
    DLG_STATUS                   = 2,                 /*  */
    DLG_MAIN                     = 3,                 /*  */
    DLG_PASSWD                   = 4,                 /*  */
    DLG_CARD_ID                  = 5,                 /*  */
    DLG_WORKING_SET              = 6,                 /*  */

    DLG_EMPLOYEE_MENU            = 7,                 /*  */

    DLG_CARD_MAIN                = 20,                 /*  */
    DLG_STATUS_ONE               = 21,                 /*  */
    DLG_STATUS_TWO               = 22,                 /*  */

    DLG_CONNETCT_SET             = 23,                 /*  */

    DLG_CARD_COUNT_SET           = 30,                 /*  */

    DLG_DEBUG_MAIN               = 40,                 /*  */
    DLG_DEBUG_ONE                = 41,                 /*  */
    DLG_DEBUG_TWO                = 42,                 /*  */
    DLG_DEBUG_THREE              = 43,                 /*  */

    DLG_FAULT_CODE               = 60,                 /* 故障码 */

}DLG_ID;

// 界面结构体
typedef struct Dlg
{
   u8 ID;
   char MsgRow[4][36];
   u8 highLightRow;
   u8 prevDlgID;            // 上一个菜单ID
   u8 nextDlgID;            // 下一个菜单ID
}Dlg;


void doShowStatusMenu (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowMainMenu (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowEmployeeMenu (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowCardCountSet (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowConnectModeSet (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowStatusOne (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowStatusTwo (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowIdSetMenu (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowWorkingSet (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowDebugMain (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowDebugOne (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowDebugTwo (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowDebugThree (u8 dlg_id, u8 isNotRow, void * p_parm);
void doShowFaultCode (u8 dlg_id, u8 isNotRow, void * p_parm);
u8 check_menu(u8 ch);

#endif
