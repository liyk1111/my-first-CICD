#ifndef __ADNET_H
#define __ADNET_H
#include "stdint.h"
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdint.h"
#include "math.h"
#include "cmsis_os.h"

#define  __ADNetGetTick 		(osKernelSysTick()*portTICK_PERIOD_MS)

/************************************************************************************************/
#define ADNet_config_StepIDLenght 			16/*步骤ID的长度,取值范围 8,16,32*/
#define ADNet_config_ResendUseFastArith		0/*是否使用快速重发算法。1是开启，0是关闭，这个暂时不要打开，没有实现*/
#define ADNet_config_ResendFlagSlotNum		4/*取值范围1-8*/
#define ADNet_config_UseMacroAPI			1/*是否使用宏API来提高允许速度*/
#define ADNet_config_EnStepTiming			1/*使能步骤定时*/
#define ADNet_config_MaxPrioGroupNum		3/*优先级组数量*/
/************************************************************************************************/
/*动态内存*/
#define ADNetMalloc 		malloc
#define ADNetFree 	 		free





#define ADNet_NO				0
#define ADNet_YES				1


#define ADNetSet_PRIO1			1
#define ADNetSet_PRIO2			2
#define ADNetSet_PRIO3			3
#define ADNetSet_PRIO4			4
#define ADNetSet_PRIO5			5
#define ADNetSet_PRIO6			6
#define ADNetSet_PRIO7			7
#define ADNetSet				1
#define ADNetReset				0



#define ADNet_SET_BIT(REG, BIT)     ((REG) |= (BIT))

#define ADNet_CLEAR_BIT(REG, BIT)   ((REG) &= ~(BIT))

#define ADNet_READ_BIT(REG, BIT)    ((REG) & (BIT))

#define ADNet_CLEAR_REG(REG)        ((REG) = (0x0))

#define ADNet_WRITE_REG(REG, VAL)   ((REG) = (VAL))

#define ADNet_READ_REG(REG)         ((REG))


#define		ADNet_RecordReSend		0x08			/*是否已经记录重发了*/

#define ADNet_ParaNone				0				/*没有参数*/
#define ADNet_ParaUint8_t			1				/*uint8_t类型参数*/
#define ADNet_ParaUint16_t			2				/*uint16_t类型参数*/
#define ADNet_ParaUint32_t			3				/*uint32_t类型参数*/
#define ADNet_ParaFloat				4				/*float类型参数*/
#define ADNet_ParaDouble			5				/*double类型参数*/
#define ADNet_ParaPointer			6				/*void指针类型参数,实际上所有的指针类型参数都可以用这个类型*/
#define ADNet_ParaDynamicPointer    7				/*void指针类型参数，且参数用堆备份，实际上所有的指针类型参数都可以用这个类型*/
#define ADNet_ParaUint8_tPointer    8				/*uint8_t指针类型参数*/
#define ADNet_ParaUint16_tPointer   9				/*uint16_t指针类型参数*/
#define ADNet_ParaUint32_tPointer   10				/*uint32_t指针类型参数*/
#define ADNet_ParaFloatPointer    	11				/*float指针类型参数*/
#define ADNet_ParaDoublePointer    	12				/*double指针类型参数*/
#define ADNet_ParaCharPointer    	13				/*char指针类型参数*/

#define ADNet_Combination(a,b)			a##b
#define ADNet_CombinationUnion(a,b)		ADNet_Combination(a,b)
#define ADNetSetStepWithPara(xADNet,f,Prio,ParaType,Para,...)	{\
																		fpUnion xfpUnion;										\
																		ADNet_Combination(xfpUnion.fp,ParaType) = Para;				\
																		S_ADNetSetStepWithPara(xADNet,f,Prio,ParaType,xfpUnion,##__VA_ARGS__);		\
																}
																
#define ADNetSetTimingStepWithPara(xADNet,f,Prio,readyTime,ParaType,Para,...)	{\
																		fpUnion xfpUnion;										\
																		ADNet_Combination(xfpUnion.fp,ParaType) = Para;				\
																		S_ADNetSetTimingStepWithPara(xADNet,f,Prio,readyTime,ParaType,xfpUnion,##__VA_ARGS__);		\
																}																
/**************************************************************************************/

typedef void (*ADNetIdleAndReadyCallBack)(void *ADNets);
typedef void (*ADNetStepFailCallBack)(void *ADNets,void *f,unsigned int id);

typedef union 
{
	uint8_t 	ADNet_CombinationUnion(fp,ADNet_ParaUint8_t);	
	uint16_t 	ADNet_CombinationUnion(fp,ADNet_ParaUint16_t);	
	uint32_t 	ADNet_CombinationUnion(fp,ADNet_ParaUint32_t);	
	float 		ADNet_CombinationUnion(fp,ADNet_ParaFloat);	
	double 		ADNet_CombinationUnion(fp,ADNet_ParaDouble);	
	void *		ADNet_CombinationUnion(fp,ADNet_ParaPointer);
	void *		ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer);
	
	uint8_t *	ADNet_CombinationUnion(fp,ADNet_ParaUint8_tPointer);
	uint16_t *	ADNet_CombinationUnion(fp,ADNet_ParaUint16_tPointer);
	uint32_t *	ADNet_CombinationUnion(fp,ADNet_ParaUint32_tPointer);
	float *		ADNet_CombinationUnion(fp,ADNet_ParaFloatPointer);
	double*		ADNet_CombinationUnion(fp,ADNet_ParaDoublePointer);
	
	char  *		ADNet_CombinationUnion(fp,ADNet_ParaCharPointer);
}  fpUnion;
typedef unsigned char (*ADNetStepF)(fpUnion Para,unsigned short int len);
typedef struct 
{
	fpUnion		fp;
	unsigned	int		fpLenght;		/*步骤执行是的参数的长度*/
	unsigned	char	fpType;			/*步骤执行是的参数的类型*/
#if ADNet_config_StepIDLenght	==	8
	unsigned	char			id;
#elif	ADNet_config_StepIDLenght	==	16
	unsigned	short	int		id;
#elif	ADNet_config_StepIDLenght	==	32
	unsigned	int				id;
#endif


}
ADNetPassParameterBody;

typedef void (*ADNetReSendFailCallBack)(void *ADNets,void *f,ADNetPassParameterBody	*formalPara);
/*步骤的参数结构体*/
typedef struct ADNetPassParameter
{
	ADNetPassParameterBody		formalPara;	/*步骤函数的参数*/
	struct ADNetPassParameter	*next;
}
ADNetPassParameter;

/*重发结构体*/
typedef struct 
{
	unsigned	int		reSendPeriod;					/*重发间隔*/
	unsigned	int		reSendTimes;					/*重发次数*/
	ADNetReSendFailCallBack		reSendFailCallBack;		/*重发失败回调函数*/
	unsigned	int		reSendStartTime;				/*发送时间*/	
	unsigned	int		reSendCnt;						/*已经重发的次数*/
	unsigned	char	isReady;						/*重发的优先级*/

}
ADNetResend;

/*步骤的结构体*/
typedef struct 
{
	unsigned	char	isReady;				/*优先等级*/
	ADNetResend			*reSend;				/*重发*/
	ADNetPassParameter	para;					/*步骤传递参数*/
	unsigned	short	int		sendFailCnt;	/*发送失败计数*/	
#if ADNet_config_StepIDLenght	==	8					/*步骤ID生成*/
	unsigned	char	IDCreator;
#elif	ADNet_config_StepIDLenght	==	16
	unsigned	short	int		IDCreator;
#elif	ADNet_config_StepIDLenght	==	32
	unsigned	int		IDCreator;
#endif	

#if ADNet_config_EnStepTiming == 1
	uint32_t isReadyTime;
#endif	
	
}
ADNetStep;


/*步骤的结构体*/
typedef struct 
{
	ADNetStep 					*Step;
	const ADNetStepF			*sFunc;
	unsigned	short	int		lenght;		
}
ADNetStepAssembly;

typedef struct 
{	
	ADNetStepAssembly			StepAssembly[ADNet_config_MaxPrioGroupNum];	
	ADNetStep					backupsRunStep;		/*备份*/
	unsigned	char			isIdle;				/*步骤是否全部清零*/
#if ADNet_config_ResendUseFastArith == 1
	unsigned    char 			resendFlag;			/*重发标志位*/
	unsigned short int resendSlotSize[ADNet_config_ResendFlagSlotNum];	/*每个槽的大小*/		
#endif
	ADNetIdleAndReadyCallBack	isIdleCallBack;		/**/
	ADNetIdleAndReadyCallBack	isReadyCallBack;
	unsigned	short	int		stepFailMaxTimes;
	ADNetStepFailCallBack		stepFailCallBack;
#if ADNet_config_EnStepTiming == 1
	uint32_t	MinTimingStepTime ;
#endif
	uint8_t isLock;

}
ADNet;
/**************************************************************************************/
/*创建一个网络*/
void *ADNetCreat(void);


/*创建一个网络并指定步骤长度*/
/**
 *@lenght				:	网络中步骤的长度
*/
void *ADNetCreatWithLenght(unsigned short int lenght);


/*使用静态内存创建一个网络并指定步骤长度*/
/**
 *@lenght				:	网络中步骤的长度
 *@*S					:	步骤函数数组
*/
void *ADNetCreatWithLenghtStatic(unsigned short int lenght,ADNetStep *S);


//设置步骤优先级
/*设置空步骤时回调*/
/*设置自动重发*/
/**
 *@ADNets				:	网络
 *@*f[]					:	步骤函数数组
 *@lenght				:	步骤的长度
*/
char ADNetSetStepPrio(ADNet *ADNets ,void *f[],unsigned short int lenght);

//步骤置1
/**
 *@ADNets				:	网络
 *@f					:	步骤函数数组
 *@Prio					:	步骤的优先级
 *@para					:	传入步骤的参数
 *@...					:	当para不为NULL时，需要传入参数的长度。
*/
unsigned int ADNetSetStep(ADNet *ADNets ,void *f,unsigned char Prio,...);
//步骤定时置1
unsigned int ADNetSetTimingStep(ADNet *ADNets ,void *f,unsigned char Prio,uint32_t readyTime);
//步骤置1
/**
 *@ADNets				:	网络
 *@f					:	步骤函数数组
 *@Prio					:	步骤的优先级
 *@ParaType				:	传入步骤的参数的类型
 *@para					:	传入参数。
*/
unsigned int S_ADNetSetStepWithPara(ADNet *ADNets ,void *f,unsigned char Prio,uint8_t ParaType,fpUnion para,...);
//步骤置1
unsigned int S_ADNetSetTimingStepWithPara(ADNet *xADNet ,void *f,unsigned char Prio,uint32_t readyTime,uint8_t ParaType,fpUnion para,...);
/*上锁*/
void ADNetLock(ADNet *xADNet);
/*解锁*/
void ADNetUnLock(ADNet *xADNet);
uint8_t ADNetGetLockState(ADNet *xADNet);
/*设置空步骤时回调*/
/*设置自动重发*/
/**
 *@ADNets				:	网络
 *@f1					:	网络空闲时调用
 *@f2					:	网络从空闲到不空闲转变时调用
*/
void ADNetSetIdleAndReadyCallBack(ADNet *ADNets,void *f1,void *f2);

/*设置最大发送失败次数和回调函数*/
/**
 *@ADNets				:	网络
 *@times				:	次数
 *@stepFailCallBack		:	发送失败超过设定的次数回调
*/
unsigned char ADNetSetStepFailTimes(ADNet *xADNet,unsigned short int times,void  *stepFailCallBack);

/*设置自动重发*/
/**
 *@ADNets				:	网络
 *@f					:	步骤函数
 *@ReSendTimes			:	重发次数
 *@ReSendPeriod			:	重发间隔
 *@ReSendFailCallBack	:	重发失败回调
*/
void ADNetSetResend(ADNet *xADNet,void *f,unsigned int ReSendTimes,unsigned int reSendPeriod,void *reSendFailCallBack);
#if ADNet_config_UseMacroAPI == 0
	/*检查步骤是否为空*/
	uint8_t ADNetIsIdle(ADNet *xADNet);
#else
	#define	ADNetIsIdle(xADNet) ((ADNet *)xADNet)->isIdle
#endif

/*循环执行步骤*/
/**
 *@f xADNet				:	网络
*/
uint32_t ADNetRun(ADNet *xADNet);

/*获取步骤的id*/
uint32_t ADNetGetStepId(ADNet *xADNet);

/*执行重发机制*/
void ADNetResendRun(ADNet *xADNet);

/*获取重发计数*/
/**
 *@ADNets				:	网络
 */
unsigned int ADNetGetResendCnt(ADNet *xADNet);
/*停止全部重发*/
/**
 *@ADNets				:	网络
 *@f					:	步骤函数
 */
void ADNetResendStopAll(ADNet *xADNet);

/*停止重发*/
/**
 *@ADNets				:	网络
 *@f					:	步骤函数
 */
void ADNetResendStop(ADNet *xADNet,void *f);
/*开始重发*/
/**
 *@ADNets				:	网络
 *@f					:	步骤函数
 */
void ADNetResendStart(ADNet *ADNets,void *f,uint8_t yn);

/*生成重发标志算法*/
void __ADNetGenerateResendFlag(ADNet *xADNet);


//获取某一步骤的状态
unsigned char ADNetGetStepStatus(ADNet *ADNets ,void *f);
//复位所有步骤
void ADNetResetAllStep(ADNet *xADNet);
/*根据步骤函数对步骤情况*/
char ADNetResetStep(ADNet *xADNet ,void *f,...);
/*下一次执行的时间*/
uint32_t ADNetGetNextResendTime(ADNet *xADNet);
/*查看步骤是否开启重发*/
uint8_t ADNetGetStepIsResendStart(ADNet *xADNet,void *f);
#endif
