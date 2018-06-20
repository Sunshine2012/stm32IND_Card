#ifndef __BSP_GENERALTIME_H
#define __BSP_GENERALTIME_H


#include "stm32f10x.h"


/**************通用定时器TIM参数定义，只限TIM2、3、4、5************/
// 当需要哪个定时器的时候，只需要把下面的宏定义改成1即可
#define GENERALTIM2    1
#define GENERALTIM3    1
#define GENERALTIM4    1
#define GENERALTIM5    1

#if  GENERALTIM2
#define            GENERAL_TIM2                   TIM2
#define            GENERAL_TIM2_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM2_CLK               RCC_APB1Periph_TIM2
#define            GENERAL_TIM2_Period            (50000-1)     // 2s 中断
#define            GENERAL_TIM2_Prescaler         2840
#define            GENERAL_TIM2_IRQ               TIM2_IRQn
#define            GENERAL_TIM2_IRQHandler        TIM2_IRQHandler

#endif

#if  GENERALTIM3
#define            GENERAL_TIM3                   TIM3
#define            GENERAL_TIM3_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM3_CLK               RCC_APB1Periph_TIM3
#define            GENERAL_TIM3_Period            (10000-1)      // 10ms 中断
#define            GENERAL_TIM3_Prescaler         71
#define            GENERAL_TIM3_IRQ               TIM3_IRQn
#define            GENERAL_TIM3_IRQHandler        TIM3_IRQHandler

#endif

#if   GENERALTIM4
#define            GENERAL_TIM4                   TIM4
#define            GENERAL_TIM4_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM4_CLK               RCC_APB1Periph_TIM4
#define            GENERAL_TIM4_Period            (1000-1)
#define            GENERAL_TIM4_Prescaler         71
#define            GENERAL_TIM4_IRQ               TIM4_IRQn
#define            GENERAL_TIM4_IRQHandler        TIM4_IRQHandler

#endif

#if   GENERALTIM5
#define            GENERAL_TIM5                   TIM5
#define            GENERAL_TIM5_APBxClock_FUN     RCC_APB1PeriphClockCmd
#define            GENERAL_TIM5_CLK               RCC_APB1Periph_TIM5
#define            GENERAL_TIM5_Period            (1000-1)
#define            GENERAL_TIM5_Prescaler         71
#define            GENERAL_TIM5_IRQ               TIM5_IRQn
#define            GENERAL_TIM5_IRQHandler        TIM5_IRQHandler

#endif
/**************************函数声明********************************/

extern s32 g_siKeyTime;                 // 按键状态计时变量
extern s32 g_siCycleAskMsgTime;         // ms 计时变量
//extern s32 g_siKeyMsgLockTime;        // 卡机按键上报状态的锁定时间
extern s32 g_siStatusOverTimeS;         // 发卡机状态短超时
extern s32 g_siStatusOverTimeL;         // 发卡机状态长超时
extern s32 g_siKeyPressTime;            // 锁定按键的时间
extern s32 g_siOutCardMsgTime;          // 重复发送翻卡信息的次数
extern s32 g_siCardTakeMsgTime;         // 重复发送取卡信息的次数
extern s32 g_siCheckStatus;             // 检查卡机的状态时间间隔
extern s32 g_siaCheck[4];               // 检查卡机的通讯状态时间间隔
extern s32 g_siSendToPcMsgTime;         // 通过串口发送数据的间隔

void generalTIM2Init(void);
void generalTIM3Init(void);
void generalTIM4Init(void);
void generalTIM5Init(void);


#endif  /* __BSP_GENERALTIME_H */


