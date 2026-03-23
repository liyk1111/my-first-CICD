#include "ADKey.h"


/*
*V1.0.1_2023-06-09-1:修改可以通过外部设置按键数量
*V1.0.1_2023-06-15-1:添加ADKeyInit。修复多按键同时连按可能出现的BUG
*V1.0.1_2023-08-07-1:添加ADKeyRun返回,如果ADKeyRun仍然需要被执行，便返回ADKey_IsPressed，否则返回ADKey_IsRelease。
*V1.0.1_2023-08-16-1:修改宏定义YES和NO为ADKey_IsPressed和ADKey_IsRelease。
*V1.0.1_2023-10-07-1:修改ADKeyRun，使得编译不报错。
*V1.0.1_2023-11-07-1:添加ADKeyStatusIsChange功能
*V2.0.0_2023-11-10-1:添加可以配置消抖次数，消抖时间，以及长按时间的功能。
*V2.0.0_2023-11-13-1:修复短按消抖次数Bug.
*V2.0.1_2024-03-15-1:新增判断为连按的时间间隔可配置.
*/
#ifndef ADKey_config_KeyNum
	#define ADKey_config_KeyNum 1
#endif
KeyStruct Key[ADKey_config_KeyNum];

#if ADKey_config_KeyIsShortPressedCallback == 1
	KeyIsShortPressedCallbackFunc		ADKeyIsShortPressedCallback = NULL;	/*短按*/
#endif
#if ADKey_config_KeyIsLongPressedCallback == 1
	KeyIsLongPressedCallbackFunc		ADKeyIsLongPressedCallback = NULL;	/*长按*/
#endif
#if ADKey_config_KeyBingLongPressedCallback == 1	
	KeyBingLongPressedCallbackFunc		ADKeyBingLongPressedCallback = NULL;	/*正在被长按*/
#endif	
#if ADKey_config_KeyIsMulPressedCallback == 1
	KeyIsMulPressedCallbackFunc			ADKeyIsMulPressedCallback = NULL;		/*按键被连击回调*/
#endif
#if ADKey_config_KeyStatusChangeCallback == 1
	KeyStatusChangeCallbackFunc		ADKeyStatusIsChange = NULL;
#endif
ADKeyTickFunc  ADKey_GetTick = NULL;

/*初始化ADKey*/
void ADKeyInit(ADKeyTickFunc f)
{
	uint32_t ii = 0;
	ADKey_GetTick = f;
	for(ii = 0;ii<ADKey_config_KeyNum;ii++)
	{
		Key[ii].KeyDetection 				= 	NULL;
		Key[ii].KeyHaveBeenPressedFlag 		= 	ADKey_IsRelease;
		Key[ii].KeyIsPressed 				= 	ADKey_IsRelease;
		Key[ii].KeyIsTentative	 			= 	ADKey_IsRelease;		
		Key[ii].KeyPressedTime 				= 	0;
		Key[ii].KeyStartPressedTime 		= 	0;
#if ADKey_config_KeyIsMulPressedCallback == 1			
		Key[ii].LongPressedCnt 				= 	0;
		Key[ii].MulPressedStartJudgeTime 	= 	0;
		Key[ii].MulPressedJudgeFlag 		= 	ADKey_IsRelease;
		Key[ii].MulPressedCnt 				= 	0;
#endif	
	
#if ADKey_config_Para == 1			
		Key[ii].Para						=	NULL;
#endif	
	
	}
}
#if ADKey_config_Para == 1	
	void ADKeySetPara(uint8_t xKey,void *p)
	{
		if(xKey<ADKey_config_KeyNum)
		{
			Key[xKey].Para = p;
		}
	}
#endif

void S_ADKeyReset(KeyStruct *xKey)
{	
	xKey->KeyIsPressed 					= 	ADKey_IsRelease;
	xKey->KeyIsTentative	 			= 	ADKey_IsRelease;
	xKey->StartScanTick 				=	0;
#if ADKey_config_Para == 1	
	xKey->VibrationEliminateCnt		=   0;
#endif	
}
/*按键扫描*/
/*检测完成，便返回ADKey_IsPressed，否则就返回ADKey_IsRelease*/
ADKeyRunState S_ADKeyScan(void)
{
	uint16_t ii = 0;
	ADKeyRunState state = ADKey_continue;							/*标记是都需要继续检测按键*/
	for(ii = 0;ii<ADKey_config_KeyNum;ii++)
	{
		if(Key[ii].KeyDetection!=NULL)
		{
			if((Key[ii].KeyIsTentative  == ADKey_IsPressed)&&(Key[ii].KeyIsPressed == ADKey_IsRelease))			/*可能有被被按下了*/
			{
#if ADKey_config_Para == 1
				if(Key[ii].Para!=NULL)
				{
					if(ADKey_GetTick()-Key[ii].StartScanTick>=Key[ii].Para->VibrationEliminateTick)		/*消抖时间到了*/
					{
						Key[ii].KeyIsTentative =  Key[ii].KeyDetection();
						if(Key[ii].KeyIsTentative  == ADKey_IsPressed)
						{
							Key[ii].VibrationEliminateCnt++;						    
							Key[ii].StartScanTick = ADKey_GetTick();		/*更新上一次被按下的时间*/
							if(Key[ii].VibrationEliminateCnt>=Key[ii].Para->VibrationEliminateTimes)		/*消抖次数到了*/
							{
								Key[ii].KeyIsPressed = ADKey_IsPressed;
								Key[ii].VibrationEliminateCnt = 0;
							}
						}
						else 																				/*没有继续被按下，只是抖动而已*/
						{
							S_ADKeyReset(&Key[ii]);
						}						
					}
				}
				else
				{
					if(ADKey_GetTick()-Key[ii].StartScanTick>=20)		/*消抖时间到了*/
					{
						Key[ii].KeyIsTentative =  Key[ii].KeyDetection();
						if(Key[ii].KeyIsTentative  == ADKey_IsPressed)
						{							
							Key[ii].KeyIsPressed = ADKey_IsPressed;						
						}
						else 																				/*没有继续被按下，只是抖动而已*/
						{
							S_ADKeyReset(&Key[ii]);
						}
					}
				} 				
#else 
				if(ADKey_GetTick()-Key[ii].StartScanTick>=20)		/*消抖时间到了*/
				{
					Key[ii].KeyIsTentative =  Key[ii].KeyDetection();
					if(Key[ii].KeyIsTentative  == ADKey_IsPressed)
					{							
						Key[ii].KeyIsPressed = ADKey_IsPressed;						
					}
					else 																				/*没有继续被按下，只是抖动而已*/
					{
						S_ADKeyReset(&Key[ii]);
					}
				}
#endif			
			}
			else 
			{
				if(Key[ii].KeyIsPressed == ADKey_IsPressed)
				{
					if(Key[ii].KeyDetection()!=ADKey_IsPressed)
					{
						S_ADKeyReset(&Key[ii]);
					}
				}
				else 
				{
					Key[ii].KeyIsTentative =  Key[ii].KeyDetection();
					if(Key[ii].KeyIsTentative  == ADKey_IsPressed)
					{
						Key[ii].StartScanTick = ADKey_GetTick();
						
					}					
				}								
			}								
		}			
	}
	return state;	
}
#if ADKey_config_KeyIsMulPressedCallback == 1	
void S_ADKeyJudgeIsMulPressed(void)
{
	uint16_t ii = 0;
	uint32_t temp32 = 800;
	/*检查是否有需要判断被连按的按键*/
	for(ii = 0;ii<ADKey_config_KeyNum;ii++)
	{
		if(Key[ii].MulPressedJudgeFlag == ADKey_IsPressed)
		{
#if ADKey_config_KeyDoubleClickMaxTick == 1 && ADKey_config_Para == 1
			if(Key[ii].Para != NULL)
			{
				temp32 = Key[ii].Para->DoubleClickMaxTick;
			}
			else temp32 = 800;
			if(ADKey_GetTick() - Key[ii].MulPressedStartJudgeTime>temp32)
#else
			if(ADKey_GetTick() - Key[ii].MulPressedStartJudgeTime>800)
#endif
			{						
				if(Key[ii].MulPressedCnt!=0)
				{
					if(ADKeyIsMulPressedCallback!=NULL&&Key[ii].MulPressedCnt>1)
					ADKeyIsMulPressedCallback(ii,Key[ii].MulPressedCnt,Key[ii].KeyPressedTime);
					Key[ii].MulPressedCnt = 0;
				}			
				Key[ii].MulPressedJudgeFlag = ADKey_IsRelease;
			
			}			
		}
	}			
}
#endif
/*如果ADKeyRun仍然需要被执行，便返回ADKey_IsPressed，否则返回ADKey_IsRelease*/
ADKeyRunState ADKeyRun(void)
{
	uint16_t ii = 0;
	ADKeyRunState StillNeedsToBeExecuted = ADKey_stop;

	if(ADKey_GetTick == NULL)
	return ADKey_stop;
	
	 S_ADKeyScan(); 
					
		for(ii = 0;ii<ADKey_config_KeyNum;ii++)
		{						
			if(Key[ii].KeyIsPressed == ADKey_IsPressed)
			{	
								
				if(Key[ii].KeyHaveBeenPressedFlag  == ADKey_IsRelease)		/*记录开始被按下的时间*/
				{
					Key[ii].KeyStartPressedTime = ADKey_GetTick();
#if ADKey_config_KeyStatusChangeCallback == 1					
					if(ADKeyStatusIsChange != NULL)
					{
						ADKeyStatusIsChange(ii,ADKey_IsPressed);
					}
#endif					
				}
				else 
				{	
#if ADKey_config_KeyBingLongPressedCallback == 1					
					if(ADKeyBingLongPressedCallback!=NULL)
					{
#if ADKey_config_Para  == 1
						if(Key[ii].Para!=NULL)
						{
							if((ADKey_GetTick()-Key[ii].KeyStartPressedTime)/Key[ii].Para->LongPressedTick==Key[ii].LongPressedCnt+1)
							{									
								Key[ii].LongPressedCnt++;
								ADKeyBingLongPressedCallback(ii,Key[ii].LongPressedCnt);									
							}
						}
						else 
						{
							if((ADKey_GetTick()-Key[ii].KeyStartPressedTime)/ADKey_LongPressTime==Key[ii].LongPressedCnt+1)
							{									
								Key[ii].LongPressedCnt++;
								ADKeyBingLongPressedCallback(ii,Key[ii].LongPressedCnt);									
							}
						}
#else 					
						if((ADKey_GetTick()-Key[ii].KeyStartPressedTime)/ADKey_LongPressTime==Key[ii].LongPressedCnt+1)
						{									
							Key[ii].LongPressedCnt++;
							ADKeyBingLongPressedCallback(ii,Key[ii].LongPressedCnt);									
						}
#endif						
					}
#endif					
				}
				Key[ii].KeyHaveBeenPressedFlag = ADKey_IsPressed;/*记录被按下了*/
			}
			else 
			{
				if(Key[ii].KeyHaveBeenPressedFlag == ADKey_IsPressed)
				{
					Key[ii].KeyHaveBeenPressedFlag = ADKey_IsRelease;
					Key[ii].LongPressedCnt  = 0;
					/*记录按下次数和按下时间*/															
					Key[ii].KeyPressedTime = ADKey_GetTick()-Key[ii].KeyStartPressedTime;
					
#if ADKey_config_Para == 1					
					if(Key[ii].Para!=NULL)
					{
	#if ADKey_config_KeyIsLongPressedCallback == 1
						/*检查是否为长按*/
						if(ADKeyIsLongPressedCallback!=NULL)
						{
							if(Key[ii].KeyPressedTime>=Key[ii].Para->LongPressedTick)
							{
								ADKeyIsLongPressedCallback(ii,Key[ii].KeyPressedTime);
							}
						}						
	#endif		
	#if ADKey_config_KeyIsShortPressedCallback == 1	
						/*检查是否为短按*/
						if(ADKeyIsShortPressedCallback!=NULL)
						{
							if(Key[ii].KeyPressedTime<Key[ii].Para->LongPressedTick) 
							{								
								ADKeyIsShortPressedCallback(ii);					
							}
						}						
	#endif			
					}
					else 
					{
	#if ADKey_config_KeyIsLongPressedCallback == 1
						/*检查是否为长按*/
						if(ADKeyIsLongPressedCallback!=NULL)
						{
							if(Key[ii].KeyPressedTime>=ADKey_LongPressTime)
							{
								ADKeyIsLongPressedCallback(ii,Key[ii].KeyPressedTime);
							}
						}						
	#endif		
	#if ADKey_config_KeyIsShortPressedCallback == 1	
						/*检查是否为短按*/
						if(ADKeyIsShortPressedCallback!=NULL)
						{
							if(Key[ii].KeyPressedTime<ADKey_LongPressTime) 
							{								
								ADKeyIsShortPressedCallback(ii);					
							}
						}						
	#endif				
					}

#else	
	#if ADKey_config_KeyIsLongPressedCallback == 1
						/*检查是否为长按*/
						if(ADKeyIsLongPressedCallback!=NULL)
						{
							if(Key[ii].KeyPressedTime>=ADKey_LongPressTime)
							{
								ADKeyIsLongPressedCallback(ii,Key[ii].KeyPressedTime);
							}
						}						
	#endif		
	#if ADKey_config_KeyIsShortPressedCallback == 1	
						/*检查是否为短按*/
						if(ADKeyIsShortPressedCallback!=NULL)
						{
							if(Key[ii].KeyPressedTime<ADKey_LongPressTime) 
							{								
								ADKeyIsShortPressedCallback(ii);					
							}
						}						
	#endif

#endif
					
#if ADKey_config_KeyIsMulPressedCallback == 1	
					Key[ii].MulPressedCnt++;
					Key[ii].MulPressedStartJudgeTime = ADKey_GetTick();					
					Key[ii].MulPressedJudgeFlag = ADKey_IsPressed;	
#endif	
											
					
#if ADKey_config_KeyStatusChangeCallback == 1					
					if(ADKeyStatusIsChange != NULL)
					{
						ADKeyStatusIsChange(ii,ADKey_IsRelease);
					}
#endif					
				}
			}
		}
#if ADKey_config_KeyIsMulPressedCallback == 1		
		S_ADKeyJudgeIsMulPressed();		
#endif		
		/*这里是为判断，是否还有按键需要等待判断有没有被连按*/
	for(ii = 0;ii<ADKey_config_KeyNum;ii++)
	{
#if ADKey_config_KeyIsMulPressedCallback == 1	
		if(Key[ii].MulPressedJudgeFlag == ADKey_IsPressed)
		{
			StillNeedsToBeExecuted = ADKey_continue;
			break;
		}
#endif		
		if(Key[ii].KeyHaveBeenPressedFlag == ADKey_IsPressed||Key[ii].KeyIsTentative == ADKey_IsPressed)/*只要还有一个按键需要等待判断是否被连按，那就返回*/
		{
			StillNeedsToBeExecuted = ADKey_continue;
			break;
		}
	}
	return StillNeedsToBeExecuted ;
			
}

/*添加按钮*/
uint16_t  ADKeyAdd(uint8_t xKey,KeyDetectionFunc f1)
{
	if(xKey<ADKey_config_KeyNum)
	{
		Key[xKey].KeyDetection = f1;
		Key[xKey].KeyHaveBeenPressedFlag = ADKey_IsRelease;
		Key[xKey].KeyIsPressed = ADKey_IsRelease;
		Key[xKey].KeyIsTentative = ADKey_IsRelease;		
		Key[xKey].KeyPressedTime = 0;
		Key[xKey].KeyStartPressedTime = 0;
		Key[xKey].LongPressedCnt = 0;
		
#if ADKey_config_KeyIsMulPressedCallback == 1			
		Key[xKey].MulPressedStartJudgeTime = 0;
		Key[xKey].MulPressedJudgeFlag = ADKey_IsRelease;
		Key[xKey].MulPressedCnt = 0;
#endif		
	}
	return 0xffff;
}
/*
	f1:单次短按回调
	f2:单次长按回调
	f3:长按中回调
	f4:连续按下回调
*/
void ADKeySetCallback(KeyIsShortPressedCallbackFunc f1,KeyIsLongPressedCallbackFunc f2,KeyBingLongPressedCallbackFunc f3,KeyIsMulPressedCallbackFunc f4)
{
#if ADKey_config_KeyIsShortPressedCallback == 1
	ADKeyIsShortPressedCallback = 		f1;		/*短按*/
#endif	
#if ADKey_config_KeyIsLongPressedCallback == 1
	ADKeyIsLongPressedCallback = 		f2;		/*长按*/
#endif

#if ADKey_config_KeyBingLongPressedCallback == 1	
	ADKeyBingLongPressedCallback = 		f3;		/*按键正在被长按回调*/
#endif

#if ADKey_config_KeyIsMulPressedCallback == 1	
	ADKeyIsMulPressedCallback = 		f4;		/*按键被连击回调*/
#endif 	
}

#if ADKey_config_KeyStatusChangeCallback == 1
	void ADKeySetStatusChangeCallback(KeyStatusChangeCallbackFunc f1)
	{
		ADKeyStatusIsChange = f1;
	}
#endif

#if ADKey_config_KeyIsShortPressedCallback == 1
	void ADKeySetShortPressedCallback(KeyIsShortPressedCallbackFunc f1)
	{
		ADKeyIsShortPressedCallback = f1;
	}
#endif

#if ADKey_config_KeyIsLongPressedCallback == 1
	void ADKeySetLongPressedCallback(KeyIsLongPressedCallbackFunc f1)
	{
		ADKeyIsLongPressedCallback = f1;		/*长按*/
	}
#endif

#if ADKey_config_KeyBingLongPressedCallback == 1
	void ADKeySetBingLongPressedCallback(KeyBingLongPressedCallbackFunc f1)
	{
		ADKeyBingLongPressedCallback = 		f1;		/*按键正在被长按回调*/
	}
#endif	

#if ADKey_config_KeyIsMulPressedCallback == 1
	void ADKeySetMulPressedCallback(KeyIsMulPressedCallbackFunc f1)
	{
		ADKeyIsMulPressedCallback = 		f1;		/*按键被连击回调*/
	}
#endif	
