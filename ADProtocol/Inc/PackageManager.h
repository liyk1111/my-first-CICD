/*
注册以及解析功能码对应的回调函数
1.因单片机内存较小，默认20个包长度，若业务包数太多自行修改
*/
#ifndef PACKAGEMANAGER_H
#define PACKAGEMANAGER_H

#include "Package.h"
#include "stdint.h"
#include "stdio.h"
#include "PackagePublic.h"

#define PACKAGE_MANAGER_LEN	20

typedef uint32_t (* PACKAGEPARSER)(uint8_t dest[]);

void RegisterPackage(int32_t command, PACKAGEPARSER parser);
uint32_t Parse(uint8_t dest[]);

#endif
