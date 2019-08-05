#ifndef __USARTX_H
#define __USARTX_H
#include "stm32f4xx.h"
#include <stdio.h>
#include "led.h"

/* 
USART1_TX A9        USART2_TX A2       USART3_TX B10
USART1_RX A10       USART2_RX A3       USART3_RX B11
*/
void usartx_init(USART_TypeDef * USARTX,u32 Bound);

void usartx_send(USART_TypeDef * USARTX,u8 *string);       //·¢ËÍ×Ö·û´®

void usartx_puchar(USART_TypeDef * USARTX,u8 *buff, u32 len);

int fputc(int ch, FILE *f);




#endif	 


