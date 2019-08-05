#ifndef _TIM_H
#define _TIM_H 			    

/********************** 头文件 ***********************/
#include "stm32f4xx.h"
#include "string.h" 
#include "key.h"
#include "led.h"

/********************** 宏定义 ***********************/
#define key1 162
#define key2 98
#define key3 226
#define key4 34
#define key5 2
#define key6 194
#define key7 224
#define key8 168
#define key9 144
#define keyX 104
#define key0 152
#define keyQ 176
#define keyU 24
#define keyL 16
#define keyOK 56
#define keyR 90
#define keyD 74
#define base_num 32

/********************** 任务结构 ***********************/
#define Angle_T	5
#define ACCEL_T	10
#define GYRO_T	10

typedef struct _TASK_COMPONENTS
{
    uint8_t Run;                 	// 程序运行标记：0-不运行，1运行
    uint16_t Timer;              	// 计时器
    uint16_t ItvTime;              	// 任务运行间隔时间
    void (*TaskHook)(void);    		// 要运行的任务函数
}TASK_COMPONENTS;       			// 任务定义

/********************** 函数声明 ***********************/
/*
 * 功  能：定时器3初始化
 * 参  数：无
 * 返回值：无
 */
void TIM3_Init(uint16_t Period ,uint16_t Prescaler);
/**************************************************************************************
* FunctionName   : TaskProcess()
* Description    : 任务处理
* EntryParameter : None
* ReturnValue    : None
**************************************************************************************/
void TaskProcess(void);

/*
 * 功  能：欢迎页
 * 参  数：
 *				name:作者名字
 *				role:功能说明
 * 返回值：无
 */
void Login(char name[], char role[]);

/*
 * 功  能：修改或初始化AT24C02
 * 参  数：
 *				首次进入：choose == 1
 *				修改参数：choose == 0
 * 返回值：无
 */
void Modify_TH(uint8_t choose);
	
#endif
