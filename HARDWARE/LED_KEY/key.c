#include "key.h"


/*
 * PA0  		--->KEY0
 * PE2  		--->KEY1
 * PE3  		--->KEY2
 * PE4  		--->KEY3
 * KEY_ON		--->0
 * KEY_OFF		--->1
 */
 
/*
 * 功  能：KEY初始化
 * 参  数：无
 * 返回值：无
 */ 
void KEY_Init(void)
{
	//定义结构体变量
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//1、开启 GPIOE GPIOA 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE|RCC_AHB1Periph_GPIOA, ENABLE);

	//2、配置 GPIO
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;    	//输入
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;    	//上拉

	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_0;      	//第 0 个引脚
	GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;      	//第 2 3 4 个引脚
	GPIO_Init(GPIOE, &GPIO_InitStruct);	
}









