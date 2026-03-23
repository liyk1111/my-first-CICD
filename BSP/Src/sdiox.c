#include "sdiox.h"
#include "driverPort.h"
/*SDIO初始化，应该先初始化SDIO后再初始化FATFS*/
void SdioInit(void)
{
	MX_SDIO_SD_Init();
}

void SdioDeInit(void)
{
	HAL_SD_DeInit(&hsd);
}
