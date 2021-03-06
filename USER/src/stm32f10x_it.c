/**
  ******************************************************************************
  * @file    Project/STM32F10x_StdPeriph_Template/stm32f10x_it.c
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    08-April-2011
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTI

  AL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "includes.h"
/** @addtogroup STM32F10x_StdPeriph_Template
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M3 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

// 串口中断服务函数
// 把接收到的数据存在一个数组缓冲区里面，当接收到的的值等于">"时，把值返回
void macUSART1_IRQHandler( void )
{
    static u16 i = 0;
    if ( USART_GetITStatus( macUSART1, USART_IT_IDLE ) != RESET )
    {
        i = USART1->SR;
        i = USART1->DR;
        USART_ClearITPendingBit(macUSART1, USART_IT_IDLE);

        g_num = SENDBUFF_SIZE - DMA_GetCurrDataCounter (USART1_RX_DMA_CHANNEL); // 获取收到的字节长度
        g_ucaRxBuff[g_num] = 0;    // 加上行尾标识符

        if ( FRAME_START ==  g_ucaRxBuff[0] \
          && FRAME_END == g_ucaRxBuff[g_num - 1] )      // 如果收到的完整的一帧数据,则处理
        {
            // 如果是正负应答帧,不入队列
            if( POSITIVE_ACK == g_ucaRxBuff[2] \
             || NAGATIVE_ACK == g_ucaRxBuff[2] )
            {
                g_num = 0;
                g_ucaRxBuff[0] = 0;
                DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//关闭DMA
                USART1_RX_DMA_CHANNEL->CNDTR = SENDBUFF_SIZE;
                DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//开启DMA
                return;
            }
            // 禁止串口空闲中断
            USART_ITConfig(macUSART1, USART_IT_IDLE, DISABLE);

            DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//关闭DMA,防止处理其间有数据

            g_tP_RsctlFrame.RSCTL = g_ucaRxBuff[1];
            //g_ucP_RsctlFrame = 1;
            uartInQueue( &g_tUARTTxQueue, (char *)&g_tP_RsctlFrame ); // 不考虑竞争,所以不设置自旋锁
            g_ucaRxBuff[g_num + 1] = 0;    // 加上行尾标识符
            /* 发布消息到消息队列 queue */
            uartInQueue( &g_tUARTRxQueue, g_ucaRxBuff ); // 不考虑竞争,所以不设置自旋锁

            g_num = 0;
            g_ucaRxBuff[0] = 0;
            USART1_RX_DMA_CHANNEL->CNDTR = SENDBUFF_SIZE;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//开启DMA
            USART_ITConfig(macUSART1, USART_IT_IDLE, ENABLE);
        }
        else // 发送负应答
        {
            uartInQueue( &g_tUARTTxQueue, (char *)&g_tN_RsctlFrame ); // 不考虑竞争,所以不设置自旋锁

            g_num = 0;
            g_ucaRxBuff[0] = 0;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//关闭DMA,防止处理其间有数据
            USART1_RX_DMA_CHANNEL->CNDTR = SENDBUFF_SIZE;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//开启DMA
        }
    }
}


// 串口发送中断服务函数
void macUSART1_DMA_TX_IRQHandler( void )
{
    static u16 i = 0;

    if( DMA_GetITStatus(DMA1_IT_TC4) != RESET )
    {
        g_siSendToPcMsgTime = 4;
        //memset ( g_ucaUartTxMsg,0,50 );
        //if ( 0 == uartOutQueue( &g_tUARTTxQueue, g_ucaUartTxMsg ) )
        //{
        //    USART1_SendStringFromDMA( (char *)g_ucaUartTxMsg, strlen( (const char *)g_ucaUartTxMsg ) );
        //}
        DMA_ClearITPendingBit(DMA1_IT_TC4); //清除全部中断标志
    }
}

#if uart4

// 串口中断服务函数
// 把接收到的数据存在一个数组缓冲区里面，当接收到的的值等于">"时，把值返回
void macUSART4_IRQHandler( void )
{
    if ( USART_GetITStatus( macUSART4, USART_IT_RXNE ) != RESET )
    {
        g_rx_buf[g_num] = USART_ReceiveData( macUSART4 );

        if ( g_rx_buf[g_num] == FRAME_START )
        {
            memset( g_rx_buf, 0, sizeof( g_rx_buf ) );
            g_rx_buf[0] = FRAME_START;
            g_num = 0;
            g_num++;
        }
        else if ( g_rx_buf[g_num] == FRAME_END ) // 当接收到的值等于'>'时，结束一帧数据
        {
            g_rx_buf[g_num + 1] = 0;    // 加上行尾标识符

            /* 发布消息到消息队列 queue */
            uartInitQueue( &g_tUARTRxQueue, g_rx_buf ); // 不考虑竞争,所以不设置自旋锁
        }

        // 当值不等时候，则继续接收下一个
        else
        {
            g_num++;

            if ( g_num > 50 ) //一帧数据最大50字节,超出则丢弃
            {
                g_num = 0;
            }
        }
    }
}
#endif

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/**
  * @}
  */


/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
