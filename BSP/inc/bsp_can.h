#ifndef __BSP_CAN_H__
#define __BSP_CAN_H__

#include <includes.h>

typedef enum {FAILED = 0, PASSED = !FAILED} TestStatus;


//volatile TestStatus TestRx;



u8 myCANTransmit(CanTxMsg Msg, u8 mechine_id, u8 boxNum, u8 cmd, u8 status,
                      u8 data_H, u8 data_L, u8 errNum);
u8 myCANTransmit_ID(CanTxMsg Msg, u8 targetID, u8 mechineID, u8 boxNum, u8 cmd, u8 status,
                      u8 data_H, u8 data_L, u8 errNum);

static void CAN_RCC_Config(void);
static void CAN_GPIO_Config(void);
static void CAN_NVIC_Config(void);

void canInit( void );
void CAN_Interrupt(void);
void CAN_Polling(void);
void CANTransmit (void * p_Msg);

void USB_LP_CAN1_RX0_IRQHandler(void);


extern CanTxMsg gt_TxMessage;      // CAN·¢ËÍÊý¾Ý»º´æ

#endif
