/*
注册以及解析功能码对应的回调函数
1.因单片机内存较小，默认20个包长度，若业务包数太多自行修改
*/
#ifndef PROCESSESMANAGER_H
#define PROCESSESMANAGER_H

#include "Package.h"
#include "stdint.h"
#include "stdio.h"
#include "PackagePublic.h"

#define PROCESSES_MANAGER_LEN	20

typedef void (*PROCESSESLISTENER)(void);

void RegisterProcesses(int32_t command, PROCESSESLISTENER parser);
void Process(uint32_t tmpCommand);

#endif
