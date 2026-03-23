#include "ADDebug.h"

/*
*V1.0.1_2023-03-07-1:第一版
*V1.0.1_2023-04-12-1:修改没有成功去回车换行的BUG
*V1.0.1_2023-05-04-1:修改函数ADDebugLoop名为ADDebugRun
*V1.0.1_2023-06-02-1:添加ADDebug_SpecificMode类型
*V1.0.1_2023-06-09-1:修改可以通过外部设置指令长度
*V1.0.1_2023-06-12-1:修改ADDebugRun，修改回调时，传入的参数不对问题。
*V1.0.1-2023-08-18-1:修改已满足安电库的规定。
*V1.0.1-2023-08-25-1:在ADDebug_ParaMode模式下，输入为空时，取消指令
*V1.0.1-2023-10-30-1:修复在ADDebug_ParaMode下传入参数不对问题
*V1.0.1-2023-12-20-1:ADDebug_SpecificMode模式添加前后回调
*V1.0.1-2024-03-28-1:添加ADDebugPrintf,ADDebugAddInsWithDeclare函数,添加上锁功能。
*V1.0.1-2024-05-30-1:优化ADDebugPrintf函数
*V1.0.2-2024-09-19-1:新增支持多个相同指令。但是相同指令的类型必须相同。
*/

uint8_t ADDebug_isLock = 0;
char *ADDebug_password ="andian@123";
debugInsStruct debugIns[ADDebug_debugInsMaxSize];
uint16_t debugInsArray[ADDebug_sameInsMaxSize];
/*初始化调试*/
void ADDebugInit(void)
{
	uint16_t ii  = 0;
	for(ii = 0;ii<ADDebug_debugInsMaxSize;ii++)
	{
		debugIns[ii].debugInsStr		=	NULL;
		debugIns[ii].debugInsStrLenght	=	0;
		debugIns[ii].debugInsType		=	ADDebug_UnknownMode;
		debugIns[ii].dFunc				=	NULL;
		debugIns[ii].remindStr			=	NULL;
		debugIns[ii].remindStrLenght	=	0;
	}
}
/*添加调试指令*/
uint8_t ADDebugAddIns(uint8_t mode ,const char *debugInsStr,void *f,...)
{
	uint16_t ii  = 0;
	uint8_t *P;
	P = (uint8_t*)(&f);
	for(ii = 0;ii<ADDebug_debugInsMaxSize;ii++)
	{
		if(debugIns[ii].debugInsStr == NULL)
		{
			debugIns[ii].debugInsStr		=	debugInsStr;
			debugIns[ii].debugInsStrLenght	=	strlen(debugInsStr);
			debugIns[ii].debugInsType		=	mode;
			debugIns[ii].dFunc				=	(debugCallback)f;
			if(mode == ADDebug_ControlMode)
			{
				
				debugIns[ii].remindStr			=	NULL;
				debugIns[ii].remindStrLenght	=	0;
			}
			else if((mode == ADDebug_ParaMode) || (mode == ADDebug_SpecificMode))
			{
				debugIns[ii].remindStr	 		= (const char*)(*((uint32_t*)((P+4))));
				debugIns[ii].remindStrLenght	=  strlen(debugIns[ii].remindStr);	
				if(mode == ADDebug_SpecificMode)
				{
					debugIns[ii].preFunc	=	(debugCallback)(*((uint32_t*)((P+8))));
					debugIns[ii].postFunc	=	(debugCallback)(*((uint32_t*)((P+12))));
				}
			}
			return ADDebug_OK;
		}
	}
	
	return ADDebug_Err;
}
/*添加调试指令*/
uint8_t ADDebugAddInsWithDeclare(uint8_t mode ,const char *debugInsStr,void *f,...)
{
	uint16_t ii  = 0;
	uint8_t *P;
	P = (uint8_t*)(&f);
	for(ii = 0;ii<ADDebug_debugInsMaxSize;ii++)
	{
		if(debugIns[ii].debugInsStr == NULL)
		{
			debugIns[ii].debugInsStr		=	debugInsStr;
			debugIns[ii].debugInsStrLenght	=	strlen(debugInsStr);
			debugIns[ii].debugInsType		=	mode;
			debugIns[ii].dFunc				=	(debugCallback)f;
			if(mode == ADDebug_ControlMode)
			{
				
				debugIns[ii].remindStr			=	(const char*)(*((uint32_t*)((P+4))));;
				debugIns[ii].remindStrLenght	=	strlen(debugIns[ii].remindStr);;
			}
			else if((mode == ADDebug_ParaMode) || (mode == ADDebug_SpecificMode))
			{
				debugIns[ii].remindStr	 		= (const char*)(*((uint32_t*)((P+4))));
				debugIns[ii].remindStrLenght	=  strlen(debugIns[ii].remindStr);	
				if(mode == ADDebug_SpecificMode)
				{
					debugIns[ii].preFunc	=	(debugCallback)(*((uint32_t*)((P+8))));
					debugIns[ii].postFunc	=	(debugCallback)(*((uint32_t*)((P+12))));
				}
			}
			return ADDebug_OK;
		}
	}
	
	return ADDebug_Err;
}
uint16_t S_ADDebugGetInsOffest(uint8_t *data,uint16_t len)
{
	uint16_t ii  = 0;
	for(ii = 0;ii<ADDebug_debugInsMaxSize;ii++)
	{
		if(debugIns[ii].debugInsStr!=NULL)
		{
			if(len == debugIns[ii].debugInsStrLenght)
			{
				if(memcmp(debugIns[ii].debugInsStr,data,len) == 0)
				{
					return ii;
				}	
				else 
				{
					if(debugIns[ii].debugInsStr[debugIns[ii].debugInsStrLenght-1] == '#')
					{
						if(memcmp(debugIns[ii].debugInsStr,data,debugIns[ii].debugInsStrLenght-1) == 0)
						{
							return ii;
						}
					}
				}
			}
			else 
			{	
				
				if(debugIns[ii].debugInsStr[debugIns[ii].debugInsStrLenght-1] == '#')
				{
					if(memcmp(debugIns[ii].debugInsStr,data,debugIns[ii].debugInsStrLenght-1) == 0)
					{
						return ii;
					}
				}
			}
			
		}
	}
	return ADDebug_debugInsMaxSize+1;
}




void ADDebugPrintf(void)
{
	uint16_t ii = 0;
	uint16_t yy = 0;
	uint16_t remindStrMaxLenght =0;
	uint16_t blankLenght = 0;
	for(ii = 0;ii<ADDebug_debugInsMaxSize;ii++)
	{
		if(debugIns[ii].debugInsStr!=NULL)
		{
			if(debugIns[ii].remindStrLenght>remindStrMaxLenght)
			{
				remindStrMaxLenght = debugIns[ii].remindStrLenght;
			}
		}
	}
	printf("调试指令:\r\n");
	for(ii = 0;ii<ADDebug_debugInsMaxSize;ii++)
	{
		if(debugIns[ii].debugInsStr!=NULL)
		{
			if(debugIns[ii].remindStrLenght != 0)
			{
				printf("  %.*s",debugIns[ii].remindStrLenght,debugIns[ii].remindStr);
				blankLenght = remindStrMaxLenght-debugIns[ii].remindStrLenght;
				for(yy = 0;yy<blankLenght;yy++)
				{
					printf(" ");
				}
				
				printf(":  %.*s\r\n",debugIns[ii].debugInsStrLenght,debugIns[ii].debugInsStr);
			}		
			else printf("  %.*s\r\n",debugIns[ii].debugInsStrLenght,debugIns[ii].debugInsStr);
		}
	}
}
/*设置密码*/
void ADDebugSetPassword( char *password)
{
	ADDebug_password = password;
}
/*上锁*/
void ADDebugLock(void)
{
	ADDebug_isLock = 1;	
}




uint16_t S_ADDebugGetInsOffest2(uint8_t *data,uint16_t len,uint16_t *destIns)
{
	uint16_t ii  = 0;
	uint16_t sameInsCnt = 0;
	for(ii = 0;ii<ADDebug_debugInsMaxSize;ii++)
	{
		if(debugIns[ii].debugInsStr!=NULL)
		{
			if(len == debugIns[ii].debugInsStrLenght)
			{
				if(memcmp(debugIns[ii].debugInsStr,data,len) == 0)
				{
					destIns[sameInsCnt] = ii;
					sameInsCnt++;
					
				}	
				else 
				{
					if(debugIns[ii].debugInsStr[debugIns[ii].debugInsStrLenght-1] == '#')
					{
						if(memcmp(debugIns[ii].debugInsStr,data,debugIns[ii].debugInsStrLenght-1) == 0)
						{
							destIns[sameInsCnt] = ii;
					sameInsCnt++;
						}
					}
				}
			}
			else 
			{	
				
				if(debugIns[ii].debugInsStr[debugIns[ii].debugInsStrLenght-1] == '#')
				{
					if(memcmp(debugIns[ii].debugInsStr,data,debugIns[ii].debugInsStrLenght-1) == 0)
					{
						destIns[sameInsCnt] = ii;
					sameInsCnt++;
					}
				}
			}
			
		}
	}
	return sameInsCnt;
}

uint8_t ADDebugRun(uint8_t *data,uint16_t len)
{
	static ADDebugRunInsState nowStatus = ADDebugRunState_Init;

	uint16_t offest  = 0;
	uint16_t destStrLenght = len;
	static uint16_t InsCnt = 0;
	if(ADDebug_isLock == 1)
	{
		if(strlen(ADDebug_password) == len)
		{
			if(memcmp(data,ADDebug_password,len) == 0)
			{
				ADDebug_isLock = 0;
				printf("解锁成功\r\n");
			}
			else return ADDebug_Err;
		}else return ADDebug_Err;
		
	}
	if((data[len-2]==0xd)&&(data[len-1]==0xa))
	{
		destStrLenght = len - 2;
	}
	else if(data[len-1]==0xd)
	{
		destStrLenght = len - 1;
	}
	if(nowStatus == ADDebugRunState_Init)
	{				
		/*找到指令的位置*/
		InsCnt	=	S_ADDebugGetInsOffest2(data,destStrLenght,debugInsArray); 
		if(InsCnt>0)
		{
			for(uint16_t ii = 0;ii<InsCnt;ii++)
			{
				offest = debugInsArray[ii];
				switch(debugIns[offest].debugInsType)
				{
					case ADDebug_ControlMode:
					{
						debugIns[offest].dFunc(data,len);
					}break;
					case ADDebug_ParaMode:
					{
						printf("%.*s\r\n",debugIns[offest].remindStrLenght,debugIns[offest].remindStr);
						nowStatus	=	ADDebugRunState_WaitData;											
					}break;
					case ADDebug_FileMode:
					{
					
					}break;
					case ADDebug_CalibrationMode:
					{
						printf("Enter CalibrationMode Succss:OK\r\n");
						nowStatus	=	ADDebugRunState_WaitData;
						
					}break;
					case ADDebug_SpecificMode:
					{
						printf("进入%.*s模式\r\n",debugIns[offest].remindStrLenght,debugIns[offest].remindStr);
						nowStatus = ADDebugRunState_WaitData;
					
						if(debugIns[offest].preFunc!=NULL)
						{
							debugIns[offest].preFunc(data,len);
						}
					}break;
				}
			}
			
		}
	}
	else 
	{
	//	printf("InsCnt=%d\r\n",InsCnt);
		for(uint16_t ii = 0;ii<InsCnt;ii++)
		{
			offest = debugInsArray[ii];
			switch(debugIns[offest].debugInsType)
			{
					case ADDebug_ParaMode:
					{
						if(debugIns[offest].dFunc!=NULL)
						{
							if(destStrLenght!=0)
							debugIns[offest].dFunc(data,destStrLenght);
							else printf("指令取消\r\n");
						}
						nowStatus = ADDebugRunState_Init;
					}break;
					case ADDebug_SpecificMode:
					{
						if(memcmp(debugIns[offest].debugInsStr,data,destStrLenght) == 0)
						{
						
							printf("退出%.*s模式\r\n",debugIns[offest].remindStrLenght,debugIns[offest].remindStr);							
							if(debugIns[offest].postFunc!=NULL)
							{
								debugIns[offest].postFunc(data,len);
							}
							nowStatus = ADDebugRunState_Init;
						}
						else 
						{
							if(debugIns[offest].dFunc!=NULL)
							debugIns[offest].dFunc(data,len);
						}
					}break;
					}
		}
	}
	return ADDebug_OK;
}




//uint8_t ADDebugRun(uint8_t *data,uint16_t len)
//{
//	static uint8_t nowStatus = ADDebug_UnknownMode;
//	static debugInsStruct	*lastIns;
//	uint16_t offest  = 0;
//	uint16_t destStrLenght = len;
//	if(ADDebug_isLock == 1)
//	{
//		if(strlen(ADDebug_password) == len)
//		{
//			if(memcmp(data,ADDebug_password,len) == 0)
//			{
//				ADDebug_isLock = 0;
//				printf("解锁成功\r\n");
//			}
//			else return ADDebug_Err;
//		}else return ADDebug_Err;
//		
//	}
//	if((data[len-2]==0xd)&&(data[len-1]==0xa))
//	{
//		destStrLenght = len - 2;
//	}
//	else if(data[len-1]==0xd)
//	{
//		destStrLenght = len - 1;
//	}
//	if(nowStatus == ADDebug_UnknownMode)
//	{				
//		/*找到指令的位置*/
//		offest	=	S_ADDebugGetInsOffest(data,destStrLenght); 
//		if(offest<ADDebug_debugInsMaxSize)
//		{
//			switch(debugIns[offest].debugInsType)
//			{
//				case ADDebug_ControlMode:
//				{
//					debugIns[offest].dFunc(data,len);
//				}break;
//				case ADDebug_ParaMode:
//				{
//					if(nowStatus	==	ADDebug_UnknownMode)
//					{
//						printf("%.*s\r\n",debugIns[offest].remindStrLenght,debugIns[offest].remindStr);
//						nowStatus	=	ADDebug_ParaMode;
//						lastIns 	= 	&debugIns[offest];
//					}
//					else 
//					{
//						lastIns->dFunc(data,len);
//						nowStatus	=	ADDebug_UnknownMode;
//						lastIns		=	NULL;
//					}
//				}break;
//				case ADDebug_FileMode:
//				{
//				
//				}break;
//				case ADDebug_CalibrationMode:
//				{
//					printf("Enter CalibrationMode Succss:OK\r\n");
//					nowStatus	=	ADDebug_CalibrationMode;
//					lastIns 	= 	&debugIns[offest];
//				}break;
//				case ADDebug_SpecificMode:
//				{
//					printf("进入%.*s模式\r\n",debugIns[offest].remindStrLenght,debugIns[offest].remindStr);
//					nowStatus = ADDebug_SpecificMode;
//					lastIns 	= 	&debugIns[offest];
//					if(debugIns[offest].preFunc!=NULL)
//					{
//						debugIns[offest].preFunc(data,len);
//					}
//				}break;
//			}
//		}
//	}
//	else
//	{
//		switch(nowStatus)
//		{
//			case ADDebug_ControlMode:
//			{
//				
//			}break;
//			case ADDebug_ParaMode:
//			{	
//				if(lastIns!=NULL)
//				{
//					if(destStrLenght!=0)
//					lastIns->dFunc(data,destStrLenght);
//					else printf("指令取消\r\n");
//				}
//				
//				nowStatus	=	ADDebug_UnknownMode;
////				lastIns		=	NULL;				
//			}break;
//			case ADDebug_FileMode:
//			{
//				lastIns->dFunc(data,len);
//			}break;
//			case ADDebug_CalibrationMode:
//			{
//			
//			}break;
//			case ADDebug_SpecificMode:
//			{
//				if(memcmp(lastIns->debugInsStr,data,destStrLenght) == 0)
//				{
//				
//					printf("退出%.*s模式\r\n",lastIns->remindStrLenght,lastIns->remindStr);
//					nowStatus = ADDebug_UnknownMode;
//					if(lastIns->postFunc!=NULL)
//					{
//						lastIns->postFunc(data,len);
//					}
//				}
//				else 
//				{
//					if(lastIns!=NULL)
//					lastIns->dFunc(data,len);
//				}
//			}break;
//		}
//	}
//	return ADDebug_OK;
//}

