#include <includes.h>

CanTxMsg gt_TxMessage;      // CAN发送数据缓存

static void CAN_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

#if 0
    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );    //重影射CAN IO脚到 PD0，PD1

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );    //重影射CAN IO脚到 PD0，PD1
#endif

    /* Configure CAN pin: RX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* Configure CAN pin: TX */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    //GPIO_PinRemapConfig(GPIO_Remap2_CAN1, ENABLE );      //重影射CAN IO脚到 PB8，PB9,    如果是使用PA12发送，PA11则不需要重映射
}

//系统中断管理
void CAN_NVIC_Config(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    /* Configure the NVIC Preemption Priority Bits */
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
#if 0
    #ifdef  VECT_TAB_RAM
        /* Set the Vector Table base location at 0x20000000 */
        NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    #else  /* VECT_TAB_FLASH  */
        /* Set the Vector Table base location at 0x08000000 */
        NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    #endif
#endif
    /* enabling interrupt */
    NVIC_InitStructure.NVIC_IRQChannel=USB_LP_CAN1_RX0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

//配置系统时钟,使能各外设时钟
void CAN_RCC_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE );
    /* CAN Periph clock enable */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
}

u8 myCANTransmit_ID(CanTxMsg mMsg, u8 targetID, u8 mechineID, u8 boxNum, u8 cmd, u8 status,
                      u8 data_H, u8 data_L, u8 errNum)
{
    u32 i = 0;
    //static u8 uCount = 0;
    u8 TransmitMailbox;

    mMsg.StdId = 0x00;
    mMsg.ExtId = 0x7810 | targetID;
    mMsg.RTR = CAN_RTR_DATA;
    mMsg.IDE = CAN_ID_EXT;;
    mMsg.DLC = 8;
    mMsg.Data[0] = g_uiSerNum;
    mMsg.Data[1] = mechineID;
    mMsg.Data[2] = boxNum;
    mMsg.Data[3] = cmd;
    mMsg.Data[4] = status;
    mMsg.Data[5] = data_H;
    mMsg.Data[6] = data_L;
    mMsg.Data[7] = errNum;

    TransmitMailbox = CAN_Transmit(CAN1,&mMsg);
    i = 0;
    while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i <= 0xffff))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending(CAN1,CAN_FIFO0) < 1) && (i <= 0xffff))
    {
        i++;
    }
    g_uiSerNum++;           // 帧序号每次加1
    return 0;
}


u8 myCANTransmit (CanTxMsg mMsg, u8 mechine_id, u8 boxNum, u8 cmd, u8 status,
                      u8 data_H, u8 data_L, u8 errNum)
{
    u32 i = 0;
    u8 TransmitMailbox = 0;
    u8 ucMechineIdTemp = 0;
    switch( mechine_id )
    {
        case 1:
        case 2:
            ucMechineIdTemp = 0x01;
            break;
        case 3:
        case 4:
            ucMechineIdTemp = 0x02;
            break;
        default:
            ucMechineIdTemp = 0x03;
            break;
    }
    mMsg.StdId = 0x00;
    mMsg.ExtId = 0x7810 | ucMechineIdTemp;
    mMsg.RTR = CAN_RTR_DATA;
    mMsg.IDE = CAN_ID_EXT;
    mMsg.DLC = 8;
    mMsg.Data[0] = g_uiSerNum;
    mMsg.Data[1] = mechine_id;
    mMsg.Data[2] = boxNum;
    mMsg.Data[3] = cmd;
    mMsg.Data[4] = status;
    mMsg.Data[5] = data_H;
    mMsg.Data[6] = data_L;
    mMsg.Data[7] = errNum;
    while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i < 0xFFFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending(CAN1,CAN_FIFO0) < 1) && (i < 0xFFFF))
    {
        i++;
    }
    TransmitMailbox = CAN_Transmit(CAN1,&mMsg);
    g_uiSerNum++;           // 帧序号每次加1
    //g_siCycleAskMsgTime = 2; // 在空挡期,4秒钟之后查询主备机
    return 0;
}


void CANTransmit (void * p_Msg)
{
    u32 i = 0;
    u8 TransmitMailbox;
    CanTxMsg *p_TxMessage = (CanTxMsg *)p_Msg;

    TransmitMailbox = CAN_Transmit(CAN1,p_TxMessage);
    i = 0;
    while((CAN_TransmitStatus(CAN1,TransmitMailbox) != CANTXOK) && (i != 0xFF))
    {
        i++;
    }

    i = 0;
    while((CAN_MessagePending(CAN1,CAN_FIFO0) < 1) && (i != 0xFF))
    {
        i++;
    }
}

/*******************************************************************************
* Function Name  : CAN_Polling
* Description    : Configures the CAN and transmit and receive by polling
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
*******************************************************************************/
void CAN_Polling(void)
{
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);
  // aiwesky 20140506
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_6tq;
  CAN_InitStructure.CAN_BS2=CAN_BS2_5tq;
  CAN_InitStructure.CAN_Prescaler=6;
  CAN_Init(CAN1,&CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;
  CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_FIFO0;
  //
  CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

}

/*******************************************************************************
* Function Name  : CAN_Interrupt
* Description    : Configures the CAN and transmit and receive by interruption
* Input          : None
* Output         : None
* Return         : PASSED if the reception is well done, FAILED in other case
*******************************************************************************/
void CAN_Interrupt(void)
{
  //http://blog.csdn.net/flydream0/article/details/8148791   设置来源为此网站
  //u32 ext_id =0x7810;    // 设置过滤ID的屏蔽位。
  CAN_InitTypeDef        CAN_InitStructure;
  CAN_FilterInitTypeDef  CAN_FilterInitStructure;

  /* CAN register init */
  CAN_DeInit(CAN1);
  CAN_StructInit(&CAN_InitStructure);

  /* CAN cell init */
  CAN_InitStructure.CAN_TTCM=DISABLE;
  CAN_InitStructure.CAN_ABOM=DISABLE;
  CAN_InitStructure.CAN_AWUM=DISABLE;
  CAN_InitStructure.CAN_NART=DISABLE;
  CAN_InitStructure.CAN_RFLM=DISABLE;
  CAN_InitStructure.CAN_TXFP=DISABLE;
  CAN_InitStructure.CAN_Mode=CAN_Mode_Normal;
  // 波特率为   36MHz / CAN_Prescaler / (CAN_SJW + CAN_BS1 + CAN_BS2)    = 50kps    aiwesky 20171022
  CAN_InitStructure.CAN_SJW=CAN_SJW_1tq;
  CAN_InitStructure.CAN_BS1=CAN_BS1_10tq;           // CAN_BS1_6tq ------> CAN_BS1_10tq
  CAN_InitStructure.CAN_BS2=CAN_BS2_7tq;            // CAN_BS1_5tq ------> CAN_BS1_7tq
  CAN_InitStructure.CAN_Prescaler=40;               // 4 -----> 40
  CAN_Init(CAN1,&CAN_InitStructure);

  /* CAN filter init */
  CAN_FilterInitStructure.CAN_FilterNumber=0;
  CAN_FilterInitStructure.CAN_FilterMode=                CAN_FilterMode_IdMask;   // 如果是过滤 '一组' 标识符(ID),则使用CAN_FilterMode_IdMask模式,如果是过滤 '一个'标识符(ID),则使用CAN_FilterMode_IdList
  CAN_FilterInitStructure.CAN_FilterScale=               CAN_FilterScale_32bit;
  CAN_FilterInitStructure.CAN_FilterIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterIdLow=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x0000;
  CAN_FilterInitStructure.CAN_FilterMaskIdLow=0x0000;

  //CAN_FilterInitStructure.CAN_FilterIdHigh=              ((ext_id<<3)>>16) & 0xffff;
  //CAN_FilterInitStructure.CAN_FilterIdLow=               ((ext_id<<3)& 0xffff)  | CAN_ID_EXT;      //

  //CAN_FilterInitStructure.CAN_FilterMaskIdHigh=          ((ext_id<<3)>>16) & 0xffff;
  //CAN_FilterInitStructure.CAN_FilterMaskIdLow=           ((ext_id<<3)& 0xffff) | CAN_ID_EXT;     // 如果是列表模式,则不需要设置屏蔽位寄存器
  CAN_FilterInitStructure.CAN_FilterFIFOAssignment=      CAN_FIFO0;
  CAN_FilterInitStructure.CAN_FilterActivation=          ENABLE;
  CAN_FilterInit(&CAN_FilterInitStructure);

  /* CAN FIFO0 message pending interrupt enable */
  CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);


}

CanRxMsg gt_RxMessage = {0};   // 发布消息，应该使用全局的变量，否则出错    20170720
/*******************************************************************************
* Function Name  : USB_LP_CAN_RX0_IRQHandler
* Description    : This function handles USB Low Priority or CAN RX0 interrupts
*                  requests.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USB_LP_CAN1_RX0_IRQHandler(void)
{

    CAN_ITConfig(CAN1,CAN_IT_FMP0, DISABLE);
    CAN_Receive(CAN1,CAN_FIFO0, &gt_RxMessage);
    // CANTransmit (&gt_RxMessage);
    if(((0x0000ff00 & gt_RxMessage.ExtId) == 0x00007800) && (gt_RxMessage.IDE == CAN_ID_EXT))
    {
        if ( CYCLE_ACK == gt_RxMessage.Data[3] )            // 定时查询是否有卡
        {
            g_ucaMechineExist[ gt_RxMessage.Data[1] - 1 ] = 1;
            if ( HAS_CARD  == gt_RxMessage.Data[4])
            {
                g_ucaCardIsReady[gt_RxMessage.Data[1] - 1] = 1;
            }
            else
            {
                g_ucaCardIsReady[gt_RxMessage.Data[1] - 1] = 0;
            }
            g_ucaMasterStandbyStatus[gt_RxMessage.Data[1] - 1] = gt_RxMessage.Data[2];
            g_ucaStatus[gt_RxMessage.Data[1] - 1] = gt_RxMessage.Data[7];

            if ( FAULT_CODE11 == g_ucaFaultCode[gt_RxMessage.Data[1] - 1] )     // 如果之前是CAN总线故障，则清除故障
            {
                g_ucaFaultCode[gt_RxMessage.Data[1] - 1] = NO_FAIL;
                copyMenu ( gt_RxMessage.Data[1], CONNECTED, 0, 8, 6 );
                g_ucIsUpdateMenu    = 1;
                g_ucCurDlg = DLG_STATUS;
                myCANTransmit(gt_TxMessage, gt_RxMessage.Data[1], NO_FAIL, CLEAR_FAULT_CODE, CLEAR_FAULT, NO_FAIL, NO_FAIL, FAULT_CODE11);   // 如果报告总线故障码,本机直接处理
            }
        }
        else if ( 0xaa == gt_RxMessage.Data[3] )    // 调试命令
        {
            switch (gt_RxMessage.Data[4])
            {
            case 1:
                g_ucKeyValues = KEY_ENTRY;
                break;
            case 2:
                g_ucKeyValues = KEY_OK;
                break;
            case 3:
                g_ucKeyValues = KEY_CANCEL;
                break;
            case 4:
                g_ucKeyValues = KEY_QUIT;
                break;
            case 5:
                g_ucKeyValues = KEY_UP;
                break;
            case 6:
                g_ucKeyValues = KEY_DOWN;
                break;
            case 7:
                g_ucKeyValues = KEY_LEFT;
                break;
            case 8:
                g_ucKeyValues = KEY_RIGHT;
                break;
            default:
                break;
            }
        }
        else
        {
            canInQueue (&g_tCanRxQueue, &gt_RxMessage);
        }
        //g_siaCheck[gt_RxMessage.Data[1] - 1] = 6000;        // 60秒没有收到卡机回复,发初始化命令,报警

    }
    else
    {

    }
    CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);
    CAN_ITConfig(CAN1,CAN_IT_FMP0, ENABLE);
}

void canInit( void )
{
    CAN_RCC_Config ();      // 初始化总线
    CAN_GPIO_Config ();     // 初始化GPIO,需要用到TX:PA12 ,RX:PA11
    CAN_NVIC_Config ();     // 接收中断
    CAN_Interrupt ();       // 设置波特率等其他参数
}
