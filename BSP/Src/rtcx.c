#include "rtcx.h"
#include "driverPort.h"
#include "stdio.h"
void RTCInit(void)
{
	MX_RTC_Init();
}

void RTCDeInit(void)
{
	HAL_RTC_DeInit(&hrtc);
}
//RTC时间设置
//hour,min,sec:小时,分钟,秒钟
//ampm:@RTC_AM_PM_Definitions:RTC_HOURFORMAT12_AM/RTC_HOURFORMAT12_PM
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败
uint8_t RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm)
{
	RTC_TimeTypeDef RTC_TimeStructure;

	RTC_TimeStructure.Hours=hour;
	RTC_TimeStructure.Minutes=min;
	RTC_TimeStructure.Seconds=sec;
	RTC_TimeStructure.TimeFormat=ampm;
	RTC_TimeStructure.DayLightSaving=RTC_DAYLIGHTSAVING_NONE;
    RTC_TimeStructure.StoreOperation=RTC_STOREOPERATION_RESET;
	return HAL_RTC_SetTime(&hrtc,&RTC_TimeStructure,RTC_FORMAT_BIN);
}

//RTC日期设置
//year,month,date:年(0~99),月(1~12),日(0~31)
//week:星期(1~7,0,非法!)
//返回值:SUCEE(1),成功
//       ERROR(0),进入初始化模式失败
uint8_t RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week)
{
	RTC_DateTypeDef RTC_DateStructure;

	RTC_DateStructure.Date=date;
	RTC_DateStructure.Month=month;
	RTC_DateStructure.WeekDay=week;
	RTC_DateStructure.Year=year;
	return HAL_RTC_SetDate(&hrtc,&RTC_DateStructure,RTC_FORMAT_BIN);
}
//
void RTCSetTime(uint8_t *Time)
{
	RTC_Set_Time(Time[3],Time[4],Time[5],RTC_FORMAT_BIN);	//设置时间
	RTC_Set_Date(Time[0],Time[1],Time[2],7);				//设置日期
}
void RTCGetTime(uint8_t *Time)
{
	RTC_TimeTypeDef RTC_TimeStruct;
	RTC_DateTypeDef RTC_DateStruct;
	HAL_RTC_GetTime(&hrtc, &RTC_TimeStruct, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &RTC_DateStruct, RTC_FORMAT_BIN);
	Time[0] = RTC_DateStruct.Year;
	Time[1] = RTC_DateStruct.Month;
	Time[2] = RTC_DateStruct.Date;
	Time[3] = RTC_TimeStruct.Hours;
	Time[4] = RTC_TimeStruct.Minutes;
	Time[5] = RTC_TimeStruct.Seconds;
}
/*打印时间*/
void PrintfRTCTime(void)
{
	uint8_t time[6];
	RTCGetTime(time);
	printf("  当前时间:20%d年-%d月-%d日:%d时-%d分-%d秒\r\n",time[0],time[1],time[2],time[3],time[4],time[5]);
}
/*设置唤醒时间
	单位：秒,最大65535
*/
void RTCSetWakeUpTime(uint32_t s)
{
	HAL_NVIC_DisableIRQ(RTC_WKUP_IRQn);
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc,RTC_FLAG_WUTF);
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //清除Wake_UP标志 
	
	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);
	/*开启RTC中断，设置定时唤醒避免电量过低*/													
	HAL_RTCEx_SetWakeUpTimer_IT(&hrtc,s-1, RTC_WAKEUPCLOCK_CK_SPRE_16BITS);
}
/*设置闹钟中断*/
void RTCSetAlarmTime(uint32_t s)
{
	RTC_AlarmTypeDef sAlarm;
	RTC_TimeTypeDef nowTime;
	HAL_RTC_GetTime(&hrtc, &nowTime, RTC_FORMAT_BIN);
	

	HAL_NVIC_DisableIRQ(RTC_Alarm_IRQn);

	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);                  //清除Wake_UP标志 
	__HAL_RTC_ALARM_DISABLE_IT(&hrtc,RTC_IT_ALRA);
	
    __HAL_RTC_ALARM_CLEAR_FLAG(&hrtc, RTC_FLAG_ALRAF);

	
	
	
//	__HAL_RCC_PWR_CLK_ENABLE();         //使能PWR时钟
//    HAL_PWR_EnableBkUpAccess();         //后备区域访问使能 
	PrintfRTCTime();
	sAlarm.AlarmTime.Hours = nowTime.Hours;
	sAlarm.AlarmTime.Minutes = nowTime.Minutes;
	sAlarm.AlarmTime.Seconds = nowTime.Seconds+3;
	printf("sAlarm.AlarmTime.Seconds =%d\r\n",sAlarm.AlarmTime.Seconds);
	sAlarm.AlarmTime.SubSeconds = 5;
	sAlarm.AlarmTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
	sAlarm.AlarmTime.StoreOperation = RTC_STOREOPERATION_RESET;
	sAlarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY|RTC_ALARMMASK_HOURS
					  |RTC_ALARMMASK_MINUTES;
					  
	sAlarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_ALL;
	sAlarm.AlarmDateWeekDaySel = RTC_ALARMDATEWEEKDAYSEL_DATE;
	sAlarm.AlarmDateWeekDay = 1;
	sAlarm.Alarm = RTC_ALARM_A;
	__HAL_RTC_ALARM_ENABLE_IT(&hrtc,RTC_IT_ALRA);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);	
	if (HAL_RTC_SetAlarm_IT(&hrtc, &sAlarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		printf("Error_Handler\r\n");
		Error_Handler();
	}
	
}




__weak void RTC_WakeUp_IRQHandler(void)
{
	//printf("RTCSetWakeUpTime\r\n");
	
}


void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	UNUSED(hrtc);
	RTC_WakeUp_IRQHandler();
}


void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
 
		PrintfRTCTime();
}