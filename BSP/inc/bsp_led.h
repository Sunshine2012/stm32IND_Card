#ifndef __LED_H_
#define	__LED_H_

#include "stm32f10x.h"

/****************************** LED 引脚配置参数定义***************************************/
#define             macLED1_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED1_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macLED1_GPIO_PORT                       GPIOB
#define             macLED1_GPIO_PIN                        GPIO_Pin_6

#define             macLED2_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED2_GPIO_CLK                        RCC_APB2Periph_GPIOC
#define             macLED2_GPIO_PORT                       GPIOC
#define             macLED2_GPIO_PIN                        GPIO_Pin_0

#define             macLED3_GPIO_APBxClock_FUN              RCC_APB2PeriphClockCmd
#define             macLED3_GPIO_CLK                        RCC_APB2Periph_GPIOB
#define             macLED3_GPIO_PORT                       GPIOB
#define             macLED3_GPIO_PIN                        GPIO_Pin_4

/****************************** LED 函数宏定义***************************************/
#define             macLED1_ON()                            GPIO_ResetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )
#define             macLED1_OFF()                           GPIO_SetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )
#define             macLED1_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED1_GPIO_PORT, macLED1_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN ) : GPIO_SetBits ( macLED1_GPIO_PORT, macLED1_GPIO_PIN )

#define             macLED2_ON()                            GPIO_ResetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )
#define             macLED2_OFF()                           GPIO_SetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )
#define             macLED2_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED2_GPIO_PORT, macLED2_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN ) : GPIO_SetBits ( macLED2_GPIO_PORT, macLED2_GPIO_PIN )

#define             macLED3_ON()                            GPIO_ResetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )
#define             macLED3_OFF()                           GPIO_SetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )
#define             macLED3_TOGGLE()                        GPIO_ReadOutputDataBit ( macLED3_GPIO_PORT, macLED3_GPIO_PIN ) ? \
                                                            GPIO_ResetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN ) : GPIO_SetBits ( macLED3_GPIO_PORT, macLED3_GPIO_PIN )

#define             macANT_IO0_ON()                         GPIO_SetBits ( GPIOA, GPIO_Pin_0 )
#define             macANT_IO0_OFF()                        GPIO_ResetBits ( GPIOA, GPIO_Pin_0 )
#define             macANT_IO0_TOGGLE()                     GPIO_ReadOutputDataBit ( GPIOA, GPIO_Pin_0 ) ? \
                                                            GPIO_ResetBits ( GPIOA, GPIO_Pin_0 ) : GPIO_SetBits ( GPIOA, GPIO_Pin_0 )

#define             macANT_IO1_ON()                         GPIO_SetBits ( GPIOC, GPIO_Pin_0 )
#define             macANT_IO1_OFF()                        GPIO_ResetBits ( GPIOC, GPIO_Pin_0 )
#define             macANT_IO1_TOGGLE()                     GPIO_ReadOutputDataBit ( GPIOC, GPIO_Pin_0 ) ? \
                                                            GPIO_ResetBits ( GPIOC, GPIO_Pin_0 ) : GPIO_SetBits ( GPIOC, GPIO_Pin_0 )

#define             macANT_IO2_ON()                         GPIO_SetBits ( GPIOC, GPIO_Pin_1 )
#define             macANT_IO2_OFF()                        GPIO_ResetBits ( GPIOC, GPIO_Pin_1 )
#define             macANT_IO2_TOGGLE()                     GPIO_ReadOutputDataBit ( GPIOC, GPIO_Pin_1 ) ? \
                                                            GPIO_ResetBits ( GPIOC, GPIO_Pin_1 ) : GPIO_SetBits ( GPIOC, GPIO_Pin_1 )

#define             macANT_IO3_ON()                         GPIO_SetBits ( GPIOC, GPIO_Pin_2 )
#define             macANT_IO3_OFF()                        GPIO_ResetBits ( GPIOC, GPIO_Pin_2 )
#define             macANT_IO3_TOGGLE()                     GPIO_ReadOutputDataBit ( GPIOC, GPIO_Pin_2 ) ? \
                                                            GPIO_ResetBits ( GPIOC, GPIO_Pin_2 ) : GPIO_SetBits ( GPIOC, GPIO_Pin_2 )

#define             macANT_IO4_ON()                         GPIO_SetBits ( GPIOC, GPIO_Pin_3 )
#define             macANT_IO4_OFF()                        GPIO_ResetBits ( GPIOC, GPIO_Pin_3 )
#define             macANT_IO4_TOGGLE()                     GPIO_ReadOutputDataBit ( GPIOC, GPIO_Pin_3 ) ? \
                                                            GPIO_ResetBits ( GPIOC, GPIO_Pin_3 ) : GPIO_SetBits ( GPIOC, GPIO_Pin_3 )


/************************** PAD 函数声明********************************/
void                               LED_Init                         ( void );
void                               antGPIOInit                      ( void );



#endif /* __LED_H_ */

