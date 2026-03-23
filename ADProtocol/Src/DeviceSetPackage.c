#include "stdio.h"
#include "DeviceSetPackage.h"
#include "stdlib.h"


//TODO：根据业务重写
Package *CreateDeviceSetPackage(void)
{
	uint16_t i = 0;
	PackageProperty *pPackageProperty = NULL;
	Package *pPackage = createPackage(DEVICESET_PROPERTY_NUM);	
	pPackage->command = DEVICESET_COMMAND;										

	pPackageProperty = pPackage->properties;

	//根据业务重写，必须按协议顺序，否则读取会出现错误
	setPackagePropertyValueForINT(pPackageProperty++, 0);
	setPackagePropertyValueForDouble(pPackageProperty++, 0);
	setPackagePropertyValueForDouble(pPackageProperty++, 0);
	setPackagePropertyValueForFloat(pPackageProperty++, 0);
	setPackagePropertyValueForFloat(pPackageProperty++, 0);
	
	return pPackage;
}
