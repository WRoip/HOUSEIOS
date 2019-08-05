#include "rc522_spi.h"

extern void delay_ms(u32);
extern void delay_us(u32);

/*
*	SPI1_SCK 	-->GPIOD6
*	SPI1_MOSI	-->GPIOC6
*	SPI1_MISO	-->GPIOC8
*	F_CS		-->GPIOD7
*	RST			-->GPIOC11
*/

 /*
  * 功  能：初始化SPI
  * 参  数：无
  * 返回值：无
  *
 */
void RC522_Spi_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//开GPIOA、GPIOB时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_PuPd		= GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_6 | GPIO_Pin_11;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Mode		= GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_Pin		= GPIO_Pin_8;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	
}


