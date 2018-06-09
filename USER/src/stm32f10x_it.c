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

// �����жϷ�����
// �ѽ��յ������ݴ���һ�����黺�������棬�����յ��ĵ�ֵ����">"ʱ����ֵ����
void macUSART1_IRQHandler( void )
{
    static u16 i = 0;
    if ( USART_GetITStatus( macUSART1, USART_IT_IDLE ) != RESET )
    {
        i = USART1->SR;
        i = USART1->DR;
        USART_ClearITPendingBit(macUSART1, USART_IT_IDLE);

        g_num = SENDBUFF_SIZE - DMA_GetCurrDataCounter (USART1_RX_DMA_CHANNEL); // ��ȡ�յ����ֽڳ���
        g_ucaRxBuff[g_num] = 0;    // ������β��ʶ��

        if ( FRAME_START ==  g_ucaRxBuff[0] \
          && FRAME_END == g_ucaRxBuff[g_num - 1] )      // ����յ���������һ֡����,����
        {
            // ���������Ӧ��֡,�������
            if( POSITIVE_ACK == g_ucaRxBuff[2] \
             || NAGATIVE_ACK == g_ucaRxBuff[2] )
            {
                g_num = 0;
                g_ucaRxBuff[0] = 0;
                DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//�ر�DMA
                USART1_RX_DMA_CHANNEL->CNDTR = SENDBUFF_SIZE;
                DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//����DMA
                return;
            }
            // ��ֹ���ڿ����ж�
            USART_ITConfig(macUSART1, USART_IT_IDLE, DISABLE);

            DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//�ر�DMA,��ֹ�������������

            g_tP_RsctlFrame.RSCTL = g_ucaRxBuff[1];
            g_ucP_RsctlFrame = 1;
            g_ucaRxBuff[g_num + 1] = 0;    // ������β��ʶ��
            /* ������Ϣ����Ϣ���� queue */
            uartInQueue( &g_tUARTRxQueue, g_ucaRxBuff ); // �����Ǿ���,���Բ�����������

            g_num = 0;
            g_ucaRxBuff[0] = 0;
            USART1_RX_DMA_CHANNEL->CNDTR = SENDBUFF_SIZE;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//����DMA
            USART_ITConfig(macUSART1, USART_IT_IDLE, ENABLE);
        }
        else // ���͸�Ӧ��
        {
            USART1_SendStringFromDMA ((char *)&g_tN_RsctlFrame , sizeof (g_tN_RsctlFrame));
            g_num = 0;
            g_ucaRxBuff[0] = 0;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, DISABLE);//�ر�DMA,��ֹ�������������
            USART1_RX_DMA_CHANNEL->CNDTR = SENDBUFF_SIZE;
            DMA_Cmd(USART1_RX_DMA_CHANNEL, ENABLE);//����DMA
        }
    }
}


#if uart4

// �����жϷ�����
// �ѽ��յ������ݴ���һ�����黺�������棬�����յ��ĵ�ֵ����">"ʱ����ֵ����
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
        else if ( g_rx_buf[g_num] == FRAME_END ) // �����յ���ֵ����'>'ʱ������һ֡����
        {
            g_rx_buf[g_num + 1] = 0;    // ������β��ʶ��

            /* ������Ϣ����Ϣ���� queue */
            uartInitQueue( &g_tUARTRxQueue, g_rx_buf ); // �����Ǿ���,���Բ�����������
        }

        // ��ֵ����ʱ�������������һ��
        else
        {
            g_num++;

            if ( g_num > 50 ) //һ֡�������50�ֽ�,��������
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
