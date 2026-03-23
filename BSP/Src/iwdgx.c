#include "iwdgx.h"
#include "driverPort.h"

void IwdgInit(void)
{
	 MX_IWDG_Init();
}

void IwdgFeed(void)
{
	 HAL_IWDG_Refresh(&hiwdg); 	//Î¹¹·
}
