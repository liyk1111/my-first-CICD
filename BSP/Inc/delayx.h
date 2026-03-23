/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/
#ifndef __DELAYX__
#define __DELAYX__

//注意:最大只能设置900us左右，没有实测最终数值

#include "main.h"
#include <stdint.h>

extern TIM_HandleTypeDef htim2;

#define STMSYS_CLOCK        56    //单片机主频MHz
#define STMSYS_CLOCK_INDEX  5600  //主频乘以100

void ADDelay_ms(uint32_t mdelay);
void ADDelay_us(uint32_t udelay);

#endif
