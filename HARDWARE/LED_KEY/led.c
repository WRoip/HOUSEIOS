#include "led.h"


/*
 * PF9  		--->LED0
 * PF10  		--->LED1
 * PE13  		--->LED2
 * PE14  		--->LED3
 * LED_ON		--->0
 * LED_OFF		--->1
 */
 
/*
 * 功  能：LED初始化
 * 参  数：无
 * 返回值：无
 */
void LED_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//1、开启 GPIOE GPIOF 时钟		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOF, ENABLE);

	//2、配置 GPIO
	GPIO_InitStruct.GPIO_Mode  =GPIO_Mode_OUT;    	//输出模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;   	//推挽
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;    	//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz; 	//输出速度

	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9|GPIO_Pin_10;      	//第 9 10 个引脚
	GPIO_Init(GPIOF, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_13|GPIO_Pin_14;      	//第 13 14 个引脚
	GPIO_Init(GPIOE, &GPIO_InitStruct);	
	

	//初始灯灭
	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
	GPIO_SetBits(GPIOE,GPIO_Pin_13|GPIO_Pin_14);
}





