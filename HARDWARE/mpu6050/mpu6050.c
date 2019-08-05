#include "mpu6050.h"
#include "math.h"
/*
 IIC_SDA  	--->PB9  
 IIC_SCL	--->PB8
*/

/*
 * 功  能: IIC_GPIO 初始化
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_GPIO_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//1 开启 GPIOB 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//2 配置以及初始化 GPIO 模式
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;					//输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//推挽
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_9;		//引脚
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;   				//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;				//输出
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//初始为高电平
	GPIO_SetBits(GPIOB ,GPIO_Pin_8 | GPIO_Pin_9);
}

/*
 * 功  能: IIC_SDA 设置输出模式
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_SDAModeOut(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//1 开启 GPIOB 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//2 配置以及初始化 GPIO 模式
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_OUT;					//输出
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;					//推挽
	GPIO_InitStruct.GPIO_Pin   =  GPIO_Pin_9;					//引脚
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;   				//上拉
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;				//输出
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/*
 * 功  能: IIC_SDA 设置输入模式
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_SDAModeIn(void)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	//1 开启 GPIOB 时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	
	//2 配置以及初始化 GPIO 模式
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_IN;					//输入
	GPIO_InitStruct.GPIO_Pin   =  GPIO_Pin_9;					//引脚
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;   				//上拉
	GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/*
 * 功  能: IIC起始信号:当 SCL 为高电平时,SDA由高电平变成低电平
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_Sart(void)
{
	//1 设置为输出模式
	mpu6050_IIC_SDAModeOut();
	
	mpu6050_IIC_SDA_OUT(1);    //SDA = 1
	mpu6050_IIC_SCL_OUT(1);
	delay_us(5);
	
	mpu6050_IIC_SDA_OUT(0);    //SDA = 0
	delay_us(5);
	
	mpu6050_IIC_SCL_OUT(0);    
	delay_us(5);
}

/*
 * 功  能: IIC停止信号:当 SCL 为高电平时,SDA由低电平变成高电平
 * 参  数: 无
 * 返回值: 无
 */
void mpu6050_IIC_Stop(void)
{
	//1 设置为输出模式
	mpu6050_IIC_SDAModeOut();
	
	mpu6050_IIC_SDA_OUT(0);    	//SDA = 0
	mpu6050_IIC_SCL_OUT(0);
	delay_us(5);
	
	mpu6050_IIC_SCL_OUT(1);    	//SCL = 1
	delay_us(5);
	
	mpu6050_IIC_SDA_OUT(1);		//SDA = 1
}




/*
 * 功  能: IIC等待应答函数 : STM32 写数据给 mpu6050  ，mpu6050 给 STM32 一个回复 
 * 参  数: 无
 * 返回值: 读取到的应答信号：应答(0) ,不应答(1)
 */
uint8_t mpu6050_IIC_WaitAck(void)    
{
	uint8_t ack;
	
	//1 设置为输入模式
	mpu6050_IIC_SDAModeIn();
	
	mpu6050_IIC_SCL_OUT(0);    	//SCL = 0    --->mpu6050 进行 应答信号 准备
	delay_us(5);
	
	mpu6050_IIC_SCL_OUT(1);    	//SCL = 1    --->STM32 开始识别 应答信号
	delay_us(5);
	
	if( mpu6050_IIC_SDA_IN() )   //读取到高电平
	{
		ack = 1;
	}else{
		ack = 0;
	}
	
	mpu6050_IIC_SCL_OUT(0);    	//SCL = 0   
	delay_us(5);
	
	
	return ack;
}


/*
 * 功  能: IIC主动应答函数 : STM32 读取 mpu6050 数据 ，STM32 给 mpu6050 一个回复 
 * 参  数: 
 * 		ack		---》STM32 给 mpu6050 的回复：应答(0) ,不应答(1)
 * 返回值: 无
 */
void mpu6050_IIC_Ack(uint8_t ack)    
{
	
	//1 设置为输出模式
	mpu6050_IIC_SDAModeOut();
	
	mpu6050_IIC_SCL_OUT(0);    	//SCL = 0    --->STM32 此时进行数据准备
	delay_us(5);
	
	if(ack)   //不应答
	{
		mpu6050_IIC_SDA_OUT(1);
	}else{
		mpu6050_IIC_SDA_OUT(0);
	}
	
	mpu6050_IIC_SCL_OUT(1);    	//SCL = 1    --->mpu6050 此时开始识别数据
	delay_us(5);
	
	mpu6050_IIC_SCL_OUT(0);    	//SCL = 0
	delay_us(5);
}

/*
 * 功  能: IIC读一个字节函数 : STM32 读取 mpu6050 数据 
 * 参  数: 无
 * 返回值: 你读取的一个字节数据
 */
uint8_t mpu6050_IIC_ReadByte(void)
{
	uint8_t i;
	uint8_t data = 0;  //0000 0000 
	
	//1 设置为输入模式
	mpu6050_IIC_SDAModeIn();
	
	mpu6050_IIC_SCL_OUT(0);    	//SCL = 0    --->mpu6050 此时进行数据准备
	delay_us(5);
	
	for(i=0 ;i<8 ;i++)
	{
		//将 SCL 拉高表示 STM32 开始读取数据
		mpu6050_IIC_SCL_OUT(1);    	
		delay_us(5);
		
		//读取一位数据
		if( mpu6050_IIC_SDA_IN() )   //读取到高电平
		{
			data |= (0x1<<(7-i));
		}else{
			data &= ~(0x1<<(7-i));
		}
		
		mpu6050_IIC_SCL_OUT(0);    	//SCL = 0    --->mpu6050 此时进行交换下一位数据
		delay_us(5);	
	}
	
	return data;
}

/*
 * 功  能: IIC写一个字节函数 : STM32 写数据给 mpu6050
 * 参  数: 
 * 		data	---》你要写入的一个字节数据
 * 返回值: 无
 */
void mpu6050_IIC_WriteByte(uint8_t data)    //10101011
{
	uint8_t i;
	
	//1 设置为输出模式
	mpu6050_IIC_SDAModeOut();
	
	mpu6050_IIC_SCL_OUT(0);    	//SCL = 0    --->STM32 进行数据准备
	delay_us(5);
	
	for(i=0 ;i<8 ;i++)
	{
		//开始准备第  7-i 位数据
		if( data & (0x1<<(7-i)) )  //10000000
		{
			mpu6050_IIC_SDA_OUT(1);
		}else{
			mpu6050_IIC_SDA_OUT(0);
		}
		
		mpu6050_IIC_SCL_OUT(1);    	//SCL = 1    --->mpu6050 开始读取数据
		delay_us(5);
		
		mpu6050_IIC_SCL_OUT(0);    	//SCL = 0    --->STM32 进行下一位数据交换
		delay_us(5);	
	}
}


// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： MPU6050 写寄存器函数 
* 输入参数： regAddr：寄存器地址 regData：待写入寄存器值 
* 输出参数： 无 
* 返回值 ： 无 
******************************************************************************/
void MPU6050_Write_Reg(uint8_t regAddr, uint8_t regData)
{
    /* 发送起始信号 */
    mpu6050_IIC_Sart();
    
    /* 发送设备地址 */        
    mpu6050_IIC_WriteByte(DEV_ADDR); 
    if (mpu6050_IIC_WaitAck())
        goto stop;
    
    /* 发送寄存器地址 */
    mpu6050_IIC_WriteByte(regAddr);
    if (mpu6050_IIC_WaitAck())
        goto stop;
    
    /* 写数据到寄存器 */
    mpu6050_IIC_WriteByte(regData);
    if (mpu6050_IIC_WaitAck())
        goto stop;
    
stop:
    mpu6050_IIC_Stop();
}

// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： MPU6050 读寄存器函数 
* 输入参数： regAddr：寄存器地址 
* 输出参数： 无 
* 返回值 ： regData：读出的寄存器数据 
******************************************************************************/
uint8_t MPU6050_Read_Reg(uint8_t regAddr)
{
    uint8_t regData;
    
    /* 发送起始信号 */
    mpu6050_IIC_Sart();
    
    /* 发送设备地址 */        
    mpu6050_IIC_WriteByte(DEV_ADDR);
    if (mpu6050_IIC_WaitAck())
        goto stop;
    
    /* 发送寄存器地址 */
    mpu6050_IIC_WriteByte(regAddr);
    if (mpu6050_IIC_WaitAck())
        goto stop;
    
    /* 发送重复起始信号 */
    mpu6050_IIC_Sart();
    
    /* 发送读模式设备地址 */	
    mpu6050_IIC_WriteByte(DEV_ADDR | 0x01);
    if (mpu6050_IIC_WaitAck())
        goto stop;
    
    /* 读寄存器数据 */
    regData = mpu6050_IIC_ReadByte();
    mpu6050_IIC_Ack(1); // 非应答信号     
stop:
    mpu6050_IIC_Stop();
    
    return regData;
}

// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： MPU6050 初始化函数 
* 输入参数： 无 
* 输出参数： 无 
* 返回值 ： 无 
* 备 注： 配置 MPU6050 测量范围：± 2000 °/s ± 2g 
******************************************************************************/
void MPU6050_Init(void)
{
    mpu6050_IIC_GPIO_Init();   // I2C 初始化     

    MPU6050_Write_Reg(PWR_MGMT_1, 0x00);    //解除休眠状态     
    MPU6050_Write_Reg(SMPLRT_DIV, 0x07);    //陀螺仪采样率，典型值：0x07(125Hz)     
    MPU6050_Write_Reg(CONFIG, 0x06);        //低通滤波频率，典型值：0x06(5Hz)     
    MPU6050_Write_Reg(GYRO_CONFIG, 0x18);   //陀螺仪自检及测量范围，典型值：0x18(不自检，2000deg/s)     
    MPU6050_Write_Reg(ACCEL_CONFIG, 0x01);  //加速计自检、测量范围及高通滤波频率，典型值：0x01(不自检，2G，5Hz) 
}

// 【基础】基本数据读取\USER\src\mpu6050.c 
/****************************************************************************** 
* 函数介绍： 连续读两个寄存器并合成 16 位数据 
* 输入参数： regAddr：数据低位寄存器地址 
* 输出参数： 无 
* 返回值 ： data：2 个寄存器合成的 16 位数据 
******************************************************************************/
int16_t MPU6050_Get_Data(uint8_t regAddr)
{
    return (MPU6050_Read_Reg(regAddr) << 8) | MPU6050_Read_Reg(regAddr + 1);  // 合成数据 
}

// 基本数据读取\USER\src\main.c 
/* 传感器数据修正值（消除芯片固定误差，根据硬件进行调整） */
#define X_ACCEL_OFFSET -42		//-600 
#define Y_ACCEL_OFFSET -20		//-100 
#define Z_ACCEL_OFFSET -16054	//2900 - 19000 
#define X_GYRO_OFFSET 6			//32 
#define Y_GYRO_OFFSET -17		//-11 
#define Z_GYRO_OFFSET -1		//1 
extern int printf(const char*,...);
/****************************************************************************** 
* 函数介绍： 串口打印 6050 传感器读取的三轴加速度、陀螺仪及温度数据 
* 输入参数： 无 
* 输出参数： 无 
* 返回值 ： 无 
******************************************************************************/
void MPU6050_Display(void)
{
    /* 打印 x, y, z 轴加速度 */
    printf("ACCEL_X: %d\t", MPU6050_Get_Data(ACCEL_XOUT_H) + X_ACCEL_OFFSET);
    printf("ACCEL_Y: %d\t", MPU6050_Get_Data(ACCEL_YOUT_H) + Y_ACCEL_OFFSET);
    printf("ACCEL_Z: %d\t", MPU6050_Get_Data(ACCEL_ZOUT_H) + Z_ACCEL_OFFSET);
    
    /* 打印温度 */
    printf("TEMP: %0.2f\t", MPU6050_Get_Data(TEMP_OUT_H) / 340.0 + 36.53);
    
    /* 打印 x, y, z 轴角速度 */
    printf("GYRO_X: %d\t", MPU6050_Get_Data(GYRO_XOUT_H) + X_GYRO_OFFSET);
    printf("GYRO_Y: %d\t", MPU6050_Get_Data(GYRO_YOUT_H) + Y_GYRO_OFFSET);
    printf("GYRO_Z: %d\t", MPU6050_Get_Data(GYRO_ZOUT_H) + Z_GYRO_OFFSET);
    
    printf("\r\n");
}

/****************************************************************************** 
* 函数介绍： 获取 6050 传感器读取的三轴加速度、陀螺仪及温度数据 
* 输入参数： data_MP-->保存三轴加速度、角速度
* 返回值 ： 温度数据
******************************************************************************/
double MPU6050_Get(int * data_MP)
{
	 /* 获取 x, y, z 轴加速度 */
	data_MP[0] = (MPU6050_Read_Reg(ACCEL_XOUT_H) << 8) | MPU6050_Read_Reg(ACCEL_XOUT_H + 1) + X_ACCEL_OFFSET;// MPU6050_Get_Data(ACCEL_XOUT_H) + X_ACCEL_OFFSET;
    data_MP[1] = (MPU6050_Read_Reg(ACCEL_YOUT_H) << 8) | MPU6050_Read_Reg(ACCEL_YOUT_H + 1) + Y_ACCEL_OFFSET;
    data_MP[2] = (MPU6050_Read_Reg(ACCEL_ZOUT_H) << 8) | MPU6050_Read_Reg(ACCEL_ZOUT_H + 1) + Z_ACCEL_OFFSET;
    
    /* 获取 x, y, z 轴角速度 */
    data_MP[3] = (MPU6050_Read_Reg(GYRO_XOUT_H) << 8) | MPU6050_Read_Reg(GYRO_XOUT_H + 1) + X_GYRO_OFFSET;//MPU6050_Get_Data(GYRO_XOUT_H) + X_GYRO_OFFSET;
    data_MP[4] = (MPU6050_Read_Reg(GYRO_YOUT_H) << 8) | MPU6050_Read_Reg(GYRO_YOUT_H + 1) + Y_GYRO_OFFSET;
    data_MP[5] = (MPU6050_Read_Reg(GYRO_ZOUT_H) << 8) | MPU6050_Read_Reg(GYRO_ZOUT_H + 1) + Z_GYRO_OFFSET;
	
	return ( ( (MPU6050_Read_Reg(TEMP_OUT_H) << 8) | MPU6050_Read_Reg(TEMP_OUT_H + 1) ) / 340.0 + 36.53);
}


/****************************************************************************** 
* 函数介绍： 计算 x, y, z 三轴的倾角 
* 输入参数： 无 
* 输出参数： data：角度结构体 
* 返回值 ： 无 
******************************************************************************/

void MPU6050_Get_Angle(MPU6050_Angle *data)
{   
    /* 计算x, y, z 轴倾角，返回弧度值*/
    data->X_Angle = acos((MPU6050_Get_Data(ACCEL_XOUT_H) + X_ACCEL_OFFSET) / 16384.0);
    data->Y_Angle = acos((MPU6050_Get_Data(ACCEL_YOUT_H) + Y_ACCEL_OFFSET) / 16384.0);
    data->Z_Angle = acos((MPU6050_Get_Data(ACCEL_ZOUT_H) + Z_ACCEL_OFFSET) / 16384.0);

    /* 弧度值转换为角度值 */
    data->X_Angle = data->X_Angle * 57.29577;
    data->Y_Angle = data->Y_Angle * 57.29577;
    data->Z_Angle = data->Z_Angle * 57.29577;
} 
