/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef __LEDX__
#define __LEDX__

#include "main.h"
#include <stdint.h>

//可以根据使用场景更改-更改需添加注释-硬件电平改变也在此定义即可
//状态灯定义
#define LED_STATUS_TOGETER      1  //1-表示硬件选型的红绿灯是相连接的，绿灯亮时红灯需要灭，红灯亮时绿灯灭，否则同时亮起会变为橙色
#if(LED_STATUS_TOGETER == 1)
#define LED_STATUS_GREEN_ON     GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_STATUS_GREEN_OFF    GPIO_PIN_RESET  //灭
#define LED_STATUS_RED_ON       GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_STATUS_RED_OFF      GPIO_PIN_RESET  //灭
#define LEDSTATUSRED_GPIO_PORT  LedStatusRed_GPIO_Port  //使用需定义
#define LEDSTATUSRED_PIN        LedStatusRed_Pin  //使用需定义
#define LEDSTATUSGREEN_GPIO_PORT LedStatusGreen_GPIO_Port  //使用需定义
#define LEDSTATUSGREEN_PIN       LedStatusGreen_Pin  //使用需定义
#endif

//警示灯定义 - 如有
#define LED_ALARM_GREEN_USE     0  //1-启用
#if(LED_ALARM_GREEN_USE == 1)
#define LED_ALARM_GREEN_ON      GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_ALARM_GREEN_OFF     GPIO_PIN_RESET  //灭
#define LEDALARMGREEN_GPIO_PORT X  //使用需定义
#define LEDALARMGREEN_PIN       X  //使用需定义
#endif

#define LED_ALARM_RED_USE       0  //1-启用
#if(LED_ALARM_RED_USE == 1)
#define LED_ALARM_RED_ON        GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_ALARM_RED_OFF       GPIO_PIN_RESET  //灭
#define LEDALARMRED_GPIO_PORT X  //使用需定义
#define LEDALARMRED_PIN       X  //使用需定义
#endif

#define LED_ALARM_YELLOW_USE    0  //1-启用
#if(LED_ALARM_YELLOW_USE == 1)
#define LED_ALARM_YELLOW_ON     GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_ALARM_YELLOW_OFF    GPIO_PIN_RESET  //灭
#define LEDALARMYELLOW_GPIO_PORT X  //使用需定义
#define LEDALARMYELLOW_PIN       X  //使用需定义
#endif

//其他灯定义 - 如有
#define LED_A_USE               0  //1-启用
#if(LED_A_USE == 1)
#define LED_A_ON                GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_A_OFF               GPIO_PIN_RESET  //灭
#define LEDA_GPIO_PORT          X  //使用需定义
#define LEDA_PIN                X  //使用需定义
#endif

#define LED_B_USE               0  //1-启用
#if(LED_B_USE == 1)
#define LED_B_ON                GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_B_OFF               GPIO_PIN_RESET  //灭
#define LEDB_GPIO_PORT          X  //使用需定义
#define LEDB_PIN                X  //使用需定义
#endif

#define LED_C_USE               0  //1-启用
#if(LED_C_USE == 1)
#define LED_C_ON                GPIO_PIN_SET  //亮-根据原理图定义高电平还是低电平
#define LED_C_OFF               GPIO_PIN_RESET  //灭
#define LEDC_GPIO_PORT          X  //使用需定义
#define LEDC_PIN                X  //使用需定义
#endif

//状态LED接口
#if(LED_STATUS_TOGETER == 1)
void StatusLedRedTurnOn(void);
void StatusLedRedTurnOff(void);
void StatusLedGreenTurnOn(void);
void StatusLedGreenTurnOff(void);
#endif

//警示LED接口
#if(LED_ALARM_GREEN_USE == 1)
void AlarmLedGreenTurnOn(void);
void AlarmLedGreenTurnOff(void);
#endif

#if(LED_ALARM_RED_USE == 1)
void AlarmLedRedTurnOn(void);
void AlarmLedRedTurnOff(void);
#endif

#if(LED_ALARM_YELLOW_USE == 1)
void AlarmLedYellowTurnOn(void);
void AlarmLedYellowTurnOff(void);
#endif

//其他LED接口
#if(LED_A_USE == 1)
void LedATrunOn(void);
void LedATrunOff(void);
#endif

#if(LED_B_USE == 1)
void LedBTrunOn(void);
void LedBTrunOff(void);
#endif

#if(LED_C_USE == 1)
void LedCTrunOn(void);
void LedCTrunOff(void);
#endif

#endif
