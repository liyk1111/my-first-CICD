/*
使用说明：
    1.根据项目需求，自行启用哪个串口并更改串口定义的数组长度；
		2.由于F407开发板的串口4引脚PA0和PA1有异常，所以测试失败
		3.其他几个串口都已经测试过正常接收和发送，但公共接口可能会存在并发问题，需要留意。
		4.中断处理虚函数需要重新实现，处理后需要清零接收的数据长度，否则会一直累加
		5.需要包含retarget.c才能使用printf

功能介绍：
    1.串口收发，非DMA
		2.中断处理过程：
			当串口接收完成产生中断时，HAL库会调用HAL_UARTEx_RxEventCallback虚函数，该虚函数已在源文件中重新实现；
			根据串口编号执行不同的虚函数，此虚函数实现应用业务内容，例如释放某个信号量等。
		3.初始化函数会马上启动串口功能，无需自行开启
		4.接收使用空闲中断回调函数
		5.发送使用各自的Usart(x)_Send_Data
		6.低功耗串口没有测试

更新记录：
    2023-11-25：
			1.详细测试并优化隔离HAL库，使用枚举和使能操作。
*/

#ifndef USARTX_H
#define USARTX_H

#include "STM32CubeBSP.h"
#include "ADBSPPublicConfig.h"

//#define USARTDELAY osDelay			//TODO:这里没有系统时需要优化
#define DEFAULT_BAUD		115200


//串口配置
#define ENLPUSART1 	0
#define ENUSART1 	1
#define ENUSART2 	0
#define ENUSART3 	0
#define ENUSART4 	0
#define ENUSART5 	0
#define ENUSART6 	0
#define Usart1_Rx_Buff_Len		100
#define Usart2_Rx_Buff_Len		500
#define	Usart3_Rx_Buff_Len		100
#define	Usart4_Rx_Buff_Len		100
#define	Usart5_Rx_Buff_Len		100
#define Usart6_Rx_Buff_Len		100

//公用接口
uint8_t UsartInit(ADUartIndex UsartIndex, uint32_t tmpBaud);	//初始化
uint8_t UsartDeInit(ADUartIndex UsartIndex);									//释放初始化
uint32_t UsartReceiveStart(ADUartIndex UsartIndex);						//开启串口接收
uint32_t UsartGetState(ADUartIndex UsartIndex);								//获取串口状态
void UsartClearRxLenght(ADUartIndex UsartIndex);							//清空接收长度

//不同串口的独立接口
#if ENLPUSART1 == 1	//	没有处理
	//低功耗串口1
	#define LPUsart1_Rx_Max_Size 	1000
	extern uint8_t LPUsart1_Rx_Buff[LPUsart1_Rx_Max_Size];
	extern uint16_t LPUsart1_Rx_Len ;
	//初始化
	void LPUsart1Init(void);
	//
	void LPUsart1DeInit(void);
	//开启串口接收
	uint32_t LPUsart1ReceiveStart(void);
	//获取串口状态
	uint32_t LPUsart1GetState(void);
	//低功耗串口1发送
	uint8_t LPUsart1_Send_Data(uint8_t *data, uint16_t len);
	uint8_t LPUsart1Printf(char* fmt, ...);
#endif 

#if ENUSART1 == 1
	//串口1
	extern uint8_t Usart1_Rx_Buff[Usart1_Rx_Buff_Len];
	extern uint16_t Usart1_Rx_Len ;
	
	//串口1发送
	uint8_t Usart1_Send_Data(uint8_t *data, uint16_t len);
	//串口1打印
	uint8_t Usart1Printf(char* fmt, ...);
#endif

#if ENUSART2 == 1
	extern uint8_t Usart2_Rx_Buff[Usart2_Rx_Buff_Len];
	extern uint16_t Usart2_Rx_Len ;
	//串口2发送
	uint8_t Usart2_Send_Data(uint8_t *data, uint16_t len);
	//串口2打印
	uint8_t Usart2Printf(char* fmt, ...);
#endif

#if ENUSART3 == 1
	extern uint8_t Usart3_Rx_Buff[Usart3_Rx_Buff_Len];
	extern uint16_t Usart3_Rx_Len ;
	//串口3发送
	uint8_t Usart3_Send_Data(uint8_t *data, uint16_t len);
	//串口3打印
	uint8_t Usart3Printf(char* fmt, ...);
#endif

#if ENUSART4 == 1
	extern uint8_t Usart4_Rx_Buff[Usart4_Rx_Buff_Len];
	extern uint16_t Usart4_Rx_Len ;
	//串口4发送
	uint8_t Usart4_Send_Data(uint8_t *data, uint16_t len);
	//串口4打印
	uint8_t Usart4Printf(char* fmt, ...);
#endif

#if ENUSART5 == 1
	extern uint8_t Usart5_Rx_Buff[Usart5_Rx_Buff_Len];
	extern uint16_t Usart5_Rx_Len ;
	//串口5发送
	uint8_t Usart5_Send_Data(uint8_t *data, uint16_t len);
	//串口5打印
	uint8_t Usart5Printf(char* fmt, ...);
#endif

#if ENUSART6 == 1
	extern uint8_t Usart6_Rx_Buff[Usart6_Rx_Buff_Len];
	extern uint16_t Usart6_Rx_Len ;
	//串口6发送
	uint8_t Usart6_Send_Data(uint8_t *data, uint16_t len);
	//串口6打印
	uint8_t Usart6Printf(char* fmt, ...);
#endif

#endif

