#ifndef _RC522_SPI_H_
#define _RC522_SPI_H_

/************************* 头文件 **************************/
#include "stm32f4xx.h"

/************************* 宏定义 **************************/

#define Set_SCK(a)		(a) ? (GPIO_SetBits(GPIOD ,GPIO_Pin_6)) : (GPIO_ResetBits(GPIOD ,GPIO_Pin_6))

#define Set_MOSI(a)		(a)	?	(GPIO_SetBits(GPIOC ,GPIO_Pin_6)) : (GPIO_ResetBits(GPIOC ,GPIO_Pin_6))

#define Set_CS(a)		(a)	? (GPIO_SetBits(GPIOD ,GPIO_Pin_7)) : (GPIO_ResetBits(GPIOD ,GPIO_Pin_7))

#define Set_RST(a)			(a)	? (GPIO_SetBits(GPIOC ,GPIO_Pin_11)) : (GPIO_ResetBits(GPIOC ,GPIO_Pin_11))

#define Red_MISO()	GPIO_ReadInputDataBit(GPIOC ,GPIO_Pin_8)

/************************ 函数声明 *************************/
  /*
  * 功  能：初始化SPI
  * 参  数：无
  * 返回值：无
  *
 */
void RC522_Spi_Init(void);

#endif
