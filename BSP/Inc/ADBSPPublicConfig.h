/*
使用说明：
    1.APP必须包含

功能介绍：
		1.枚举了所有需要隔离HAL库的外设

更新记录：
    2023-11-25：
			1.创建，调试
*/

#ifndef ADBSPPUBLICCONFIG_H
#define ADBSPPUBLICCONFIG_H


//串口个数枚举
typedef enum
{
  ADUart1 = 1,
  ADUart2,
	ADUart3,
	ADUart4,
	ADUart5,
	ADUart6
}ADUartIndex;

//定时器枚举枚举
typedef enum
{
  ADTimer1 = 1,
	ADTimer2,
	ADTimer3,
	ADTimer4,
	ADTimer5,
	ADTimer6,
	ADTimer7,
	ADTimer8,
	ADTimer9,
	ADTimer10,
	ADTimer11,
	ADTimer12,
	ADTimer13,
	ADTimer14
}ADTimerIndex;






#endif
