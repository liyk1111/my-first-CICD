#include "ProcessesRegister.h"

//根据业务添加对应功能码和接口函数

void DeviceStatus_PROCESSLISTEN(void);
void DeviceSet_PROCESSLISTEN(void);

void AllProcessesRegister()
{
	RegisterProcesses(0xEA, DeviceStatus_PROCESSLISTEN);
	RegisterProcesses(0x300, DeviceSet_PROCESSLISTEN);
}




