#ifndef __APPPUBLIC_H
#define __APPPUBLIC_H
#include "cmsis_os.h"
#include "stdint.h"
#include "gpiox.h"
#include "stdio.h"
#include "flashx.h"
#include "ADConfig.h"
#include "adcx.h"
#include "DebugPlatform.h"

//任务句柄
extern osThreadId defaultTaskHandle;


//信号量、互斥量
extern osSemaphoreId DebugSemHandle;
extern osSemaphoreId adc6semHandle;


#endif

