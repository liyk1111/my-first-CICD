/******************************************************
* Program Assignment :      
* Author:                   liyk
* Date:                     2023/7/10   创建
* Description:              宏定义:
                    		1.引脚配置
                    		2.状态定义
修改记录:   2024-12-24:V1.0
						基于STM32F103RET6移植电位器工程到STM32F407ZET6
						使用HAL库 
						Firmware Package VERSION：V1.28.1
				
*****************************************************/
#ifndef __ADCONFIG__H__
#define __ADCONFIG__H__


#define VERSION "1.1"

#define NO_DEBUG_USART		1

#define OCXO_TIMER				1800				//<注意>实际时间至少3分钟，100ms定时器累加，应为：1800
#define OCXO_COUNT_VALUE	83999990


#define VALID_DATA		1
#define INVALID_DATA	0


#endif
