#include "usartx.h"
#include "onenetconnet.h"

extern volatile uint8_t IRQ_OFF;
//USART1_TX A9          USART2_TX A2         USART3_TX B10
//USART1_RX A10         USART2_RX A3         USART3_RX B11


//加入以下代码,支持printf函数,而不需要选择use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//标准库需要的支持函数                 
struct __FILE 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
//定义_sys_exit()以避免使用半主机模式    
_sys_exit(int x) 
{ 
	x = x; 
} 
//重定向fputc函数
//printf的输出，指向fputc，由fputc输出到串口
//这里使用串口1(USART1)输出printf信息
int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0);//等待上一次串口数据发送完成  
	USART1->DR = (u8) ch;      	//写DR,串口1将发送数据
	return ch;
}
#endif 


//void usartx_init(USART_TypeDef * USARTX,u32 Pclk,u32 Bound,u8 Able,u8 Groub, u8 Grab, u8 Sub)
//输入参数 
//USARTX    : USART1 | USART2| USART3   (选择串口)
//Bound     : 1200-                     (设置波特率)

void usartx_init(USART_TypeDef * USARTX,u32 Bound)
{ 
if(USARTX==USART1)
  {
GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1、开启串口1的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
	
	//2、开启串口1相应 GPIO 口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//3、将 PA9 PA10 复用为串口1的特殊功能引脚
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9, GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10, GPIO_AF_USART1);
	
	//4、初始化 PA9 PA10 为复用功能引脚
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_9 | GPIO_Pin_10;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    //以下三行影响不大
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//5、配置以及初始化串口模式
	USART_InitStruct.USART_BaudRate = Bound;  //波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //关闭硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式
	USART_InitStruct.USART_Parity = USART_Parity_No;			//无校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;			//1bit停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//8bit数据位
	USART_Init(USART1, &USART_InitStruct);
	
	//6、开启 串口1 的接收中断  ---》接收数据寄存器不为空
	USART_ITConfig(USART1, USART_IT_RXNE,ENABLE);
	
	//7、初始化 NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART1_IRQn;  //串口1中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART1, ENABLE);	
  }
else if(USARTX==USART2)
  {
   	GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1、开启串口2的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
	
	//2、开启串口2相应 GPIO 口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	
	//3、将 PA2 PA3 复用为串口2的特殊功能引脚
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource2, GPIO_AF_USART2);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource3, GPIO_AF_USART2);
	
	//4、初始化 PA2 PA3 为复用功能引脚
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    //以下三行影响不大
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	
	//5、配置以及初始化串口模式
	USART_InitStruct.USART_BaudRate = Bound;  //波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //关闭硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式
	USART_InitStruct.USART_Parity = USART_Parity_No;			//无校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;			//1bit停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//8bit数据位
	USART_Init(USART2, &USART_InitStruct);
	
	//6、开启 串口2 的接收中断  ---》接收数据寄存器不为空
	USART_ITConfig(USART2, USART_IT_RXNE,ENABLE);
	
	//7、初始化 NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART2_IRQn;  //串口1中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART2, ENABLE);		
 }
else if(USARTX==USART3)
  {
    GPIO_InitTypeDef GPIO_InitStruct;
	USART_InitTypeDef USART_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	//1、开启串口3的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	
	//2、开启串口3相应 GPIO 口时钟
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//3、将 PB10 PB11 复用为串口3的特殊功能引脚
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11, GPIO_AF_USART3);
	
	//4、初始化 PA2 PA3 为复用功能引脚
	GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;    //以下三行影响不大
	GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_UP;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_25MHz;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
	
	//5、配置以及初始化串口模式
	USART_InitStruct.USART_BaudRate = Bound;  //波特率
	USART_InitStruct.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //关闭硬件流控
	USART_InitStruct.USART_Mode = USART_Mode_Rx | USART_Mode_Tx; //接收和发送模式
	USART_InitStruct.USART_Parity = USART_Parity_No;			//无校验
	USART_InitStruct.USART_StopBits = USART_StopBits_1;			//1bit停止位
	USART_InitStruct.USART_WordLength = USART_WordLength_8b;	//8bit数据位
	USART_Init(USART3, &USART_InitStruct);
	
	//6、开启 串口3 的接收中断  ---》接收数据寄存器不为空
	USART_ITConfig(USART3, USART_IT_RXNE,ENABLE);
	
	//7、初始化 NVIC
	NVIC_InitStruct.NVIC_IRQChannel = USART3_IRQn;  //串口1中断
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
	
	USART_Cmd(USART3, ENABLE);	
 }
}



//void usartx_send(USART_TypeDef * USARTX,,u8 *string)
//输入参数 
//USARTX    : USART1 | USART2| USART3   (选择串口)
//buff      :"sadsad"| str              (字符串或数组首地址)
void usartx_send(USART_TypeDef * USARTX,u8 *string)            //发送字符串
{
  USARTX->SR &=~(1<<6);
  while(*string!='\0')
	  {
	    USARTX->DR = *(string++);
	    while(!(USARTX->SR & (1<<6)));
	  }
}

//void uart_putbuff(USART_TypeDef * USARTX,,u8 *buff, u32 len)
//输入参数 
//USARTX    : USART1 | USART2| USART3   (选择串口)
//buff      :"sadsad"| str              (字符串或数组首地址)
//len       :strlen(str)                (字符串或数组长度)
void usartx_puchar(USART_TypeDef * USARTX,u8 *buff, u32 len)    //发送字符串
{  
	USARTX->SR &=~(1<<6);
	while(len--)
	  {
 	    USARTX->DR = *(buff++);
	    while(!(USARTX->SR & (1<<6)));
    }
}




//void USART1_IRQHandler(void)
//{
//	u8 receive=0;
//	if(USART1->SR & (1<<5))
//	{
//		receive = USART1->DR;
//		while(USART1->SR & (1<<5));
//	}
//} 

//void USART2_IRQHandler(void)
//{
//    u8 receive=0;
//    if(USART2->SR & (1<<5))
//		  {
//	      receive = USART2->DR;
//	      while(USART2->SR & (1<<5));
//	    }
//}


/*********************************************************************************/
int mode = 0;
char data;
char usart2_rcv_buf[128];
uint8_t usart2_rcv_len = 0;
char rcv_cmd_start = 0;
char usart2_cmd_buf[128];
uint8_t usart2_cmd_len = 0;
#define MAX_CMD_LEN 128 

void USART3_IRQHandler(void)
{
	//if(IRQ_OFF == 0){
		//IRQ_OFF = 1;
		if(mode == 0)
		{
		   if(USART3->SR & (1<<5))
			{	
			  if(Wifi_cnt >= sizeof(Wifi_receive))Wifi_cnt = 0;      //防止串口被刷爆
				 Wifi_receive[Wifi_cnt] = USART3->DR;                //把串口接收的数据存在数组Wifi_receive
				   if(Wifi_receive[Wifi_cnt]=='E'&&Wifi_cnt>=4)      //用于判断是否断开了onenet，断开了的话接收到close
					   if(strstr((const char *)Wifi_receive, "CLOSE") != NULL)
							 connect_out=1;
					 Wifi_cnt++;
				 while(USART3->SR & (1<<5));
		   } 
		}else if(mode == 1){
			data = USART3->DR;
			usart2_rcv_buf[usart2_rcv_len++]=data;
			
			if(data=='{') //约定平台下发的控制命令以'{'为开始符，‘}’为控制命令结束符，读者可以自定义自己的开始符合结束符
			{
				rcv_cmd_start=1;
			}
			if(rcv_cmd_start==1)
			{
				usart2_cmd_buf[usart2_cmd_len++]=data;
				if((data=='}')||(usart2_cmd_len>=MAX_CMD_LEN-1))
				{
					rcv_cmd_start=0;
					if((NULL != strstr((const char *)usart2_cmd_buf, "LED01")))  //约定平台控制命令"LED1"为打开灯
					{
						LED0(LED_ON);
						//printf("recv cmd LED1 ok\r\n");
					}else if((NULL != strstr((const char *)usart2_cmd_buf, "LED00")))  //约定平台控制命令"LED0"为关闭灯
					{
						LED0(LED_OFF);  
						//printf("recv cmd LED0 ok\r\n");
					}
					
					if((NULL != strstr((const char *)usart2_cmd_buf, "LED11")))  //约定平台控制命令"LED1"为打开灯
					{
						LED1(LED_ON);
						//printf("recv cmd LED1 ok\r\n");
					}else if((NULL != strstr((const char *)usart2_cmd_buf, "LED10")))  //约定平台控制命令"LED0"为关闭灯
					{
						LED1(LED_OFF);  
						//printf("recv cmd LED0 ok\r\n");
					}
					
					if((NULL != strstr((const char *)usart2_cmd_buf, "LED21")))  //约定平台控制命令"LED1"为打开灯
					{
						LED2(LED_ON);
						//printf("recv cmd LED1 ok\r\n");
					}else if((NULL != strstr((const char *)usart2_cmd_buf, "LED20")))  //约定平台控制命令"LED0"为关闭灯
					{
						LED2(LED_OFF);  
						//printf("recv cmd LED0 ok\r\n");
					}
					
					if((NULL != strstr((const char *)usart2_cmd_buf, "LED31")))  //约定平台控制命令"LED1"为打开灯
					{
						LED3(LED_ON);
						//printf("recv cmd LED1 ok\r\n");
					}else if((NULL != strstr((const char *)usart2_cmd_buf, "LED30")))  //约定平台控制命令"LED0"为关闭灯
					{
						LED3(LED_OFF);  
						//printf("recv cmd LED0 ok\r\n");
					}
					memset(usart2_cmd_buf,0,sizeof(usart2_cmd_buf));
					usart2_cmd_len=0;
				}
			} 
		}
		//IRQ_OFF = 0;
	//}
}

