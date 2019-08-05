#ifndef _RTC_H
#define _RTC_H 			    

//头文件
#include "stm32f4xx.h"						
							
//函数声明
/*
 * 功  能：RTC初始化  
 * 参  数：无
 * 返回值：无
 */
void Rtc_Init(void);

/*
 * 功  能：设置时间
 * 参  数：
 *				hours:	小时
 *			minutes:	分钟
 *			seconds:	秒
 * 返回值：无
 */
void Set_Time(uint8_t hours, uint8_t minutes, uint8_t seconds);

/*
 * 功  能：设置日期
 * 参  数：
 *			 year:	年
 *			month:	月
 *			 date:	日
 * 返回值：无
 */
void Set_Date(uint8_t year, uint8_t month, uint8_t date);

#endif
