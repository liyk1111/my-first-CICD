/*
使用说明：
    1.每一个新项目都需要开启对应的头文件包含，默认全部屏蔽
		2.操作系统默认启用，裸机测试自行屏蔽cmsis_os.h

功能介绍：
    1.STM32Cube软件产生的外设bsp的头文件，默认全部关闭，自行开启

更新记录：
    2023-11-25：
			1.创建调试
*/

#ifndef STM32CUBEBSP_H
#define STM32CUBEBSP_H

//公共头文件
#include "stdint.h"
#include "stdio.h"
//#include "cmsis_os.h"		//FreeRTOS默认启用，若裸机测试需要屏蔽，BSP部分编译错误自行处理一下即可

//STM32外设头文件
//#include "adc.h"
//#include "gpio.h"
//#include "iic.h"
//#include "iis.h"
//#include "iwdg.h"
//#include "rtc.h"
//#include "sdio.h"
//#include "systick.h"
//#include "tim.h"
#include "usart.h"

#endif
