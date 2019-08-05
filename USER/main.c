#include "stm32f4xx.h"
#include "systick.h"
#include "usartx.h"
#include "tim.h"
#include "onenetconnet.h"
#include "dht.h"
#include "infrared.h"
#include "rc522.h"
#include "mpu6050.h"
#include "oled.h"
#include "chinese.h"
#include "oledfont.h"
#include "rtc.h"
#include "key.h"
#include "led.h"
#include "at24c02.h"
#include "At24c02_Read.h"

extern int mode;

const char* DEVID_T = DEVID;
const char* APIKEY_T = APIKEY;

int main(void)
{
	/**************************** 硬件初始化 *******************************/
	//EXTI_DeInit();
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3);
	Systick_Init();
	
	
	usartx_init(USART1,115200);             //初始化串口1，不开启接收中断，主要用做发送数据串口显示调试
	usartx_init(USART3,115200);             //初始化串口3，开启接收中断，设置中断向量表分组和优先级 
	TIM3_Init(30000,84);
	
	ESP8266_init();							//初始化ESP8266模块，连接WIFI,接入平台IP
	OneNet_DevLink(DEVID,APIKEY);			//接入平台设备。
	
	DHT11_Init();
	Infrared_Init();
	RC522_Init();
	OLed_Init();
	Rtc_Init();
	KEY_Init();
	LED_Init();
	AT24C02_Init();
	mode = 1;								//串口3接收进入接收命令模式
	printf("OneNet_DevLink Success!\r\n");
	
	MPU6050_Init();	
	
	/*********************** 从AT24C02获取温度、湿度比较值 ****************************/
	Modify_TH(1);
	
	printf("HardWare Init Success!\r\n");

	
	OLED_ClearLogin();
	Login(author, role);					//欢迎页，author作者名；role功能说明
	delay_s(2);								//2,延时2s
	
	/****************************** 实现的功能 ************************************/
	while(1)
	{
		TaskProcess();
	}
}
