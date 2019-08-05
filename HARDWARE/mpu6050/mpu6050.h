#ifndef _MPU6050_H 
#define _MPU6050_H

//头文件
#include "stm32f4xx.h"
#include "systick.h"

//=======================================================
// 【基础】基本数据读取\USER\src\mpu6050.h 

#define DEV_ADDR 0xD0 // 6050 器件地址 
//----------------------------------------- 
// 定义MPU6050内部地址 
//----------------------------------------- 
#define SMPLRT_DIV 0x19 //陀螺仪采样率，典型值：0x07(125Hz) 
#define CONFIG 0x1A //低通滤波频率，典型值：0x06(5Hz) 
#define GYRO_CONFIG 0x1B //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s) 
#define ACCEL_CONFIG 0x1C //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz) 

/* 加速度相关寄存器地址 */
#define ACCEL_XOUT_H 0x3B 
#define ACCEL_XOUT_L 0x3C 
#define ACCEL_YOUT_H 0x3D 
#define ACCEL_YOUT_L 0x3E 
#define ACCEL_ZOUT_H 0x3F 
#define ACCEL_ZOUT_L 0x40 

/* 温度相关寄存器地址 */
#define TEMP_OUT_H 0x41 
#define TEMP_OUT_L 0x42 

/* 陀螺仪相关寄存器地址 */
#define GYRO_XOUT_H 0x43
#define GYRO_XOUT_L 0x44 
#define GYRO_YOUT_H 0x45 
#define GYRO_YOUT_L 0x46 
#define GYRO_ZOUT_H 0x47 
#define GYRO_ZOUT_L 0x48 

#define PWR_MGMT_1 0x6B  //电源管理，典型值：0x00(正常启用) 
#define WHO_AM_I 0x75 //IIC地址寄存器(默认数值0x68，只读) 
#define SlaveAddress 0xD0 //IIC写入时的地址字节数据，+1为读取 

#define mpu6050_IIC_SCL_OUT(a);		if(a) 	\
						( GPIOB->BSRRL |= (0x1 << 8) ); \
					else 	\
						( GPIOB->BSRRH |= (0x1 << 8) );	
#define mpu6050_IIC_SDA_OUT(a);		if(a) 	\
						GPIO_SetBits(GPIOB, GPIO_Pin_9); \
					else 	\
						GPIO_ResetBits(GPIOB, GPIO_Pin_9);	
	
#define mpu6050_IIC_SDA_IN()		GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_9)

//【扩展】坐标倾角计算\USER\src\main.c 
/* 坐标角度结构体 */
typedef struct Angle
{
    double X_Angle;
    double Y_Angle;
    double Z_Angle;
    
} MPU6050_Angle;
					
//函数声明
/*
 * 功  能: IIC_GPIO 初始化
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_GPIO_Init(void);
/*
 * 功  能: IIC_SDA 设置输出模式
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_SDAModeOut(void);
/*
 * 功  能: IIC_SDA 设置输入模式
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_SDAModeIn(void);
/*
 * 功  能: IIC起始信号:当 SCL 为高电平时,SDA由高电平变成低电平
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_Sart(void);
/*
 * 功  能: IIC停止信号:当 SCL 为高电平时,SDA由低电平变成高电平
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_Stop(void);
/*
 * 功  能: IIC读一个字节函数 : STM32 读取 mpu6050 数据 
 * 参  数: 无
 * 返回值: 你读取的一个字节数据
 */
uint8_t mpu6050_IIC_ReadByte(void);
/*
 * 功  能: IIC写一个字节函数 : STM32 写数据给 mpu6050
 * 参  数: 
 * 		data	---》你要写入的一个字节数据
 * 返回值: 无
 */
void mpu6050_IIC_WriteByte(uint8_t data);
/*
 * 功  能: IIC等待应答函数 : STM32 写数据给 mpu6050  ，mpu6050 给 STM32 一个回复 
 * 参  数: 无
 * 返回值: 读取到的应答信号：应答(0) ,不应答(1)
 */
uint8_t mpu6050_IIC_WaitAck(void);
/*
 * 功  能: IIC主动应答函数 : STM32 读取 mpu6050 数据 ，STM32 给 mpu6050 一个回复 
 * 参  数: 
 * 		ack		---》STM32 给 mpu6050 的回复：应答(0) ,不应答(1)
 * 返回值: 无
 */
void mpu6050_IIC_Ack(uint8_t ack);

// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： MPU6050 写寄存器函数 
* 输入参数： regAddr：寄存器地址 regData：待写入寄存器值 
* 输出参数： 无 
* 返回值 ： 无 
******************************************************************************/
void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData);

// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： MPU6050 读寄存器函数 
* 输入参数： regAddr：寄存器地址 
* 输出参数： 无 
* 返回值 ： regData：读出的寄存器数据 
******************************************************************************/
uint8_t MPU6050_Read_Reg(uint8_t regAddr);

// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： MPU6050 初始化函数 
* 输入参数： 无 
* 输出参数： 无 
* 返回值 ： 无 
* 备 注： 配置 MPU6050 测量范围：± 2000 °/s ± 2g 
******************************************************************************/
void MPU6050_Init(void);

// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： 连续读两个寄存器并合成 16 位数据 
* 输入参数： regAddr：数据低位寄存器地址 
* 输出参数： 无 
* 返回值 ： data：2 个寄存器合成的 16 位数据 
******************************************************************************/
int16_t MPU6050_Get_Data(uint8_t regAddr);

/****************************************************************************** 
* 函数介绍： 串口打印 6050 传感器读取的三轴加速度、陀螺仪及温度数据 
* 输入参数： 无 
* 输出参数： 无 
* 返回值 ： 无 
******************************************************************************/
void MPU6050_Display(void);

/****************************************************************************** 
* 函数介绍： 获取 6050 传感器读取的三轴加速度、陀螺仪及温度数据 
* 输入参数： data_MP-->保存三轴加速度、角速度
* 返回值 ： 温度数据
******************************************************************************/
double MPU6050_Get(int * data_MP);

/****************************************************************************** 
* 函数介绍： 计算 x, y, z 三轴的倾角 
* 输入参数： 无 
* 输出参数： data：角度结构体 
* 返回值 ： 无 
******************************************************************************/
void MPU6050_Get_Angle(MPU6050_Angle *data);


#endif
