#include "PackageRegister.h"

//根据业务添加对应功能码和接口函数

uint32_t DeviceStatusPackage_PACKAGEPARSER(uint8_t dest[]);
uint32_t DeviceSetPackage_PACKAGEPARSER(uint8_t dest[]);

void AllPackageRegister()
{
	RegisterPackage(0xEA, DeviceStatusPackage_PACKAGEPARSER);
	RegisterPackage(0x300, DeviceSetPackage_PACKAGEPARSER);
}




