/*
使用说明：
    1.

功能介绍：
    1.

更新记录：
    2023-11-20：TODO：需要下载到开发板重复验证
*/

#ifndef __RTCX_H
#define __RTCX_H
#include "stdint.h"
void RTCInit(void);
void RTCDeInit(void);

//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败
uint8_t RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);
//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败
uint8_t RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week);

void RTCSetTime(uint8_t *Time);
void RTCGetTime(uint8_t *Time);
void RTCSetWakeUpTime(uint32_t s);
/*设置闹钟中断*/
void RTCSetAlarmTime(uint32_t s);
#endif
