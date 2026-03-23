/*
接收包，需要实现回调函数以及线程处理函数：两个回调都建议在APP处实现，方便使用全局变量
1.uint32_t xxxPackage_PACKAGEPARSER(uint8_t dest[])，根据业务重写
2.void xxx_PROCESSLISTEN(uint32_t tmpCommand)，根据业务重写
3.CreatexxxPackage，根据业务重写
*/
#ifndef DEVICESETPACKAGE_H
#define DEVICESETPACKAGE_H

#include "stdint.h"
#include "Package.h"
#include "PackageConfig.h"
#include "PackagePublic.h"

typedef struct{
	uint32_t 	sequenceid;
	double 		pulseWidth;
  double 		triggerLevel;
	float 		powerFreq;
  float 		preTrigger;
}DeviceSetPackage;

Package *CreateDeviceSetPackage(void);

#endif
