/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef __EXTIX__
#define __EXTIX__

#include "main.h"
#include <stdint.h>
#include "stm32f4xx_hal_gpio.h"
#include "stm32f4xx.h"






//#define BSPEXTI1_IRQ   EXTI1_IRQn
#define BSPEXTI2_IRQ   EXTI2_IRQn
#define BSPEXTI3_IRQ   EXTI3_IRQn
//#define BSPEXTI11_IRQ  EXTI15_10_IRQn
//#define BSPEXTI10_IRQ  EXTI15_10_IRQn
//#define BSPEXTI12_IRQ  EXTI15_10_IRQn





void EXTIn_IRQ_ENABLE(IRQn_Type IRQn);
void EXTIn_IRQ_DISABLE(IRQn_Type IRQn);

#endif
