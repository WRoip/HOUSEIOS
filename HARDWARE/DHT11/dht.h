#ifndef _DHT_H_
#define _DHT_H_

/************************* 头文件 **************************/
#include "stm32f4xx.h"

/************************ 变量声明 *************************/

/************************* 宏定义 **************************/
#define DHT11_OUT()	GPIO_InitStruct_DHT.GPIO_Mode = GPIO_Mode_OUT;\
					GPIO_InitStruct_DHT.GPIO_Pin = GPIO_Pin_9;\
					GPIO_Init(GPIOG, &GPIO_InitStruct_DHT);

#define DHT11_IN()	GPIO_InitStruct_DHT.GPIO_Mode = GPIO_Mode_IN;\
					GPIO_InitStruct_DHT.GPIO_Pin = GPIO_Pin_9;\
					GPIO_Init(GPIOG, &GPIO_InitStruct_DHT);
										
#define SetGPIOG(a) a?GPIO_SetBits(GPIOG, GPIO_Pin_9):GPIO_ResetBits(GPIOG, GPIO_Pin_9)

/************************ 函数声明 *************************/
 /*
  * 功  能：DHT初始化
  * 参  数：void
  * 返回值：void
  *
 */
void DHT11_Init(void);

 /*
  * 功  能：读DHT11的数据
  * 参  数：char *c
  * 返回值：int		 0	成功
	*								-1	失败,响应超时
	*								-2	失败,接收数据出错
	*								-3	失败,检验出错
  *
 */
int Read_DHT11(char *c);

 /*
  * 功  能：接收DHT11的40bit数据到ch数组
  * 参  数：char *c
  * 返回值：int
  *
 */
int Read_Data(char *ch);

#endif
