/*
使用说明：
    1.常用芯片选择，或者自行定义
    2.使用的定时器才需要开启，否则屏蔽；原因是源文件定义了全部初始化接口，这样源文件就无需更改
    3.定时器中断处理函数TimIRQHandler和捕获中断处理函数TimCaptureIRQHandler必须重新实现，
      所有中断都指向一个中断处理函数，内部根据不同定时器执行不同操作。

功能介绍：
    1.定时器的使用
    2.捕获的使用，捕获两个上升/下降沿的时间，计算工频和外部输入的频率，做相位自适应功能。

更新记录：
    2023-11-20：创建
*/
#ifndef TIMERX_H
#define TIMERX_H
#include "stdint.h"
#include "tim.h"

//常用芯片系列选择定义，没有定义即为自行配置时钟频率
#define AD_STM32F407
//#define AD_STM32F405
//#define AD_STM32F103 

//正常配置下：APB2 = 2 * APB1
#if defined(AD_STM32F407) | defined(AD_STM32F405)
//#define __Tim1ClockFreq  168000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim2ClockFreq  84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim3ClockFreq  84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim4ClockFreq  84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim5ClockFreq  84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim6ClockFreq  84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim7ClockFreq  84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim8ClockFreq  168000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim9ClockFreq  168000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim10ClockFreq 168000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim11ClockFreq 168000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim12ClockFreq 84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim13ClockFreq 84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
//#define __Tim14ClockFreq 84000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#elif defined(AD_STM32F103)
#define __Tim1ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim2ClockFreq  36000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim3ClockFreq  36000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim4ClockFreq  36000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim5ClockFreq  36000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim6ClockFreq  36000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim7ClockFreq  36000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim8ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#else   //自定义：例如F407使用三重高速ADC，频率不能设置168M，只能设置144M，相对应频率就不能用上面的固有定义
#define __Tim1ClockFreq  144000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim2ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim3ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim4ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim5ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim6ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim7ClockFreq  72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim8ClockFreq  144000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim9ClockFreq  144000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim10ClockFreq 144000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim11ClockFreq 144000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim12ClockFreq 72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim13ClockFreq 72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#define __Tim14ClockFreq 72000000		/*定时器的时钟频率，查找手册的系统架构，AHB/APB总线*/
#endif


//定时器部分
void TimInit(TIM_HandleTypeDef *htim);                                  //定时器初始化
void TimDeInit(TIM_HandleTypeDef *htim);                                //定时器释放
uint32_t TimGetClock(TIM_HandleTypeDef *htim);                          //获取定时器时钟
void TimInitWithSetPeriod_us(TIM_HandleTypeDef *htim, uint32_t us);     //初始化定时器同时设置定时的时间.单位微秒。
void TimInitWithSetPeriod_ms(TIM_HandleTypeDef *htim, uint32_t ms);     //初始化定时器同时设置定时的时间.单位毫秒。
void TimSetPeriod_us(TIM_HandleTypeDef *htim, uint32_t us);             //设置定时器中断时间间隙	单位：微秒
void TimSetPeriod_ms(TIM_HandleTypeDef *htim, uint32_t ms);             //设置定时器中断时间间隙	单位：毫秒
void TimStart(TIM_HandleTypeDef *htim);                                 //开始定时
void TimStop(TIM_HandleTypeDef *htim);                                  //停止计时
uint32_t TimGetCnt(TIM_HandleTypeDef *htim);                            //获取定时器值
void TimSetCnt(TIM_HandleTypeDef *htim, uint32_t counter);              //设置定时器值
void TimSetPrescaler(TIM_HandleTypeDef *htim, uint32_t tmpPsc);		    //设置定时器分频
void TimSetAutoReload(TIM_HandleTypeDef *htim, uint32_t tmpArr);	    //设置定时器重装载值
void TimGenerateEvent(TIM_HandleTypeDef *htim);							//设置定时器配置立刻生效
extern void TimIRQHandler(TIM_HandleTypeDef *htim);

//PWM部分
uint8_t PWMInitWithSetFrequency(TIM_HandleTypeDef *htim, uint32_t ClockFrequency, uint32_t PWMFrequency);
void PWMInit(TIM_HandleTypeDef *htim);                                                  //初始化PWM
void PWMDeInit(TIM_HandleTypeDef *htim);                                                //释放PWM
void PWMStartOutput(TIM_HandleTypeDef *htim, uint32_t xChannel);                        //开始输出PWM
void PWMStopOutput(TIM_HandleTypeDef *htim, uint32_t xChannel);                         //停止输出PWM
uint8_t PWMSetCompare(TIM_HandleTypeDef *htim, uint32_t xChannel,float Compare);        //设置占空比，Compare:占空比(单位:%)
uint8_t PWMSetPolarity(TIM_HandleTypeDef *htim, uint32_t xChannel, uint32_t Polarity);  //设置极性

//捕获部分
void TimCaptureInit(TIM_HandleTypeDef *htim);                            //捕获初始化 - 等同于定时器初始化
void TimCaptureDeInit(TIM_HandleTypeDef *htim);                          //捕获释放
void TimCaptureStart(TIM_HandleTypeDef *htim, uint32_t timCH);           //捕获开始
void TimCaptureStop(TIM_HandleTypeDef *htim, uint32_t timCH);            //捕获停止
extern void TimCaptureIRQHandler(TIM_HandleTypeDef *htim);

#endif
