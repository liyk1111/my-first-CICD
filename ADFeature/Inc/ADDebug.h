#ifndef __ADDEBUG_H
#define __ADDEBUG_H
#include "stdint.h"
#include "string.h"
#include "stdio.h"

#define ADDebug_debugInsMaxSize 50
#define ADDebug_sameInsMaxSize  5

#define ADDebug_UnknownMode		0
#define ADDebug_ControlMode 	1
#define ADDebug_ParaMode    	2
#define ADDebug_FileMode    	3
#define ADDebug_CalibrationMode 4
#define ADDebug_SpecificMode	5

#define ADDebug_Err				0
#define ADDebug_OK				1

typedef unsigned char (*debugCallback)(uint8_t *data,uint16_t len);

typedef struct{
	debugCallback 	dFunc;					/*回调函数*/
	const 	char 	*debugInsStr;			/*调试指令*/
	uint8_t 		debugInsStrLenght;		/*指令长度*/
	uint8_t 		debugInsType;			//指令类型。0为控制，1参数输入，2文件输入
	const 	char 	*remindStr;				/*提示内容*/
	uint8_t 		remindStrLenght;		/*提示内容长度*/	
	
	debugCallback    preFunc;
	debugCallback	 postFunc;
}debugInsStruct;

typedef enum{
	ADDebugRunState_Init,
	ADDebugRunState_WaitData,

}ADDebugRunInsState;

/*初始化调试*/
void ADDebugInit(void);
/*添加调试指令*/
uint8_t ADDebugAddIns(uint8_t mode ,const char *debugInsStr,void *f,...);
/*添加调试指令*/
uint8_t ADDebugAddInsWithDeclare(uint8_t mode ,const char *debugInsStr,void *f,...);
uint8_t ADDebugRun(uint8_t *data,uint16_t len);
void ADDebugPrintf(void);
void ADDebugSetPassword( char *password);
void ADDebugLock(void);

uint8_t ADDebugRun2(uint8_t *data,uint16_t len);
#endif
