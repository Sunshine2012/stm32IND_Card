/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   �ض���c��printf������usart�˿�
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:
  * ��̳    :
  * �Ա�    :
  *
  ******************************************************************************
  */

#include "bsp_usart1.h"


u8 g_ucaTxBuff[SENDBUFF_SIZE] = "���, ��ӭʹ����Ϊ���ӷ�����ϵͳ\n";  // ʹ��ȫ�ֵ����ݻ���
u8 g_ucaRxBuff[SENDBUFF_SIZE];  // ʹ��ȫ�ֵ����ݻ���
u8 (*g_ucpTxBuff)[SENDBUFF_SIZE] = &g_ucaTxBuff;
u8 g_num = 0;

/**
   * @brief  USART1 RX DMA ���ã�����(USART1->DR)���ڴ�
   * @param  ��
   * @retval ��
   */
void USART1_RX_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    // ����DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // ����DMAԴ��ַ���������ݼĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)USART1_DR_ADDRESS;
    // �ڴ��ַ(Ҫ����ı�����ָ��)
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_ucaRxBuff;
    // ���򣺴����赽�ڴ�
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    // �����С
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    // �����ַ����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // �ڴ��ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // �������ݵ�λ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // �ڴ����ݵ�λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // DMAģʽ��һ�λ���ѭ��ģʽ
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    // ���ȼ�����
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    // ��ֹ�ڴ浽�ڴ�Ĵ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    // ����DMAͨ��
    DMA_Init(USART1_RX_DMA_CHANNEL, &DMA_InitStructure);
    // ʹ��DMA
    DMA_Cmd (USART1_RX_DMA_CHANNEL,ENABLE);
}



 /**
   * @brief  USARTx TX DMA ���ã��ڴ浽����(USART1->DR)
   * @param  ��
   * @retval ��
   */

void USART1_TX_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    // ����DMAʱ��
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // ����DMAԴ��ַ���������ݼĴ�����ַ*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_ADDRESS;
    // �ڴ��ַ(Ҫ����ı�����ָ��)
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_ucpTxBuff;
    // ���򣺴��ڴ浽����
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // �����С
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    // �����ַ����
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // �ڴ��ַ����
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // �������ݵ�λ
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // �ڴ����ݵ�λ
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // DMAģʽ��һ�λ���ѭ��ģʽ
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    // ���ȼ�����
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    // ��ֹ�ڴ浽�ڴ�Ĵ���
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    // ����DMAͨ��
    DMA_Init(USART1_TX_DMA_CHANNEL, &DMA_InitStructure);
    //����DMA������ɺ�����ж�
    DMA_ITConfig(USART1_TX_DMA_CHANNEL,DMA_IT_TC,ENABLE);
    // ʹ��DMA
    DMA_Cmd (USART1_TX_DMA_CHANNEL,ENABLE);
 }

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void UART1_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = macUSART1_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  ����Ƕ�������жϿ�����NVIC
  * @param  ��
  * @retval ��
  */
static void UART1_DMA_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Ƕ�������жϿ�������ѡ�� */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  /* ����USARTΪ�ж�Դ */
  NVIC_InitStructure.NVIC_IRQChannel = macUSART1_DMA_TX_IRQ;
  /* �������ȼ�*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  /* �����ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  /* ʹ���ж� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* ��ʼ������NVIC */
  NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USARTx GPIO ����,����ģʽ���á�115200 8-N-1
  * @param  ��
  * @retval ��
  */
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // �򿪴���GPIO��ʱ��
    macUSART1_GPIO_APBxClock_FUN(macUSART1_GPIO_CLK, ENABLE);

    // �򿪴��������ʱ��
    macUSART1_APBxClock_FUN(macUSART1_CLK, ENABLE);

    // ��USART Tx��GPIO����Ϊ���츴��ģʽ
    GPIO_InitStructure.GPIO_Pin = macUSART1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macUSART1_TX_PORT, &GPIO_InitStructure);

    // ��USART Rx��GPIO����Ϊ��������ģʽ
    GPIO_InitStructure.GPIO_Pin = macUSART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(macUSART1_RX_PORT, &GPIO_InitStructure);

    // ���ô��ڵĹ�������
    // ���ò�����
    USART_InitStructure.USART_BaudRate = macUSART1_BAUD_RATE;
    // ���� �������ֳ�
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    // ����ֹͣλ
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    // ����У��λ
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    // ����Ӳ��������
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    // ���ù���ģʽ���շ�һ��
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // ��ɴ��ڵĳ�ʼ������
    USART_Init(macUSART1, &USART_InitStructure);

    // �����ж����ȼ�����
    UART1_NVIC_Configuration();
    UART1_DMA_NVIC_Configuration();

    // ʹ�ܴ��ڽ����ж�
    //USART_ITConfig(macUSART1, USART_IT_RXNE, ENABLE);

    //USART_ITConfig(macUSART1, USART_IT_RXNE, DISABLE);
    //USART_ITConfig(macUSART1, USART_IT_TC,   DISABLE);
    //USART_ITConfig(macUSART1, USART_IT_TXE,  DISABLE);
    /* ʹ��IDLE�ж� */
    USART_ITConfig(macUSART1, USART_IT_IDLE, ENABLE);

    // ��Ҫ�����������ַ���ڴ��ַ�Լ���������С��
    // ���úú�DMA�ͻ��Զ��İѴ������ݴ浽��Ӧ���ڴ��ַ��
    USART1_RX_DMA_Config();

    USART1_TX_DMA_Config();

    //���ô���1DMA����
    USART_DMACmd(macUSART1, USART_DMAReq_Rx, ENABLE);

    // USART1 �� DMA����TX����
    USART_DMACmd(macUSART1, USART_DMAReq_Tx, ENABLE);

    // ʹ�ܴ���
    USART_Cmd(macUSART1, ENABLE);

    // ���������ɱ�־
    //USART_ClearFlag(USART1, USART_FLAG_TC);
}


/*****************  ͨ��DMA�����ַ��� **********************/
void USART1_SendStringFromDMA( u8 *str, u32 len )
{
    if ( ( NULL != str ) && ( 0 != len ) )
    {
        DMA_Cmd(USART1_TX_DMA_CHANNEL,DISABLE);
        USART1_TX_DMA_CHANNEL->CNDTR = len;
        USART1_TX_DMA_CHANNEL->CMAR = (u32)str;
        DMA_Cmd(USART1_TX_DMA_CHANNEL,ENABLE);
    }
}

/*****************  ����һ���ַ� **********************/
void USART1_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* ����һ���ֽ����ݵ�USART */
    USART_SendData(pUSARTx,ch);

    /* �ȴ��������ݼĴ���Ϊ�� */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  �����ַ��� **********************/
void USART1_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do
    {
        USART1_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');

    /* �ȴ�������� */
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
    {}
}

/*****************  ����һ��16λ�� **********************/
void Usart1_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
    uint8_t temp_h, temp_l;

    /* ȡ���߰�λ */
    temp_h = (ch&0XFF00)>>8;
    /* ȡ���Ͱ�λ */
    temp_l = ch&0XFF;

    /* ���͸߰�λ */
    USART_SendData(pUSARTx,temp_h);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

    /* ���͵Ͱ�λ */
    USART_SendData(pUSARTx,temp_l);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  �����ַ��� **********************/
void USART1_nSendString(USART_TypeDef * pUSARTx,char *str,int n)
{
    unsigned int k=0;
    do
    {
        /* ����һ���ֽ����ݵ�USART */
        USART_SendData(pUSARTx,str[k]);

        /* �ȴ��������ݼĴ���Ϊ�� */
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
        k++;
    } while(k < n);
}

#ifdef uart1
///�ض���c�⺯��printf�����ڣ��ض�����ʹ��printf����
int fputc(int ch, FILE *f)
{
    u32 i = 1000000;
    /* �ȴ��������,������ϴη��͵����� */
    while (USART_GetFlagStatus(macUSART1, USART_FLAG_TXE) == RESET)
    {
        if (i-- == 0)
        {
            break;
        }
    }
    /* ����һ���ֽ����ݵ����� */
    USART_SendData(macUSART1, (u8) ch);

    return (ch);
}

///�ض���c�⺯��scanf�����ڣ���д����ʹ��scanf��getchar�Ⱥ���
int fgetc(FILE *f)
{
    u32 i = 1000;
    /* �ȴ������������� */
    while (USART_GetFlagStatus(macUSART1, USART_FLAG_RXNE) == RESET)
    {
        if (i-- == 0)
        {
            break;
        }
    }
    return (int)USART_ReceiveData(macUSART1);
}
/*********************************************END OF FILE**********************/
#endif
