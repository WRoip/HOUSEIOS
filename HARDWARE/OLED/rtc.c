#include "rtc.h"

RTC_InitTypeDef RTC_InitStruct_RTC;
RTC_DateTypeDef RTC_DataStructure_RTC;
RTC_TimeTypeDef RTC_TimeStructure_RTC;

/*
 * 功  能：RTC初始化  
 * 参  数：无
 * 返回值：无
 */
void Rtc_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR,ENABLE);
	PWR_BackupAccessCmd(ENABLE);/*使能后备寄存器访问*/
	
	if(RTC_ReadBackupRegister(RTC_BKP_DR0) != 0xA0A0)
	{

		RCC_LSEConfig(RCC_LSE_ON);/*使能LSE振荡器*/
		while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)/*等待就绪*/
		{
		}
		RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);/*选择RTC时钟源*/
		RCC_RTCCLKCmd(ENABLE);/*使能RTC时钟*/
		RTC_WaitForSynchro();/*等待RTC APB寄存器同步*/
		/*配置RTC数据寄存器和分频器*/
		RTC_InitStruct_RTC.RTC_AsynchPrediv	= 0x7f;
		RTC_InitStruct_RTC.RTC_HourFormat		= RTC_HourFormat_24;
		RTC_InitStruct_RTC.RTC_SynchPrediv	= 0xff;
		RTC_Init(&RTC_InitStruct_RTC);
		/*设置年月日和星期*/
		RTC_DataStructure_RTC.RTC_Year = 0x19;
		RTC_DataStructure_RTC.RTC_Month = 0x7;
		RTC_DataStructure_RTC.RTC_Date = 0x29;
		RTC_DataStructure_RTC.RTC_WeekDay = 0x01;
		RTC_SetDate(RTC_Format_BCD,&RTC_DataStructure_RTC);
		/*设置时分秒 及显示格式*/
		RTC_TimeStructure_RTC.RTC_H12 = RTC_H12_PM;
		RTC_TimeStructure_RTC.RTC_Hours = 0x14;
		RTC_TimeStructure_RTC.RTC_Minutes = 0x52;
		RTC_TimeStructure_RTC.RTC_Seconds = 0x00;
		RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure_RTC);
		/*配置备份寄存器，表示已经设置过RTC*/
		RTC_WriteBackupRegister(RTC_BKP_DR0,0xA0A0);
	}
	


}

/*
 * 功  能：设置时间
 * 参  数：
 *		hours:		小时
 *		minutes:	分钟
 *		seconds:	秒
 * 返回值：无
 */
void Set_Time(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
	hours = ((hours / 10) << 4) | (hours % 10);
	minutes = ((minutes / 10) << 4) | (minutes % 10);
	seconds = ((seconds / 10) << 4) | (seconds % 10);
	RTC_TimeStructure_RTC.RTC_Hours		= hours;
	RTC_TimeStructure_RTC.RTC_Minutes	= minutes;
	RTC_TimeStructure_RTC.RTC_Seconds	= seconds;
	RTC_SetTime(RTC_Format_BCD,&RTC_TimeStructure_RTC);
}

/*
 * 功  能：设置日期
 * 参  数：
 *			 year:	年
 *			month:	月
 *			 date:	日
 * 返回值：无
 */
void Set_Date(uint8_t year, uint8_t month, uint8_t date)
{
	month = ((month / 10) << 4) | (month % 10);
	date = ((date / 10) << 4) | (date % 10);
	RTC_DataStructure_RTC.RTC_Year	= year;
	RTC_DataStructure_RTC.RTC_Month	= month;
	RTC_DataStructure_RTC.RTC_Date	= date;
	RTC_SetDate(RTC_Format_BCD,&RTC_DataStructure_RTC);
}





