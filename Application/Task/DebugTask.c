#include "stdio.h"
#include "ADDebug.h"
#include "ADConfig.h"
#include "stdlib.h"
#include "string.h"
//#include "timerx.h"
//#include "dac.h"
#include "appPublic.h"

extern uint16_t G_NOGPSCount;

//初始化串口调试指令
void InitDebugIns(void);
void PrintVersion(void);

int str2int(char* buff)
{
	char* pos;
	int val=0;
	
	pos = strchr(buff, '=');
	if(pos != NULL){
		val = strtol(pos+1, NULL, 10);
	}

	return val;
}

double str2double(char* buff)
{
	char* pos;
	double val=0.0;
	
	pos = strchr((char*)buff, '=');
	if(pos != NULL){
		val = strtod(pos+1, NULL);
	}

	return val;
}

//end

void Debug_Rx_IRQHandler(void)
{
	osSemaphoreRelease(DebugSemHandle);
}

void DebugTask(void const * argument)
{
	printf("DebugTask\r\n");
	uint8_t *DebugIns = NULL;
	uint16_t DebugInsLenght = 0;
	
	//初始化Debug
	DebugDriverInit();
	InitDebugIns();

	while(1)
	{
		osSemaphoreWait(DebugSemHandle, osWaitForever);					/*等待接收指令*/
		if(GetDebugRxLenght()!= 0)
		{
			DebugInsLenght = GetDebugRxLenght();							/*获取指令长度*/
			DebugIns = GetDebugRxData();							        /*获取指令内容*/
			ADDebugRun(DebugIns,DebugInsLenght);							/*解析指令*/
			ClearDebugRxLenght();											        /*清除指令*/
		}		
	}
}

//初始化串口调试指令
void InitDebugIns(void)
{
	ADDebugAddIns(ADDebug_ControlMode, "VER", PrintVersion);					//软件版本

}


void PrintVersion()
{
    printf("Version:%s\r\n",VERSION);
}






