/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef __GPIOX__
#define __GPIOX__

#include "main.h"
#include <stdint.h>

//可以根据使用场景更改-更改需添加注释-硬件电平改变也在此定义即可
//设备使用GPIO定义
#define POWERCTRL_ENABLE          1  //开-根据原理图定义高电平还是低电平
#define POWERCTRL_DISABLE         0  //关
#define POWERCTRL_GPIO_PORT       PowerCtrl_GPIO_Port
#define POWERCTRL_PIN             PowerCtrl_Pin

#define WBPOWER_ENABLE           0  //开-根据原理图定义高电平还是低电平
#define WBPOWER_DISABLE          1  //关
#define WBPOWER_GPIO_PORT        WB_PWR_GPIO_Port
#define WBPOWER_PIN              WB_PWR_Pin

#define TFPOWER_ENABLE           0  //开-根据原理图定义高电平还是低电平
#define TFPOWER_DISABLE          1  //关
#define TFPOWER_GPIO_PORT        TF_PWR_GPIO_Port
#define TFPOWER_PIN              TF_PWR_Pin

#define GPSEN_ENABLE             0  //开-根据原理图定义高电平还是低电平
#define GPSEN_DISABLE            1  //关
#define GPSEN_GPIO_PORT          GPS_EN_GPIO_Port
#define GPSEN_PIN                GPS_EN_Pin


//
#define POWERKEY_GPIO_PORT        PowerKeyIn_GPIO_Port
#define POWERKEY_PIN              PowerKeyIn_Pin
//无电源接入：低电平，有电源接入：高电平
#define DCIN_GPIO_PORT            DCIN_GPIO_Port
#define DCIN_PIN                  DCIN_Pin
//无电源接入：高电平，有电源接入：低电平-充电   高电平-充满
#define CHARGESTATUS_GPIO_PORT    CHG_STAT_GPIO_Port
#define CHARGESTATUS_PIN          CHG_STAT_Pin





void GPIOSetOutputPinLevel(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
uint16_t GPIOReadInputPinLevel(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);

#endif
