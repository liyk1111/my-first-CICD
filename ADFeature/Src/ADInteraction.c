#include "ADInteraction.h"
#include "stdlib.h"
#include "stdio.h"
/*
	V2.0.0-2023-10-19:V2的第一版
	V2.0.1-2023-10-20:添加交互完成回调等。 
	V2.0.1-2023-10-25:添加函数ADInteractionGetStatus
	V2.0.1-2024-05-23:修复优先级BUG
*/


static void *ADInteractionLinkListHead = NULL;
/*插入一个新的交互*/
uint8_t S_ADInteractionLinkListInsert(ADInteraction *elm)
{
	ADInteraction *p;
	if(ADInteractionLinkListHead == NULL)
	{
		ADInteractionLinkListHead = elm;
	}
	else 
	{
		p = ADInteractionLinkListHead;
		while(p->next!=NULL)
		{
			p = p->next;
		}
		p->next = elm;
	}
	return ADInteraction_OK;

}
void S_ADInteractionRunTask(void *xADInteraction,ADInteractionBodyStruct *body)
{
	//printf("body->Offset=%d\r\n",body->Offset);
	body->InteractionTask[body->Offset].fun();	
	body->Offset++;	
	body->Offset = body->Offset % body->TaskLenght;
	/*等于0，证明已经执行了一轮了*/
	if(body->RunTimes!=0)
	{
		if(body->Offset == 0)
		{
			body->RunCnt++;							/*执行次数加1*/
			if(body->RunCnt>=body->RunTimes)
			{
				body->RunCnt = 0;
				body->isRun = 0;
#if configEnInteractionFinishCallback == 1
				if(((ADInteraction*)xADInteraction)->finishCallback!=NULL)
				((ADInteraction*)xADInteraction)->finishCallback(xADInteraction,body->name);
#endif
			}
		}
	}
}
/*获取需要运行的最大优先级任务*/
uint8_t S_ADInteractionGetHighestPrio(ADInteractionBodyStruct *body,uint8_t bodyNum)
{
	uint8_t ii = 0;
	uint8_t HighestPrio = 0;
	uint8_t HighestPrioOffset = 0xff;
	for(ii = 0;ii<bodyNum;ii++)
	{
		if((HighestPrio<body[ii].Prio)&&(body[ii].isRun == 1))
		{
			HighestPrio = body[ii].Prio;
			HighestPrioOffset = ii;
		}
	}

	return HighestPrioOffset;
}

void ADInteractionPrintf(void)
{
	ADInteraction *p;
	 uint16_t ii = 0;
	
	p = ADInteractionLinkListHead;
	while(p!=NULL)
	{
		printf("交互类型数量:%d\r\n",p->BodyNum);
		for(ii = 0;ii<p->BodyNum;ii++){	
			printf("名称:%d\r\n",p->Body[ii].name);
			printf("  运行:%d\r\n",p->Body[ii].isRun);			
			printf("  优先级:%d\r\n",p->Body[ii].Prio);
			
		}
		printf("\r\n");
		p = p->next;
	}
		
	
}

/*创建一个交互*/
void *ADInteractionCreat(uint8_t num)
{
	ADInteraction *p;
	p = ADInteraction_Malloc(sizeof(ADInteraction));
	if(p == NULL)
	{
		return NULL;
	}
	else 
	{
		p->Body = ADInteraction_Malloc(sizeof(ADInteraction)*num); 
		if(p->Body == NULL)
		{
			ADInteraction_Free(p);
			return NULL;
		}
		else 
		{
			uint8_t ii = 0;
			p->BodyNum = num;
			p->DefaultFun = NULL;
			p->StartTime = ADInteraction_WaitForever;
#if configEnInteractionFinishCallback == 1
			p->finishCallback = NULL;
#endif
			for(ii = 0;ii<p->BodyNum;ii++)
			{
				p->Body[ii].name = 0;
				p->Body[ii].isRun = 0;
				p->Body[ii].RunCnt = 0;
				p->Body[ii].RunTimes = 0;
				p->Body[ii].TaskLenght = 0;
			}
			S_ADInteractionLinkListInsert(p);
			return p;
		}		
	}	
}
/*添加一个交互*/
/*
name :交互名称
prio:优先级，值越小优先级越大。
times:次数。0为无次数。
*/
uint8_t ADInteractionAdd(void *xADInteraction,uint8_t name,uint8_t prio,uint16_t times,const ADInteractionMode *TaskList,uint8_t lenght)
{

	ADInteraction *p;
	uint16_t ii = 0;
	
	p = xADInteraction;
	
	for(ii = 0;ii<p->BodyNum;ii++)
	{
		if(p->Body[ii].TaskLenght == 0)
		{
			p->Body[ii].InteractionTask = 	TaskList;
			p->Body[ii].name 			= 	name;
			p->Body[ii].isRun 			= 	0;
			p->Body[ii].RunCnt 			= 	0;
			p->Body[ii].Offset 			= 	0;
			p->Body[ii].RunTimes 		= 	times;
			p->Body[ii].TaskLenght 		= 	lenght;
			p->Body[ii].Prio			=	prio;
			p->Body[ii].isProtect		=	0;
			break;
		}
	}
	if(ii>=p->BodyNum)
	return ADInteraction_Err;
	else return ADInteraction_OK;
}

uint32_t ADInteractionRun(void)
{
	uint32_t MinTime = ADInteraction_WaitForever;
	ADInteraction *p;
					/*标记交互是否全部不需要执行了*/
	uint8_t which;
	if(ADInteractionLinkListHead == NULL)
	{
		return ADInteraction_WaitForever;
	}
	else 
	{		
		p = ADInteractionLinkListHead;
		while(p != NULL)
		{			
			which = S_ADInteractionGetHighestPrio(p->Body,p->BodyNum);
			if(which == 0xff)
			{
				
			}
			else 
			{
				uint8_t offset = 0;
				uint32_t nowTime = ADInteraction_GetTick();
				
				if(p->Body[which].Offset == 0)
				{
					offset = p->Body[which].TaskLenght-1;
				}
				else offset = p->Body[which].Offset-1;
				
				if((p->StartTime ==0)||(p->StartTime+p->Body[which].InteractionTask[offset].RunTime)<nowTime)
				{
					
					S_ADInteractionRunTask(p,&p->Body[which]);
					p->StartTime = nowTime;
				}
				/*还需要执行的话,记录下次需要执行的最小时间*/
				if(p->Body[which].isRun == 1)
				{
					//printf("Offset = %d\r\n",p->Body[ii].Offset);
					if(MinTime!=0)
					{
						if(p->Body[which].Offset == 0)
						{
							offset = p->Body[which].TaskLenght-1;
						}
						else offset = p->Body[which].Offset-1;
						
						if((p->Body[which].InteractionTask[offset].RunTime+p->StartTime) < nowTime)
						{
							MinTime = 0;
						}
						else 
						{
							if(MinTime>((p->Body[which].InteractionTask[offset].RunTime+p->StartTime)-nowTime))
							{
								MinTime = (p->Body[which].InteractionTask[offset].RunTime+p->StartTime)-nowTime;
							}
						}
					}				
				}
				else 
				{
					uint8_t whichB;
					whichB = S_ADInteractionGetHighestPrio(p->Body,p->BodyNum);
					if((which != 0xff)&&(whichB!=which))
					{
						MinTime = 0;						
					}
				}
			}
			/*改交互没有需要被执行的了，那就执行默认的状态*/
			if(MinTime == ADInteraction_WaitForever)
			{
				if(p->DefaultFun!=NULL)
				p->DefaultFun();
			}
			/*下一个*/
			p = p->next;
			
		}
		return MinTime;
	}

//	ADInteraction *p;
//	uint16_t ii = 0;
//	
//	p = xADInteraction;
//	
//	for(ii = 0;ii<p->BodyNum;ii++)
//	{
//		if(p->Body[ii].TaskLenght == 0)
//		{
//			p->Body[ii].InteractionTask = 	TaskList;
//			p->Body[ii].name 			= 	name;
//			p->Body[ii].isRun 			= 	0;
//			p->Body[ii].RunCnt 			= 	0;
//			p->Body[ii].Offset 			= 	0;
//			p->Body[ii].RunTimes 		= 	times;
//			p->Body[ii].TaskLenght 		= 	lenght;
//			p->Body[ii].Prio			=	prio;
//			break;
//		}
//	}
//	if(ii>=p->BodyNum)
//	return ADInteraction_Err;
//	else return ADInteraction_OK;
//}

//uint32_t ADInteractionRun(void)
//{
//	uint32_t MinTime = ADInteraction_WaitForever;
//	ADInteraction *p;
//	uint8_t flag = 0;				/*标记交互是否全部不需要执行了*/
//	if(ADInteractionLinkListHead == NULL)
//	{
//		return ADInteraction_WaitForever;
//	}
//	else 
//	{
//		uint16_t ii = 0;
//		p = ADInteractionLinkListHead;
//		while(p != NULL)
//		{
//			 flag = 0;
//			for(ii = 0;ii<p->BodyNum;ii++)
//			{
//				uint8_t offset = 0;
//				uint32_t nowTime = ADInteraction_GetTick();
//				
//				if(p->Body[ii].Offset == 0)
//				{
//					offset = p->Body[ii].TaskLenght-1;
//				}
//				else offset = p->Body[ii].Offset-1;
//				
//				if((p->Body[ii].isRun == 1)&&
//					((p->StartTime ==0)||(p->StartTime+p->Body[ii].InteractionTask[offset].RunTime)<nowTime)
//				)
//				{
//					flag = 1;
//					S_ADInteractionRunTask(&p->Body[ii]);
//					p->StartTime = nowTime;
//				}
//				/*还需要执行的话,记录下次需要执行的最小时间*/
//				if((p->Body[ii].isRun == 1)&&(MinTime!=0))
//				{
//					//printf("Offset = %d\r\n",p->Body[ii].Offset);
//					if(p->Body[ii].Offset == 0)
//					{
//						offset = p->Body[ii].TaskLenght-1;
//					}
//					else offset = p->Body[ii].Offset-1;
//					
//					if((p->Body[ii].InteractionTask[offset].RunTime+p->StartTime) < nowTime)
//					{
//						MinTime = 0;
//					}
//					else 
//					{
//						if(MinTime>((p->Body[ii].InteractionTask[offset].RunTime+p->StartTime)-nowTime))
//						{
//							MinTime = (p->Body[ii].InteractionTask[offset].RunTime+p->StartTime)-nowTime;
//						}
//					}
//				}
//			}
//			/*改交互没有需要被执行的了，那就执行默认的状态*/
//			if(flag == 0)
//			{
//				if(p->DefaultFun!=NULL)
//				p->DefaultFun();
//			}
//			/*下一个*/
//			p = p->next;
//			
//		}
//		return MinTime;
//	}
}

void ADInteractionStart(void *xADInteraction,uint8_t name)
{
	ADInteraction *p;
	uint8_t ii = 0;
	uint8_t destPrio = 0;
	uint8_t destOffset = 0xff;
	
	p = xADInteraction;
	
	/*找到目标交互方式的位置和优先级*/
	for(ii = 0;ii<p->BodyNum;ii++)
	{
		if(p->Body[ii].name == name)
		{	
			if(p->Body[ii].isProtect == 1)
			{
				p->Body[ii].isRun = 1;
				p->StartTime = 0;
				p->Body[ii].Offset = 0;
				p->Body[ii].RunCnt = 0;
#ifdef ADInteraction_RunImmediately
				ADInteraction_RunImmediately();
#endif			
				return;
			}
			else 
			{
				destPrio = p->Body[ii].Prio;
				destOffset = ii;
				break;
			}			
		}
	}
	/*找不到这个交互方式*/
	if(destOffset>=0xff)
	{
		return ;
	}
	/*检查有没有优先级高于目标交互的和优先级低于目标等级且无保护的*/
	for(ii = 0;ii<p->BodyNum;ii++)
	{
		if(p->Body[ii].name == name)
		{		
			
		}
		else 
		{
			if(p->Body[ii].isRun == 1)
			{
				if( p->Body[ii].Prio<destPrio)					/*优先级高于目标交互的*/
				{
					return;
				}
				else 											/*优先级低于目标交互的*/
				{
					if( p->Body[ii].isProtect == 0)
					{
						p->Body[ii].isRun = 0;
					}
				}
			}
		}
	}	
	
	p->Body[destOffset].isRun = 1;
	p->StartTime = 0;
	p->Body[destOffset].Offset = 0;
	p->Body[destOffset].RunCnt = 0;
	
#ifdef ADInteraction_RunImmediately
			ADInteraction_RunImmediately();
#endif	
}

void ADInteractionStop(void *xADInteraction,uint8_t name)
{
	ADInteraction *p;
	uint8_t ii = 0;
	p = xADInteraction;
	for(ii = 0;ii<p->BodyNum;ii++)
	{
		if(p->Body[ii].name == name)
		{
			p->Body[ii].isRun = 0;
			/*立刻执行*/
#ifdef ADInteraction_RunImmediately
			ADInteraction_RunImmediately();
#endif
		}		
	}			
}
/*设置默认状态*/
void ADInteractionSetDefault(void *xADInteraction,void *f)
{
	ADInteraction *p;
	p = xADInteraction;
	p->DefaultFun = (void (*)(void))f;
}
/*设置交互保护*/
void ADInteractionSetProtect(void *xADInteraction,uint8_t name)
{
	ADInteraction *p;
	uint8_t ii = 0;
	p = xADInteraction;
	for(ii = 0;ii<p->BodyNum;ii++)
	{
		if(p->Body[ii].name == name)
		{
			p->Body[ii].isProtect = 1;
		}		
	}
}

/*清除交互保护*/
void ADInteractionClearProtect(void *xADInteraction,uint8_t name)
{
	ADInteraction *p;
	uint8_t ii = 0;
	p = xADInteraction;
	for(ii = 0;ii<p->BodyNum;ii++)
	{
		if(p->Body[ii].name == name)
		{
			p->Body[ii].isProtect = 0;
		}		
	}
}
#if configEnInteractionFinishCallback == 1
//设置交互方式完成回调
void ADInteractionSetFinishCallback(void *xADInteraction,void *f)
{
	ADInteraction *p;
	p = xADInteraction;		
	p->finishCallback = (void (*)(void *,uint8_t))f;				
}
#endif
