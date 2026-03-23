#ifndef __ADINTERACTION_H
#define __ADINTERACTION_H
#include "stdint.h"

/*************************************************************************************************************************/
/*可以修改区域*/
extern void InteractionImmediately(void);
extern uint32_t bsp_delay_getTick(void);

/*接口定义*/
#define ADInteraction_Malloc 				malloc
#define ADInteraction_Free					free
#define ADInteraction_GetTick   			bsp_delay_getTick				 /*获取时钟节拍，单位毫秒*/
//#define ADInteraction_RunImmediately 	 	InteractionImmediately   /*此函数可实现，也可以不实现。使用裸机时，可以考虑不需要实现这个函数，
//																		使用定时器或者系统为性能考虑应当实现*/
																		
/*配置项*/
#define configEnInteractionFinishCallback		1
/*************************************************************************************************************************/
//以下不要修改
#define ADInteraction_Err 		0
#define ADInteraction_OK		1
#define ADInteraction_WaitForever 0xffffffff


typedef struct
{
	void (*fun)(void) ;
	uint32_t RunTime;	
}
ADInteractionMode;

typedef struct
{	
	uint8_t 	name;
	uint8_t 	isRun;							//是够开启执行
	uint16_t 	RunTimes;						//次数
	uint8_t	  	Prio;							/*优先等级*/
	uint8_t 	Offset;		
	uint16_t 	RunCnt;							//已经执行的次数
				
	uint8_t 	isProtect;
	
	uint8_t  	TaskLenght;							
	const ADInteractionMode *InteractionTask;

}
ADInteractionBodyStruct;

typedef struct ADInteraction
{	
	ADInteractionBodyStruct *Body;	
	uint32_t	StartTime;
	void (*DefaultFun)(void) ;
	struct ADInteraction *next;
#if configEnInteractionFinishCallback == 1
	void (*finishCallback)(void *,uint8_t);	
#endif
	uint8_t 	BodyNum;
}
ADInteraction;

/*创建一个交互*/
void *ADInteractionCreat(uint8_t num);
/*添加一个交互*/
uint8_t ADInteractionAdd(void *xADInteraction,uint8_t name,uint8_t prio,uint16_t times,const ADInteractionMode *TaskList,uint8_t lenght);
uint32_t ADInteractionRun(void);
void ADInteractionStart(void *xADInteraction,uint8_t name);
void ADInteractionStop(void *xADInteraction,uint8_t name);
void ADInteractionPrintf(void);
/*设置默认状态*/
void ADInteractionSetDefault(void *xADInteraction,void *f);
/*设置交互保护*/
void ADInteractionSetProtect(void *xADInteraction,uint8_t name);
/*清除交互保护*/
void ADInteractionClearProtect(void *xADInteraction,uint8_t name);
#if configEnInteractionFinishCallback == 1
//设置交互方式完成回调
void ADInteractionSetFinishCallback(void *xADInteraction,void *f);
#endif

#endif
