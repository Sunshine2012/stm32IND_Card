/**
  ******************************************************************************

  *
  ******************************************************************************
  */



#include <includes.h>
#include "bsp_i2c2.h"

static __IO uint32_t  IICTimeout = IICT_LONG_TIMEOUT;

/*信息输出*/
#define IIC_DEBUG_ON         1

#define IIC_INFO(fmt,arg...)           printf("<<-IIC-FLASH-INFO->> "fmt"\n",##arg)
#define IIC_ERROR(fmt,arg...)          printf("<<-IIC-FLASH-ERROR->> "fmt"\n",##arg)
#define IIC_DEBUG(fmt,arg...)          do{\
                                            if(IIC_DEBUG_ON)\
                                            printf("<<-IIC-FLASH-DEBUG->> [%d]"fmt"\n",__LINE__, ##arg);\
                                            }while(0)


 /**
  * @brief  I2C_Configuration，初始化硬件IIC引脚
  * @param  无
  * @retval 无
  */
void I2C_Configuration(void)
{
    I2C_InitTypeDef  I2C_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

    /* 配置 RESET 引脚 */


    /*STM32F103RET6芯片的硬件I2C2: PC11 -- SCL; PC12 -- SDA */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11 | GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;//I2C必须开漏输出
    GPIO_Init(GPIOC, &GPIO_InitStructure);


    I2C_DeInit(I2C2);//使用I2C
    I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
    I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
    I2C_InitStructure.I2C_OwnAddress1 = 0x0;//主机的I2C地址
    I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
    I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
    I2C_InitStructure.I2C_ClockSpeed = 100000;//100K

    I2C_Cmd(I2C2, ENABLE);
    I2C_Init(I2C2, &I2C_InitStructure);
    I2C_AcknowledgeConfig(I2C2, ENABLE);
}


 /**
  * @brief  I2C_WriteByte，向OLED寄存器地址写一个byte的数据
  * @param  addr：寄存器地址
    *                    data：要写入的数据
  * @retval 无
  */
void I2C_WriteByte(uint8_t addr,uint8_t data)
{
    IICTimeout = IICT_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY))
    {
        if ((IICTimeout--) == 0) // 超时退出
        {
           IIC_TIMEOUT_UserCallback(0);
           goto fault;
        }
    }

    I2C_GenerateSTART(I2C2, ENABLE);//开启I2C2

    IICTimeout = IICT_LONG_TIMEOUT;
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT))/*EV5,主模式*/
    {
        if ((IICTimeout--) == 0)
        {
           IIC_TIMEOUT_UserCallback(0);
           goto fault;
        }
    }

    I2C_Send7bitAddress(I2C2, OLED_ADDRESS, I2C_Direction_Transmitter);//器件地址 -- 默认0x78

    IICTimeout = IICT_LONG_TIMEOUT;
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
    {
        if ((IICTimeout--) == 0)
        {
           IIC_TIMEOUT_UserCallback(0);
           goto fault;
        }
    }

    I2C_SendData(I2C2, addr);//寄存器地址

    IICTimeout = IICT_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if ((IICTimeout--) == 0)
        {
           IIC_TIMEOUT_UserCallback(0);
           goto fault;
        }
    }

    I2C_SendData(I2C2, data);//发送数据

    IICTimeout = IICT_LONG_TIMEOUT;
    while (!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED))
    {
        if ((IICTimeout--) == 0)
        {
           IIC_TIMEOUT_UserCallback(0);
           goto fault;
        }
    }

fault:
    I2C_GenerateSTOP(I2C2, ENABLE);//关闭I2C2总线
}


 u8 I2C_ReadByte(u8 addr)
 {
    uint8_t data_byte;
    //*((u8 *)0x4001080c) |=0x80;
    while(I2C_GetFlagStatus(I2C2, I2C_FLAG_BUSY));

    /* Send START condition */
    I2C_GenerateSTART(I2C2, ENABLE);
    //*((u8 *)0x4001080c) &=~0x80;

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for write */
    I2C_Send7bitAddress(I2C2, 0x0, I2C_Direction_Transmitter);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED));

    /* Clear EV6 by setting again the PE bit */
    I2C_Cmd(I2C2, ENABLE);

    /* Send the EEPROM's internal address to write to */
    I2C_SendData(I2C2, addr);

    /* Test on EV8 and clear it */
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_TRANSMITTED));

    /* Send STRAT condition a second time */
    I2C_GenerateSTART(I2C2, ENABLE);

    /* Test on EV5 and clear it */
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_MODE_SELECT));

    /* Send EEPROM address for read */
    I2C_Send7bitAddress(I2C2, addr, I2C_Direction_Receiver);

    /* Test on EV6 and clear it */
    while(!I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED));

    /* While there is data to be read */
    /* Disable Acknowledgement */
    I2C_AcknowledgeConfig(I2C2, DISABLE);

    /* Send STOP Condition */
    I2C_GenerateSTOP(I2C2, ENABLE);

    /* Test on EV7 and clear it */
    if(I2C_CheckEvent(I2C2, I2C_EVENT_MASTER_BYTE_RECEIVED))
    /* Read a byte from the EEPROM */
    data_byte = I2C_ReceiveData(I2C2);


    /* Enable Acknowledgement to be ready for another reception */
    I2C_AcknowledgeConfig(I2C2, ENABLE);
    //I2C_EE_WaitEepromStandbyState();
    return data_byte;
 }


 /**
  * @brief  WriteCmd，向OLED写入命令
  * @param  I2C_Command：命令代码
  * @retval 无
  */
void WriteCmd(unsigned char I2C_Command)//写命令
{
    I2C_WriteByte(0x00, I2C_Command);
}


 /**
  * @brief  WriteDat，向OLED写入数据
  * @param  I2C_Data：数据
  * @retval 无
  */
void WriteDat(unsigned char I2C_Data)//写数据
{
    I2C_WriteByte(0x40, I2C_Data);
}


/**
  * @brief  等待超时回调函数
  * @param  None.
  * @retval None.
  */
static  uint16_t IIC_TIMEOUT_UserCallback(uint8_t errorCode)
{
    /* 等待超时后的处理,输出错误信息 */
    IIC_ERROR("IIC 等待超时!errorCode = %d",errorCode);
    return 0;
}
