#ifndef __ADKEY_H
#define __ADKEY_H
#include "stdint.h"
#include "stddef.h"

/************************************************************************************************/
/*配置项*/
#define ADKey_config_KeyNum						5				/*按键数量*/
#define ADKey_config_Para						1				/*是否允许配置参数，例如：长按时间，消抖时间*/
#define ADKey_config_KeyStatusChangeCallback	1				/*是否开启按键状态改变回调*/
#define ADKey_config_KeyIsShortPressedCallback	1				/*是否开启按键短按回调*/
#define ADKey_config_KeyIsLongPressedCallback	1				/*是否开启按键长按回调*/
#define ADKey_config_KeyBingLongPressedCallback	1				/*是否开启按键正在被长按按回调*/
#define ADKey_config_KeyIsMulPressedCallback	1				/*是否开启按键连按回调*/
#define ADKey_config_KeyDoubleClickMaxTick		1				/*是否开启连设置“判定为连续按下的最大间隔时间”*/
/************************************************************************************************/
/*以下内容不要修改*/


#define ADKey_LongPressTime			2000			/*长按的时长，单位：毫秒*/
#define ADKey_IsPressed				1				/*按键被按下*/
#define ADKey_IsRelease				0				/*按键没有被按下*/

#define  ADKey_YES					1
#define  ADKey_NO					0

typedef enum
{
	ADKey_stop=0,
    ADKey_continue
} ADKeyRunState;



typedef uint8_t (*KeyDetectionFunc)(void);
typedef uint8_t (*KeyIsShortPressedCallbackFunc)(uint8_t xKey);													//短按	
typedef uint8_t (*KeyIsLongPressedCallbackFunc)(uint8_t xKey,uint32_t IsPressedTime);							//长按
typedef uint8_t (*KeyBingLongPressedCallbackFunc)(uint8_t xKey,uint32_t LongPressedCnt);						/*按键正在被长按回调*/
												
typedef uint8_t (*KeyIsMulPressedCallbackFunc)(uint8_t xKey,uint16_t IsPressedCnt,uint32_t IsPressedTime);		/*按键被连击回调*/
typedef uint8_t (*KeyStatusChangeCallbackFunc)(uint8_t xKey,uint8_t status);		
typedef uint32_t (*ADKeyTickFunc)(void);
	
/*初始化ADKey*/
void ADKeyInit(ADKeyTickFunc f);
#if ADKey_config_Para == 1
	typedef struct
	{		
		uint8_t VibrationEliminateTimes;			/*消抖次数*/	
		uint16_t VibrationEliminateTick;			/*消抖时间*/
		uint32_t LongPressedTick;					/*长按时间*/
#if ADKey_config_KeyDoubleClickMaxTick == 1
		uint32_t DoubleClickMaxTick;				/*判定为连续按下的最大间隔时间*/
#endif
	}
	ADKeyPara;
#endif
typedef struct
{
	uint32_t			StartScanTick;			/*第一次被按下的时间，还没有消抖*/
	uint8_t 			KeyIsTentative;			/*第一次检测到被按下*/
	uint8_t 			KeyIsPressed;			/*确定被按下了*/
	uint8_t				KeyHaveBeenPressedFlag;	/*被按下过*/
	KeyDetectionFunc	KeyDetection;			/*按键扫描函数*/
	
	
	uint32_t						KeyPressedTime;			/*被按下的时间间隔*/	
	uint32_t						KeyStartPressedTime;	/*开始被按下的时间*/
	
#if ADKey_config_KeyIsMulPressedCallback == 1		
	uint32_t 						MulPressedStartJudgeTime;/*开始判断是否连按的起始时间*/		
	uint16_t 						MulPressedCnt;			/*被按下次数*/
	uint8_t							MulPressedJudgeFlag;	/*判断是否连按标志*/
#endif
	uint8_t 						LongPressedCnt;			/*长按次数*/
	
#if ADKey_config_Para == 1	
	uint8_t VibrationEliminateCnt;				/*消抖计数*/	
	ADKeyPara						*Para;
#endif	
}
KeyStruct;




/*按键轮询*/
ADKeyRunState ADKeyRun(void);

/*添加按钮*/
uint16_t  ADKeyAdd(uint8_t num,KeyDetectionFunc f1);
/*设置回调*/
void ADKeySetCallback(KeyIsShortPressedCallbackFunc f1,KeyIsLongPressedCallbackFunc f2,KeyBingLongPressedCallbackFunc f3,KeyIsMulPressedCallbackFunc f4);
#if ADKey_config_KeyStatusChangeCallback == 1
	void ADKeySetStatusChangeCallback(KeyStatusChangeCallbackFunc f1);
#endif
#if ADKey_config_Para == 1	
	void ADKeySetPara(uint8_t xKey,void *p);
#endif	


#if ADKey_config_KeyIsShortPressedCallback == 1
	void ADKeySetShortPressedCallback(KeyIsShortPressedCallbackFunc f1);
#endif

#if ADKey_config_KeyIsLongPressedCallback == 1
	void ADKeySetLongPressedCallback(KeyIsLongPressedCallbackFunc f1);
#endif

#if ADKey_config_KeyBingLongPressedCallback == 1
	void ADKeySetBingLongPressedCallback(KeyBingLongPressedCallbackFunc f1);
#endif	

#if ADKey_config_KeyIsMulPressedCallback == 1
	void ADKeySetMulPressedCallback(KeyIsMulPressedCallbackFunc f1);
#endif

	
#endif
