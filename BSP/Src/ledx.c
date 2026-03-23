/*
注意事项:
1.如需改动，由组内统一讨论更新

*/
#include "ledx.h"
//#include "Public.h"???

//初始化灯的状态 --- TODO:其实初始化操作由APP自由决定，只提供常规使用接口，LED个数等
/*void InitAllLedStatus(void)
{
    StatusLedRedTurnOff();
    StatusLedGreenTurnOn();

    GroundLedGreenTurnOff();
    GroundLedRedTurnOff();
    GroundLedYellowTurnOff();

    LEDA(TurnOff);
    LEDB(TurnOff);
    LEDC(TurnOff);
}*/

#if(LED_STATUS_TOGETER == 1)
//打开红色状态灯
void StatusLedRedTurnOn(void)
{
    HAL_GPIO_WritePin(LEDSTATUSRED_GPIO_PORT, LEDSTATUSRED_PIN, (GPIO_PinState)LED_STATUS_RED_ON);
#if(LED_STATUS_TOGETER == 1)
    StatusLedGreenTurnOff();
#endif
}
//关闭红色状态灯
void StatusLedRedTurnOff(void)
{
    HAL_GPIO_WritePin(LEDSTATUSRED_GPIO_PORT, LEDSTATUSRED_PIN, (GPIO_PinState)LED_STATUS_RED_OFF);
}
//打开绿色状态灯
void StatusLedGreenTurnOn(void)
{
    HAL_GPIO_WritePin(LEDSTATUSGREEN_GPIO_PORT, LEDSTATUSGREEN_PIN, (GPIO_PinState)LED_STATUS_GREEN_ON);
#if(LED_STATUS_TOGETER == 1)
    StatusLedRedTurnOff();
#endif
}
//关闭绿色状态灯
void StatusLedGreenTurnOff(void)
{
    HAL_GPIO_WritePin(LEDSTATUSGREEN_GPIO_PORT, LEDSTATUSGREEN_PIN, (GPIO_PinState)LED_STATUS_GREEN_OFF);
}
#endif


#if(LED_ALARM_GREEN_USE == 1)
//绿灯开
void AlarmLedGreenTurnOn(void)
{
    HAL_GPIO_WritePin(LedGroundAlarmGreen_GPIO_Port, LedGroundAlarmGreen_Pin, (GPIO_PinState)TurnOn);
}
//绿灯关
void AlarmLedGreenTurnOff(void)
{
    HAL_GPIO_WritePin(LedGroundAlarmGreen_GPIO_Port, LedGroundAlarmGreen_Pin, (GPIO_PinState)TurnOff);
}
#endif

#if(LED_ALARM_RED_USE == 1)
//红色灯开
void AlarmLedRedTurnOn(void)
{
    HAL_GPIO_WritePin(LedGroundAlarmRed_GPIO_Port, LedGroundAlarmRed_Pin, (GPIO_PinState)TurnOn);
}
//红色灯关
void AlarmLedRedTurnOff(void)
{
    HAL_GPIO_WritePin(LedGroundAlarmRed_GPIO_Port, LedGroundAlarmRed_Pin, (GPIO_PinState)TurnOff);
}
#endif

#if(LED_ALARM_YELLOW_USE == 1)
//黄色灯开
void AlarmLedYellowTurnOn(void)
{
    HAL_GPIO_WritePin(LedGroundAlarmYellow_GPIO_Port, LedGroundAlarmYellow_Pin, (GPIO_PinState)TurnOn);
}
//黄色灯关
void AlarmLedYellowTurnOff(void)
{
    HAL_GPIO_WritePin(LedGroundAlarmYellow_GPIO_Port, LedGroundAlarmYellow_Pin, (GPIO_PinState)TurnOff);
}
#endif
/*
void AlarmLedAllTurnOff(void)
{
    HAL_GPIO_WritePin(LedGroundAlarmYellow_GPIO_Port, LedGroundAlarmYellow_Pin, (GPIO_PinState)TurnOff);
    HAL_GPIO_WritePin(LedGroundAlarmGreen_GPIO_Port, LedGroundAlarmGreen_Pin, (GPIO_PinState)TurnOff);
    HAL_GPIO_WritePin(LedGroundAlarmRed_GPIO_Port, LedGroundAlarmRed_Pin, (GPIO_PinState)TurnOff);
//	GroundLedYellowTurnOff();
//	GroundLedGreenTurnOff();
//	GroundLedRedTurnOff();
};*/

#if(LED_A_USE == 1)
void LedATrunOn()
{
    HAL_GPIO_WritePin(LEDA_GPIO_PORT, LEDA_PIN, (GPIO_PinState)LED_A_ON);
}
void LedATrunOff()
{
    HAL_GPIO_WritePin(LEDA_GPIO_PORT, LEDA_PIN, (GPIO_PinState)LED_A_OFF);
}
#endif

#if(LED_B_USE == 1)
void LedBTrunOn()
{
    HAL_GPIO_WritePin(LEDB_GPIO_PORT, LEDB_PIN, (GPIO_PinState)LED_B_ON);
}
void LedBTrunOff()
{
    HAL_GPIO_WritePin(LEDB_GPIO_PORT, LEDB_PIN, (GPIO_PinState)LED_B_OFF);
}
#endif

#if(LED_C_USE == 1)
void LedCTrunOn()
{
    HAL_GPIO_WritePin(LEDC_GPIO_PORT, LEDC_PIN, (GPIO_PinState)LED_C_ON);
}
void LedCTrunOff()
{
    HAL_GPIO_WritePin(LEDC_GPIO_PORT, LEDC_PIN, (GPIO_PinState)LED_C_OFF);
}
#endif
