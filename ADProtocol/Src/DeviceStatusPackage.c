#include "stdio.h"
#include "DeviceStatusPackage.h"
#include "stdlib.h"


//TODO：根据业务重写
Package *CreateDeviceStatusPackage(void)
{
	uint16_t i = 0;
	PackageProperty *pPackageProperty = NULL;
	Package *pPackage = createPackage(DEVICESTATUS_PROPERTY_NUM);	
	pPackage->command = DEVICESTATUS_COMMAND;										

	pPackageProperty = pPackage->properties;

	//根据业务重写，必须按协议顺序，否则读取会出现错误
	setPackagePropertyValueForINT(pPackageProperty++, 0);
	setPackagePropertyValueForBYTE(pPackageProperty++, 0);
	setPackagePropertyValueForSHORT(pPackageProperty++, 0);
	setPackagePropertyValueForBYTEArray(pPackageProperty++, 0, DEVICESTATUS_PROPERTY_BUFLEN);
	
	//setPackagePropertyValueForINT(pPackageProperty++, 0);
	
	
	//for(i = 0;i < DEVICESTATUS_PROPERTY_BUFLEN;i++)
	//	setPackagePropertyValueForBYTE(pPackageProperty++, 0);

	return pPackage;
}
