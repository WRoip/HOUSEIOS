#ifndef _LED_H_ 
#define _LED_H_

//头文件
#include "stm32f4xx.h"

//宏定义
#define LED_ON	0
#define LED_OFF	1

#define LED0(a);	if(a) \
						GPIO_SetBits(GPIOF,GPIO_Pin_9); \
					else \
						GPIO_ResetBits(GPIOF,GPIO_Pin_9);
					
#define LED1(a);	if(a) \
						GPIO_SetBits(GPIOF,GPIO_Pin_10); \
					else \
						GPIO_ResetBits(GPIOF,GPIO_Pin_10);
					
#define LED2(a);	if(a) \
						GPIO_SetBits(GPIOE,GPIO_Pin_13); \
					else \
						GPIO_ResetBits(GPIOE,GPIO_Pin_13);
					
#define LED3(a);	if(a) \
						GPIO_SetBits(GPIOE,GPIO_Pin_14); \
					else \
						GPIO_ResetBits(GPIOE,GPIO_Pin_14);
					
#define L0			GPIO_ToggleBits(GPIOF,GPIO_Pin_9);
					
#define L1			GPIO_ToggleBits(GPIOF,GPIO_Pin_10);

#define L2			GPIO_ToggleBits(GPIOE,GPIO_Pin_13);

#define L3			GPIO_ToggleBits(GPIOE,GPIO_Pin_14);

//函数声明
void LED_Init(void);


#endif
