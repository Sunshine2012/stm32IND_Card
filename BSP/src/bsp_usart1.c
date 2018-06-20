/**
  ******************************************************************************
  * @file    bsp_usart1.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   重定向c库printf函数到usart端口
  ******************************************************************************
  * @attention
  *
  * 实验平台:
  * 论坛    :
  * 淘宝    :
  *
  ******************************************************************************
  */

#include "bsp_usart1.h"


u8 g_ucaTxBuff[SENDBUFF_SIZE] = "你好, 欢迎使用乐为电子发卡机系统\n";  // 使用全局的数据缓存
u8 g_ucaRxBuff[SENDBUFF_SIZE];  // 使用全局的数据缓存
u8 (*g_ucpTxBuff)[SENDBUFF_SIZE] = &g_ucaTxBuff;
u8 g_num = 0;

/**
   * @brief  USART1 RX DMA 配置，外设(USART1->DR)到内存
   * @param  无
   * @retval 无
   */
void USART1_RX_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    // 开启DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 设置DMA源地址：串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)USART1_DR_ADDRESS;
    // 内存地址(要传输的变量的指针)
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_ucaRxBuff;
    // 方向：从外设到内存
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    // 传输大小
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    // 外设地址不增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // 内存地址自增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // 外设数据单位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // 内存数据单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // DMA模式，一次或者循环模式
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    // 优先级：高
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    // 禁止内存到内存的传输
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    // 配置DMA通道
    DMA_Init(USART1_RX_DMA_CHANNEL, &DMA_InitStructure);
    // 使能DMA
    DMA_Cmd (USART1_RX_DMA_CHANNEL,ENABLE);
}



 /**
   * @brief  USARTx TX DMA 配置，内存到外设(USART1->DR)
   * @param  无
   * @retval 无
   */

void USART1_TX_DMA_Config(void)
{
    DMA_InitTypeDef DMA_InitStructure;

    // 开启DMA时钟
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    // 设置DMA源地址：串口数据寄存器地址*/
    DMA_InitStructure.DMA_PeripheralBaseAddr = USART1_DR_ADDRESS;
    // 内存地址(要传输的变量的指针)
    DMA_InitStructure.DMA_MemoryBaseAddr = (u32)g_ucpTxBuff;
    // 方向：从内存到外设
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    // 传输大小
    DMA_InitStructure.DMA_BufferSize = SENDBUFF_SIZE;
    // 外设地址不增
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    // 内存地址自增
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    // 外设数据单位
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    // 内存数据单位
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    // DMA模式，一次或者循环模式
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal ;
    //DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    // 优先级：高
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    // 禁止内存到内存的传输
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    // 配置DMA通道
    DMA_Init(USART1_TX_DMA_CHANNEL, &DMA_InitStructure);
    // 使能DMA
    DMA_Cmd (USART1_TX_DMA_CHANNEL,ENABLE);
 }

 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @param  无
  * @retval 无
  */
static void UART1_NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* 嵌套向量中断控制器组选择 */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  /* 配置USART为中断源 */
  NVIC_InitStructure.NVIC_IRQChannel = macUSART1_IRQ;
  /* 抢断优先级*/
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  /* 子优先级 */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  /* 使能中断 */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  /* 初始化配置NVIC */
  NVIC_Init(&NVIC_InitStructure);
}
 /**
  * @brief  USARTx GPIO 配置,工作模式配置。115200 8-N-1
  * @param  无
  * @retval 无
  */
void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    // 打开串口GPIO的时钟
    macUSART1_GPIO_APBxClock_FUN(macUSART1_GPIO_CLK, ENABLE);

    // 打开串口外设的时钟
    macUSART1_APBxClock_FUN(macUSART1_CLK, ENABLE);

    // 将USART Tx的GPIO配置为推挽复用模式
    GPIO_InitStructure.GPIO_Pin = macUSART1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(macUSART1_TX_PORT, &GPIO_InitStructure);

    // 将USART Rx的GPIO配置为浮空输入模式
    GPIO_InitStructure.GPIO_Pin = macUSART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(macUSART1_RX_PORT, &GPIO_InitStructure);

    // 配置串口的工作参数
    // 配置波特率
    USART_InitStructure.USART_BaudRate = macUSART1_BAUD_RATE;
    // 配置 针数据字长
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    // 配置停止位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    // 配置校验位
    USART_InitStructure.USART_Parity = USART_Parity_No ;
    // 配置硬件流控制
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    // 配置工作模式，收发一起
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    // 完成串口的初始化配置
    USART_Init(macUSART1, &USART_InitStructure);

    // 串口中断优先级配置
    UART1_NVIC_Configuration();

    // 使能串口接收中断
    //USART_ITConfig(macUSART1, USART_IT_RXNE, ENABLE);

    //USART_ITConfig(macUSART1, USART_IT_RXNE, DISABLE);
    //USART_ITConfig(macUSART1, USART_IT_TC,   DISABLE);
    //USART_ITConfig(macUSART1, USART_IT_TXE,  DISABLE);
    /* 使能IDLE中断 */
    USART_ITConfig(macUSART1, USART_IT_IDLE, ENABLE);

    // 主要是配置外设地址和内存地址以及缓冲区大小，
    // 配置好后DMA就会自动的把串口数据存到相应的内存地址。
    USART1_RX_DMA_Config();

    USART1_TX_DMA_Config();

    //配置串口1DMA接收
    USART_DMACmd(macUSART1, USART_DMAReq_Rx, ENABLE);

    // USART1 向 DMA发出TX请求
    USART_DMACmd(macUSART1, USART_DMAReq_Tx, ENABLE);

    // 使能串口
    USART_Cmd(macUSART1, ENABLE);

    // 清除发送完成标志
    //USART_ClearFlag(USART1, USART_FLAG_TC);
}


/*****************  通过DMA发送字符串 **********************/
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

/*****************  发送一个字符 **********************/
void USART1_SendByte( USART_TypeDef * pUSARTx, uint8_t ch)
{
    /* 发送一个字节数据到USART */
    USART_SendData(pUSARTx,ch);

    /* 等待发送数据寄存器为空 */
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void USART1_SendString( USART_TypeDef * pUSARTx, char *str)
{
    unsigned int k=0;
    do
    {
        USART1_SendByte( pUSARTx, *(str + k) );
        k++;
    } while(*(str + k)!='\0');

    /* 等待发送完成 */
    while(USART_GetFlagStatus(pUSARTx,USART_FLAG_TC)==RESET)
    {}
}

/*****************  发送一个16位数 **********************/
void Usart1_SendHalfWord( USART_TypeDef * pUSARTx, uint16_t ch)
{
    uint8_t temp_h, temp_l;

    /* 取出高八位 */
    temp_h = (ch&0XFF00)>>8;
    /* 取出低八位 */
    temp_l = ch&0XFF;

    /* 发送高八位 */
    USART_SendData(pUSARTx,temp_h);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);

    /* 发送低八位 */
    USART_SendData(pUSARTx,temp_l);
    while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
}

/*****************  发送字符串 **********************/
void USART1_nSendString(USART_TypeDef * pUSARTx,char *str,int n)
{
    unsigned int k=0;
    do
    {
        /* 发送一个字节数据到USART */
        USART_SendData(pUSARTx,str[k]);

        /* 等待发送数据寄存器为空 */
        while (USART_GetFlagStatus(pUSARTx, USART_FLAG_TXE) == RESET);
        k++;
    } while(k < n);
}

#ifdef uart1
///重定向c库函数printf到串口，重定向后可使用printf函数
int fputc(int ch, FILE *f)
{
    u32 i = 1000000;
    /* 等待发送完毕,即检测上次发送的数据 */
    while (USART_GetFlagStatus(macUSART1, USART_FLAG_TXE) == RESET)
    {
        if (i-- == 0)
        {
            break;
        }
    }
    /* 发送一个字节数据到串口 */
    USART_SendData(macUSART1, (u8) ch);

    return (ch);
}

///重定向c库函数scanf到串口，重写向后可使用scanf、getchar等函数
int fgetc(FILE *f)
{
    u32 i = 1000;
    /* 等待串口输入数据 */
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
