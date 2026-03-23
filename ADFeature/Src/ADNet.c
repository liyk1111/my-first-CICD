#include "ADNet.h"




/*
*作者：詹乾坤
*V2.0.1-2023-01-17:V2的第一版。
*V2.0.1-2023-01-30:
*					修复了重发失败回调函数里面置1该步骤失效问题。
*					修改函数ADNetSetSendFailTimes名称为ADNetSetStepFailTimes
*V2.0.1-2023-01-31:添加开始重发按钮。修复ADNetResendStop可能存在的bug.
*V2.0.1-2023-02-01:优化ADNetIsIdle函数
*V2.0.1-2023-02-09:添加ADNetGetStepIsResendStart函数
*V2.0.1-2023-08-16:添加ADNetResendStopAll函数
*V2.0.1-2023-08-24:修改以满足安电软件库的要求。
*V3.0.0-2023-09-19:V3.0.0第一版，ADNetSetStep修改为无参数，新增ADNetSetStepWithPara带参数写入，且不需要传递指令等。修复ADNetGetResendCnt可能出错问题。
*V3.0.0-2023-10-27:修改写法，避免被编译器优化，导致出错。
*V3.0.0-2023-12-19:添加定时设置步骤就绪。添加上锁和解锁功能。
*V3.0.0-2023-12-28:新增允许设置多组优先级。快速重发算法暂时没有实现。
*/

#define ADNet_Ver	"V3.0.0-2023-12-19"

void S_ADNetPrintf(char *s)
{
	printf("%s",s);
}
/*步骤恢复默认*/
void S_ADNetDeInit(ADNetStep *S)
{
	S->isReady		=	ADNet_NO;
	S->IDCreator	=	0;
	S->reSend		=	NULL;
	S->sendFailCnt	=	0;
	

	S->para.formalPara.id		=	0;
	S->para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer)		=	NULL;
	S->para.formalPara.fpLenght	=	0;
	S->para.formalPara.fpType	=	ADNet_ParaNone;
	S->para.next				=	NULL;
#if ADNet_config_EnStepTiming == 1
	S->isReadyTime				=	0;
#endif
}

/*获取参数值*/
uint32_t S_ADNetParaValue(uint32_t Value)
{	
	return Value;
}

/*写入参数*/
uint32_t S_ADNetWritePara(ADNetPassParameter *sP,fpUnion para,unsigned int lenght,uint8_t ParaType,ADNetStep *Step)
{
	if(lenght!=0)
	{
		memcpy(sP->formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer),para.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer),lenght);
	}
	else 
	{
		sP->formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer) = NULL;
	}
	
	/*生成ID*/
	Step->IDCreator++;
	if(Step->IDCreator == 0)
	Step->IDCreator++;
	
	sP->formalPara.fpLenght 	= 	lenght;
	sP->formalPara.id			=	Step->IDCreator;
	sP->formalPara.fpType		=	ParaType;
	sP->next					=	NULL;
	
	return sP->formalPara.id;
}
/*写入参数*/
uint32_t S_ADNetWriteParaStatic(ADNetPassParameter *sP,fpUnion para,unsigned int lenght,uint8_t ParaType,ADNetStep *Step)
{		
	sP->formalPara.fp = para;	
	
	Step->IDCreator++;
	if(Step->IDCreator == 0)
	Step->IDCreator++;
	

	sP->formalPara.fpLenght 	= 	lenght;
	
	
	sP->formalPara.id			=	Step->IDCreator;
	sP->formalPara.fpType		=	ParaType;
	sP->next					=	NULL;
	
	return sP->formalPara.id;
}
/*设置步骤优先级*/
void S_ADNetSetReady(ADNet *xADNet, ADNetStep *Step, unsigned char yn)
{
	Step->isReady = Step->isReady;
	Step->isReady = yn;
	
	
	if(yn == ADNet_NO)
	{
		
	}
	else 
	{
		if(xADNet->isIdle == ADNet_YES)
		{
			xADNet->isIdle = ADNet_NO;
			if(xADNet->isReadyCallBack!=NULL)
			xADNet->isReadyCallBack(xADNet);
		}
	}
}
/*释放传入参数*/
unsigned char S_ADNetFreeFormalPara(ADNetPassParameterBody *formalPara)
{
	if(formalPara->fpType == ADNet_ParaDynamicPointer)
	{
		if(formalPara->fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer)!=NULL)
		{		
			ADNetFree(formalPara->fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer));
			formalPara->fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer) = NULL;
			formalPara->fpLenght = 0;
			return ADNet_YES;
		}
	}
	else 
	{
		formalPara->fp.ADNet_CombinationUnion(fp,ADNet_ParaDouble) = NULL;
		formalPara->fpLenght = 0;
		//printf("formalPara->id=%d\r\n",formalPara->id);
	}
			
	return ADNet_NO;
}
/*初始化重发*/
void S_ADNetResendInit(ADNetStep *S)
{
	if(S->reSend!=NULL)
	{
		S->reSend->reSendCnt	=	0;
		S->reSend->isReady		=	ADNet_NO;
	}
}
/*删除重发*/
void S_ADNetReSendStop(ADNetStep *S)
{
	S_ADNetResendInit(S);
}
#if ADNet_config_ResendUseFastArith == 1
/*计算槽的长度*/
uint16_t S_ADNetCalSlotSize(uint16_t lenght ,uint16_t slotNum)
{
	uint16_t slotSize;	
	slotSize	=	round(((float)lenght)/slotNum);
	slotSize	= 	slotSize?slotSize:1;
	return slotSize;
}

uint8_t S_ADNetGenerateSlot(ADNet *xADNet,uint8_t slotNum)
{
	uint16_t lenght = 0,slotMaxSize,residueLenght = 0,ii = 0,yy = 0;
	uint8_t result = 0;
	
	
	lenght			=	xADNet->lenght;						/*计算总长度*/
	slotMaxSize		=	S_ADNetCalSlotSize(lenght,slotNum);	/*槽大小*/
	residueLenght	=	lenght;								/*剩余长度*/
	
	
	
	for(ii = 0;ii<slotNum;ii++)	
	{
		if(ii<slotNum-1)
		{
			xADNet->resendSlotSize[ii]	=	slotMaxSize;
		}
		else 
		{
			xADNet->resendSlotSize[ii]	=	residueLenght;
		}
		residueLenght = residueLenght - xADNet->resendSlotSize[ii];
	}
	

	
	for(ii = 0;ii<slotNum;ii++)	
	{
		//printf("%d,",xADNet->resendSlotSize[ii]);
		for(yy = 0;yy<xADNet->resendSlotSize[ii];yy++)
		{	
			
			if(xADNet->Step[ii*slotMaxSize+yy].reSend!=NULL)
			{
				result = result|(0x01<<ii);
				
				break;
			}
		}
	}
	//printf("\r\n");
	//printf("result=%x\r\n",result);
	return result;
}


/*生成重发标志算法*/
void S_ADNetGenerateResendFlag(ADNet *xADNet)
{	
	xADNet->resendFlag = S_ADNetGenerateSlot(xADNet,ADNet_config_ResendFlagSlotNum);
}
#endif

/*开启重发*/
void S_ADNetResendStart(ADNetStep *Step,uint8_t yn)
{
	Step->reSend->isReady = yn;
	Step->reSend->reSendStartTime	=	__ADNetGetTick + Step->reSend->reSendPeriod;	
}

void S_ADNetResendFailCallBack(ADNet *xADNet,ADNetReSendFailCallBack f,void *fstep,ADNetPassParameterBody *para)
{
	switch(para->fpType)
	{
		case ADNet_ParaNone				:	(( void	(*)(ADNet *,void *,void *,uint32_t))f)(xADNet,fstep,0,0);break;
		case ADNet_ParaUint8_t			:	(( void (*)(ADNet *,void *,uint8_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint8_t));									break;
		case ADNet_ParaUint16_t			:	(( void (*)(ADNet *,void *,uint16_t))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint16_t));									break;		
		case ADNet_ParaUint32_t			:	(( void (*)(ADNet *,void *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint32_t));									break;
		
		case ADNet_ParaFloat			:	(( void (*)(ADNet *,void *,float ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaFloat));										break;
		case ADNet_ParaDouble			:	(( void (*)(ADNet *,void *,double ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaDouble));										break;
		case ADNet_ParaPointer			:	(( void (*)(ADNet *,void *,void *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaPointer),para->fpLenght);		break;
		case ADNet_ParaDynamicPointer   :	(( void (*)(ADNet *,void *,void *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer),para->fpLenght);break;

		case ADNet_ParaUint8_tPointer   :	(( void (*)(ADNet *,void *,uint8_t *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint8_tPointer),para->fpLenght);break;
		case ADNet_ParaUint16_tPointer  :	(( void (*)(ADNet *,void *,uint16_t *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint16_tPointer),para->fpLenght);break;
		case ADNet_ParaUint32_tPointer  :	(( void (*)(ADNet *,void *,uint32_t *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint32_tPointer),para->fpLenght);break;
		case ADNet_ParaFloatPointer    	:	(( void (*)(ADNet *,void *,float *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaFloatPointer),para->fpLenght);			break;
		case ADNet_ParaDoublePointer    :	(( void (*)(ADNet *,void *,double *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaDoublePointer),para->fpLenght);break;
		case ADNet_ParaCharPointer		:	(( void (*)(ADNet *,void *,char *,uint32_t ))f)(xADNet,fstep,para->fp.ADNet_CombinationUnion(fp,ADNet_ParaCharPointer),para->fpLenght);break;
		default:(( void	(*)(ADNet *,void *,void *,uint32_t))f)(xADNet,fstep,0,0);break;
	}
}
/*重发处理*/
void S_ADNetResendProcess(ADNet *xADNet,ADNetStep *Step,void *f)
{
	
	if(Step->reSend->isReady!=ADNet_NO)
	{
		if(Step->reSend->reSendStartTime<=__ADNetGetTick)									//等待时间到了
		{									
			if(Step->reSend->reSendCnt<Step->reSend->reSendTimes)
			{
				Step->reSend->reSendCnt	= Step->reSend->reSendCnt	+	1;					
				S_ADNetSetReady(xADNet,Step,Step->reSend->isReady);	
			}
			else 					/*重发结束了*/
			{			
				Step->reSend->isReady	=	ADNet_NO;
				Step->reSend->reSendCnt	=	0;
				
				/*清零*/
				S_ADNetSetReady(xADNet,Step,ADNet_NO);	
				/*失败回调*/
				if(Step->reSend->reSendFailCallBack!=NULL)
				{
					S_ADNetResendFailCallBack(xADNet,Step->reSend->reSendFailCallBack,f,&(Step->para.formalPara));
					//printf("Step->para.formalPara.fp.fp3:%d\r\n",Step->para.formalPara.fp.fp3);
				}
				//Step->reSend->reSendFailCallBack(xADNet,xADNet->sFunc[offset],&(Step->para.formalPara));	
				
				if(Step->isReady == ADNet_NO)/*如果成立，那就说明在重发失败回调函数，里面没有被置1，那就可以放心删除参数*/
				/*删除当前步骤参数*/
				S_ADNetFreeFormalPara(&(Step->para.formalPara));				
			}
		}
	}
}
/*根据偏移量对步骤情况*/
char S_ADNetResetStepWithStep(ADNet *xADNet ,ADNetStep *Step)
{				
	if(xADNet==NULL)
	{
		return ADNet_NO;
	}		
	if(Step->reSend == NULL)
	{
		/*删除当前步骤参数*/		
		S_ADNetFreeFormalPara(&Step->para.formalPara);
		/*清零*/	
		Step->isReady = ADNet_NO;
		S_ADNetSetReady(xADNet,Step,ADNet_NO);
	}
	else 
	{
		Step->isReady = ADNet_NO;
		
		S_ADNetSetReady(xADNet,Step,ADNet_NO);
	}	
	return ADNet_YES;
}
void S_ADNetClearStep(ADNet *xADNet ,ADNetStep *realityRunStep,ADNetStep *backupsRunStep)
{
	realityRunStep->sendFailCnt = 0;
	
	if(realityRunStep->para.formalPara.id == backupsRunStep->para.formalPara.id)
	{
		/*重新开启重发*/
		if(realityRunStep->reSend!=NULL)
		{
			S_ADNetResendStart(realityRunStep,realityRunStep->isReady);
		}
		
		S_ADNetResetStepWithStep(xADNet,realityRunStep);
	}
}
/**********************************************************************************************/



/*创建一个网络*/
void *ADNetCreat(void)
{	
	ADNet *xADNet;	
	uint16_t ii = 0;
	xADNet = ADNetMalloc(sizeof(ADNet));
	if(xADNet==NULL)
	{
		printf("内存不足\r\n");
		return NULL;
	}
	else
	{		
		for(ii = 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
		{
			xADNet->StepAssembly[ii].Step = NULL;
			xADNet->StepAssembly[ii].sFunc = NULL;
			xADNet->StepAssembly[ii].lenght = 0;
		}
//		xADNet->sFunc				=	NULL;
//		xADNet->lenght				=	0;
		
		xADNet->isReadyCallBack		=	NULL;
		xADNet->isIdleCallBack		=	NULL;
		xADNet->isIdle				=	0xff;
		xADNet->isLock				=	ADNet_NO;
//		xADNet->isStaticStep		=	ADNet_NO;
#if ADNet_config_ResendUseFastArith == 1
		xADNet->resendFlag			=	0;
		for(ii = 0;ii<ADNet_config_ResendFlagSlotNum;ii++)
		{
			xADNet->resendSlotSize[ii] = 0;
		}
#endif

#if ADNet_config_EnStepTiming == 1
			xADNet->MinTimingStepTime = 0xffffffff;
#endif
		return	xADNet;					
	}
}
/*创建一个网络并指定步骤长度*/
/**
 *@lenght				:	网络中步骤的长度
*/
void *ADNetCreatWithLenght(unsigned short int lenght)
{	
	ADNet *xADNet;
	uint16_t ii = 0;	
	xADNet = ADNetMalloc(sizeof(ADNet));
	if(xADNet==NULL)
	{
		printf("ADNetCreatWithLenght 堆空间不足\r\n");
		return NULL;
	}
	else
	{	xADNet->StepAssembly[0].Step	=	ADNetMalloc(sizeof(ADNetStep)*lenght);
		if(xADNet->StepAssembly[0].Step	==	NULL)
		{
			printf("ADNetCreatWithLenght 内存不足\r\n");
			ADNetFree(xADNet);
			return NULL;
		}
		else 
		{
			//unsigned short int i;
//			for(i = 0;i<lenght;i++)
//			{
//				S_ADNetDeInit(&xADNet->Step[i]);
//			}
//			xADNet->sFunc				=	NULL;
//			xADNet->lenght				=	lenght;
			//for(ii = 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
							
			for(ii = 0;ii<lenght;ii++)
			{
				S_ADNetDeInit(&xADNet->StepAssembly[0].Step[ii]);
			}
			xADNet->StepAssembly[0].sFunc = NULL;
			xADNet->StepAssembly[0].lenght = lenght;
			
			
			xADNet->isReadyCallBack		=	NULL;
			xADNet->isIdleCallBack		=	NULL;
			xADNet->isIdle				=	0xff;
			xADNet->isLock				=	ADNet_NO;
//			xADNet->isStaticStep		=	ADNet_NO;
#if ADNet_config_ResendUseFastArith == 1
			xADNet->resendFlag			=	0;
			for(ii = 0;ii<ADNet_config_ResendFlagSlotNum;ii++)
			{
				xADNet->resendSlotSize[ii] = 0;
			}
#endif

#if ADNet_config_EnStepTiming == 1
			xADNet->MinTimingStepTime = 0xffffffff;
#endif
			return	xADNet;
		}			
	}
}
/*使用静态内存创建一个网络并指定步骤长度*/
/**
 *@lenght				:	网络中步骤的长度
 *@*S					:	步骤函数数组
*/
void *ADNetCreatWithLenghtStatic(unsigned short int lenght,ADNetStep *S)
{
	ADNet *xADNet;	
	uint16_t ii = 0;
	xADNet = ADNetMalloc(sizeof(ADNet));
	if(xADNet==NULL)
	{
		printf("ADNetCreatWithLenghtStatic 内存不足\r\n");
		return NULL;
	}
	else
	{	xADNet->StepAssembly[0].Step	=	S;
		if(xADNet->StepAssembly[0].Step	==	NULL)
		{
			printf("ADNetCreatWithLenghtStatic 您传入的步骤表为空\r\n");
			return NULL;
		}
		else 
		{
			
//			for(ii = 0;ii<lenght;ii++)
//			{
//				S_ADNetDeInit(&xADNet->Step[ii]);
//			}
//			xADNet->sFunc				=	NULL;
//			xADNet->lenght				=	lenght;
			for(ii = 0;ii<lenght;ii++)
			{
				S_ADNetDeInit(&xADNet->StepAssembly[0].Step[ii]);
			}
			xADNet->StepAssembly[0].sFunc = NULL;
			xADNet->StepAssembly[0].lenght = lenght;
			
			xADNet->isReadyCallBack		=	NULL;
			xADNet->isIdleCallBack		=	NULL;
			xADNet->isIdle				=	0xff;
			xADNet->isLock				=	ADNet_NO;
//			xADNet->isStaticStep		=	ADNet_YES;
#if ADNet_config_ResendUseFastArith == 1
			xADNet->resendFlag			=	0;
			for(ii = 0;ii<ADNet_config_ResendFlagSlotNum;ii++)
			{
				xADNet->resendSlotSize[ii] = 0;
			}
#endif

#if ADNet_config_EnStepTiming == 1
			xADNet->MinTimingStepTime = 0xffffffff;
#endif
			return	xADNet;
		}			
	}
}

//设置步骤优先级
/**
 *@xADNet				:	网络
 *@*f[]					:	步骤函数数组
 *@lenght				:	步骤的长度
*/
char ADNetSetStepPrio(ADNet *xADNet ,void *f[],unsigned short int lenght)
{
	unsigned short int ii = 0,yy = 0;
	if(xADNet == NULL)
	return ADNet_NO;
//	if(xADNet->lenght == 0||xADNet->Step == NULL)
//	{
//		xADNet->Step	=	ADNetMalloc(lenght*sizeof(ADNetStep));
//		if(xADNet->Step	== NULL)
//		{
//			printf("ADNetSetStepPrio 内存不足\r\n");
//			return ADNet_NO;
//		}
//		else 
//		{
//			for(i = 0;i<lenght;i++)
//			{
//				S_ADNetDeInit(&xADNet->Step[i]);				
//			}
//			xADNet->sFunc = (ADNetStepF*)f;
//			xADNet->lenght = lenght;
//			return ADNet_YES;
//		}
//	}
//	else 
//	{
//		if(lenght>xADNet->lenght)
//		{
//			printf("ADNetSetStepPrio 您创建的步骤长度不足\r\n");
//			return ADNet_NO;
//		}
//		else 
//		{									
//			xADNet->sFunc = (ADNetStepF*)f;
//			xADNet->lenght = lenght;	
//			return ADNet_YES;
//		}	
//	}	

	for(ii = 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
	{
		if(xADNet->StepAssembly[ii].lenght == 0||xADNet->StepAssembly[ii].Step == NULL)/*那就说明1，步骤还没有被创建，2，这个优先级组还没有被使用*/
		{
			xADNet->StepAssembly[ii].Step	=	ADNetMalloc(lenght*sizeof(ADNetStep));
			if(xADNet->StepAssembly[ii].Step	== NULL)
			{
				printf("ADNetSetStepPrio 内存不足\r\n");
				return ADNet_NO;
			}
			else 
			{
				for(yy = 0;yy<lenght;yy++)
				{
					S_ADNetDeInit(&xADNet->StepAssembly[ii].Step[yy]);				
				}
				xADNet->StepAssembly[ii].sFunc = (ADNetStepF*)f;
				xADNet->StepAssembly[ii].lenght = lenght;
				return ADNet_YES;
			}			
		}
		else if(xADNet->StepAssembly[ii].sFunc == NULL)		/*说明1，步骤被创建了，2，但这个优先级组还没有被使用*/
		{
			if(lenght>xADNet->StepAssembly[ii].lenght)
			{
				printf("ADNetSetStepPrio 您创建的步骤长度不足\r\n");
				return ADNet_NO;
			}
			else 
			{									
				xADNet->StepAssembly[ii].sFunc = (ADNetStepF*)f;
				xADNet->StepAssembly[ii].lenght = lenght;	
				return ADNet_YES;
			}			
		}
//		else 												/*说明1，步骤被创建了，2，优先级组被使用了*/
//		{
//				
//		}
	}	
	return ADNet_NO;
}

//复位所有步骤
void ADNetResetAllStep(ADNet *xADNet)
{
	unsigned int ii = 0,yy = 0,lenght ;
	ADNetStep *Step ;
	
	if(xADNet==NULL)
	{
		return;
	}
	for(ii = 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
	{
		
		lenght = xADNet->StepAssembly[ii].lenght;
		Step = xADNet->StepAssembly[ii].Step;
		
		for(yy = 0;yy<lenght;yy++)
		{
			S_ADNetSetReady(xADNet,&Step[yy],ADNet_NO);	
			Step[yy].sendFailCnt = 0;
			ADNetPassParameter *p = &Step[yy].para;
			
			/*删除第一个静态的参数*/
			S_ADNetFreeFormalPara(&p->formalPara);
		
			
			/*如果还有其他参数，继续删除*/
			if((p->next!=NULL))													/*链表不为空*/
			{
				ADNetPassParameter *l;
				p = p->next;
				while(p!=NULL)
				{
					S_ADNetFreeFormalPara(&p->formalPara);
					
					l = p;
					p = p->next;
					ADNetFree(l);
				}
			}
		}
	}
	
}

/*获取步骤位置*/
ADNetStep *ADNetGetStep(ADNet *xADNet ,void *f)
{
	unsigned int ii = 0,len ,yy = 0;
	ADNetStepF *sFunc ;
	
	if(xADNet==NULL)
	{
		return NULL;
	}
	

	
	for(ii = 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
	{
		len 	=	xADNet->StepAssembly[ii].lenght;
		sFunc 	= 	(ADNetStepF *)xADNet->StepAssembly[ii].sFunc;
	
		for(yy = 0;yy<len;yy++)
		{
			if(sFunc[yy]==f)
			{
				return &xADNet->StepAssembly[ii].Step[yy];
				
			}
		}
		
	}
	return NULL;
};


//步骤置1
unsigned int ADNetSetStep(ADNet *xADNet ,void *f,unsigned char Prio,...)
{

	ADNetStep *Step;
	unsigned int id	= ADNet_NO;
	
	if(xADNet==NULL)
	{
		printf("网络为空\r\n");
		return ADNet_NO;
	}
	else if(xADNet->isLock==ADNet_YES)
	{
		return ADNet_NO;
	}
	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);
	//printf("offset = %d,%d\r\n",offset,xADNet->lenght);	
	if(Step!=NULL)
	{
		fpUnion para;
					
/************************************************************************************************************************************************************/																									
													
		/*删除重发*/
		if(Step->reSend!=NULL)
		S_ADNetReSendStop(Step);
				
		/*释放旧的参数*/	
		S_ADNetFreeFormalPara(&Step->para.formalPara);
				
		/*写入参数*/		
		id = S_ADNetWritePara(&Step->para,para,ADNet_ParaNone,0,Step);
				
#if ADNet_config_EnStepTiming == 1
		Step->isReadyTime = 0;
#endif
		/*设置步骤优先级,这个要放在最后*/	
		S_ADNetSetReady(xADNet,Step,Prio);
		goto ADNetSetSuccss;							
	}
	else 
	{
		//printf("f=%d\r\n",f);
		goto ADNetSetFail;
	}
	ADNetSetFail:
	return ADNet_NO;
	ADNetSetSuccss:
	return id;
}
#if ADNet_config_EnStepTiming == 1
//步骤定时置1
unsigned int ADNetSetTimingStep(ADNet *xADNet ,void *f,unsigned char Prio,uint32_t readyTime)
{
	
	ADNetStep *Step;
	unsigned int id	= ADNet_NO;
	
	if(xADNet==NULL)
	{
		printf("网络为空\r\n");
		return ADNet_NO;
	}
	else if(xADNet->isLock==ADNet_YES)
	{
		return ADNet_NO;
	}
	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);
	//printf("offset = %d,%d\r\n",offset,xADNet->lenght);	
	if(Step!=NULL)
	{
		fpUnion para;
					
/************************************************************************************************************************************************************/																									
													
		/*删除重发*/
		if(Step->reSend!=NULL)
		S_ADNetReSendStop(Step);
				
		/*释放旧的参数*/	
		S_ADNetFreeFormalPara(&Step->para.formalPara);
		

		
				
		id = S_ADNetWritePara(&Step->para,para,ADNet_ParaNone,0,Step);
		
		/*设置步骤优先级,这个要放在最后*/	
		Step->isReadyTime = readyTime + __ADNetGetTick;
		S_ADNetSetReady(xADNet,Step,Prio);
		goto ADNetSetSuccss;							
	}
	else 
	{
		//printf("f=%d\r\n",f);
		goto ADNetSetFail;
	}
	ADNetSetFail:
	return ADNet_NO;
	ADNetSetSuccss:
	return id;
}


//步骤置1
unsigned int S_ADNetSetTimingStepWithPara(ADNet *xADNet ,void *f,unsigned char Prio,uint32_t readyTime,uint8_t ParaType,fpUnion para,...)
{
	
	unsigned int id	= ADNet_NO;
//	unsigned short int lenght;
	ADNetStep *Step;
	
	if(xADNet==NULL)
	{
		goto ADNetSetFail;
	}
	else if(xADNet->isLock==ADNet_YES)
	{
		return ADNet_NO;
	}
	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);
		
	if(Step!=NULL)
	{
		unsigned short int paraLenght = 0;
		
/************************************************************************************************************************************************************/		
		
		/*删除重发*/
		if(Step->reSend!=NULL)
		S_ADNetReSendStop(Step);
		
		/*释放旧的参数*/				
		S_ADNetFreeFormalPara(&Step->para.formalPara);
		
		
		/*动态参数*/
		if(ParaType == ADNet_ParaDynamicPointer)
		{
			/*保存动态参数*/
			
			
			
			paraLenght 	=	S_ADNetParaValue(*(  (uint32_t*)  S_ADNetParaValue((((uint32_t)&para)+sizeof(para)))  ));//获取参数长度，这里是避免被编译器优化
			Step->para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer) =  ADNetMalloc(paraLenght);
			
			if(Step->para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer)  == NULL)
			{
				printf("内存不足\r\n");
				Step->isReadyTime = readyTime + __ADNetGetTick;
				S_ADNetSetReady(xADNet,Step,Prio);
				goto ADNetSetFail;
			}
			else 
			{
				id = S_ADNetWritePara(&Step->para,para,paraLenght,ParaType,Step);	
			}
		}
		else 
		{
			if(ParaType == ADNet_ParaPointer||ParaType == ADNet_ParaUint8_tPointer||ParaType == ADNet_ParaUint16_tPointer
					||ParaType == ADNet_ParaUint32_tPointer||ParaType == ADNet_ParaFloatPointer||ParaType == ADNet_ParaDoublePointer||ParaType == ADNet_ParaCharPointer)
			{
				paraLenght 	=	S_ADNetParaValue(*(  (uint32_t*)  S_ADNetParaValue((((uint32_t)&para)+sizeof(para)))  ));//获取参数长度，这里是避免被编译器优化
				//printf("paraLenghtvvv=%d\r\n",paraLenght);
			}
			/*保存静态参数*/
			id = S_ADNetWriteParaStatic(&Step->para,para,paraLenght,ParaType,Step);	
		}
		
		//printf("paraLenght=%d\r\n",paraLenght);
		/*设置步骤优先级,这个要放在最后*/
		Step->isReadyTime = readyTime + __ADNetGetTick;		
		S_ADNetSetReady(xADNet,Step,Prio);
		
		goto ADNetSetSuccss;							
	}
	else goto ADNetSetFail;
					
	ADNetSetFail:
	return ADNet_NO;
	ADNetSetSuccss:
	return id;
}
#endif
//步骤置1
unsigned int S_ADNetSetStepWithPara(ADNet *xADNet ,void *f,unsigned char Prio,uint8_t ParaType,fpUnion para,...)
{
	unsigned int id	= ADNet_NO;
//	unsigned short int lenght;
	ADNetStep *Step;
	
	if(xADNet==NULL)
	{
		goto ADNetSetFail;
	}
	else if(xADNet->isLock==ADNet_YES)
	{
		return ADNet_NO;
	}
	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);
		
	if(Step!=NULL)
	{
		unsigned short int paraLenght = 0;
						
/************************************************************************************************************************************************************/		
		
		/*删除重发*/
		if(Step->reSend!=NULL)
		S_ADNetReSendStop(Step);
		
		/*释放旧的参数*/				
		S_ADNetFreeFormalPara(&Step->para.formalPara);
		
		
		/*动态参数*/
		if(ParaType == ADNet_ParaDynamicPointer)
		{
			/*保存动态参数*/
			
			
			
			paraLenght 	=	S_ADNetParaValue(*(  (uint32_t*)  S_ADNetParaValue((((uint32_t)&para)+sizeof(para)))  ));//获取参数长度，这里是避免被编译器优化
			Step->para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer) =  ADNetMalloc(paraLenght);
			
			if(Step->para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer)  == NULL)
			{
				printf("内存不足\r\n");
#if ADNet_config_EnStepTiming == 1
				Step->isReadyTime = 0;
#endif
				S_ADNetSetReady(xADNet,Step,Prio);
				goto ADNetSetFail;
			}
			else 
			{
				id = S_ADNetWritePara(&Step->para,para,paraLenght,ParaType,Step);	
			}
		}
		else 
		{
			if(ParaType == ADNet_ParaPointer||ParaType == ADNet_ParaUint8_tPointer||ParaType == ADNet_ParaUint16_tPointer
					||ParaType == ADNet_ParaUint32_tPointer||ParaType == ADNet_ParaFloatPointer||ParaType == ADNet_ParaDoublePointer||ParaType == ADNet_ParaCharPointer)
			{
				paraLenght 	=	S_ADNetParaValue(*(  (uint32_t*)  S_ADNetParaValue((((uint32_t)&para)+sizeof(para)))  ));//获取参数长度，这里是避免被编译器优化
				//printf("paraLenghtvvv=%d\r\n",paraLenght);
			}
			/*保存静态参数*/
			id = S_ADNetWriteParaStatic(&Step->para,para,paraLenght,ParaType,Step);	
		}
		
		//printf("paraLenght=%d\r\n",paraLenght);
		/*设置步骤优先级,这个要放在最后*/		
#if ADNet_config_EnStepTiming == 1
		Step->isReadyTime = 0;
#endif		
		S_ADNetSetReady(xADNet,Step,Prio);
		
		goto ADNetSetSuccss;							
	}
	else goto ADNetSetFail;
					
	ADNetSetFail:
	return ADNet_NO;
	ADNetSetSuccss:
	return id;
}
/*上锁*/
void ADNetLock(ADNet *xADNet)
{
	if(xADNet!=NULL)
	xADNet->isLock = ADNet_YES;
}
/*解锁*/
void ADNetUnLock(ADNet *xADNet)
{
	if(xADNet!=NULL)
	xADNet->isLock = ADNet_NO;
}
uint8_t ADNetGetLockState(ADNet *xADNet)
{
	if(xADNet!=NULL)
	return xADNet->isLock;
	else return ADNet_NO;
}
/*根据偏移量对步骤情况*/
char ADNetResetStepWithOffest(ADNet *xADNet ,unsigned int destId,ADNetStep *Step)
{
	
	
		
	if(xADNet==NULL)
	{
		return ADNet_NO;
	}		
	
	
	
	if(Step->reSend == NULL)
	{
		/*删除当前步骤参数*/		
		S_ADNetFreeFormalPara(&Step->para.formalPara);
		/*清零*/	
		Step->isReady = ADNet_NO;
		S_ADNetSetReady(xADNet,Step,ADNet_NO);
	}
	else 
	{
		Step->isReady = ADNet_NO;
		
		S_ADNetSetReady(xADNet,Step,ADNet_NO);
	}
				

	return ADNet_YES;
}


/*根据步骤函数对步骤情况*/
char ADNetResetStep(ADNet *xADNet ,void *f,...)
{
	ADNetStep *Step;
	char Result = ADNet_NO;
	
	if(xADNet==NULL)
	{
		return ADNet_NO;
	}
	

	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);
	
	if(Step!=NULL)
	{
#if ADNet_config_StepIDLenght == 8
			uint8_t destId = 0;
			destId = S_ADNetParaValue(*(uint8_t*)((uint32_t)&f+4));
#elif ADNet_config_StepIDLenght == 16
			uint16_t destId = 0;
			destId = S_ADNetParaValue(*(uint16_t*)((uint32_t)&f+4));
			
#elif ADNet_config_StepIDLenght == 32
			uint32_t destId = 0;
			destId = S_ADNetParaValue(*(uint32_t*)((uint32_t)&f+4));
#endif
			S_ADNetReSendStop(Step);
			Result = ADNetResetStepWithOffest(xADNet,destId,Step);
			return Result;	
	}

	return ADNet_NO;
}


//获取某一步骤的状态
unsigned char ADNetGetStepStatus(ADNet *xADNet ,void *f)
{
	ADNetStep *Step;
	
	if(xADNet==NULL)
	{
		return ADNet_NO;
	}
	

	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);
	
	if(Step!=NULL)
	{
		return Step->isReady;
	}
	return ADNet_NO;
}

//获取当前最大优先级步骤
ADNetStep  *ADNetGetHighestPrioStep(ADNet *xADNet,void *f)
{
	unsigned short int ii = 0,yy,len ;
	ADNetStep *Step,*MaxStep=NULL; 
	unsigned short int PreemptPrio = 0 ;
	
#if ADNet_config_EnStepTiming == 1
	uint32_t nowTime = __ADNetGetTick;
	uint32_t MinTime= 0xffffffff;
#endif
	
	if(xADNet==NULL)
	{
		return NULL;
	}
	
	for(ii = 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
	{
		len = xADNet->StepAssembly[ii].lenght;
		Step = xADNet->StepAssembly[ii].Step;
		
		for(yy = 0;yy<len;yy++)
		{
			
#if ADNet_config_EnStepTiming == 1
			/*找到就绪的最大优先级*/
			if((Step[yy].isReadyTime==0)||(Step[yy].isReadyTime<nowTime))
			{
				if(Step[yy].isReady>PreemptPrio)
				{
					PreemptPrio = Step[yy].isReady;
					MaxStep = &Step[yy];
					*((void**)f) =  xADNet->StepAssembly[ii].sFunc[yy];
				}
			}
			
			/*找到最小定时步骤时间*/
			if(Step[yy].isReady!=ADNet_NO&&Step[yy].isReadyTime>0)
			{
				if(MinTime>Step[yy].isReadyTime)
				{
					MinTime = Step[yy].isReadyTime;
				}
			}
			
#else
			if(Step[yy].isReady>PreemptPrio)
			{
				PreemptPrio = Step[yy].isReady;
				MaxStep = &Step[yy];
			}
#endif		
		}	
	}
	
	
#if ADNet_config_EnStepTiming == 1
	if(MinTime < nowTime)
	{
		xADNet->MinTimingStepTime = 0;
	}
	else 
	{
		if(MinTime!=0xffffffff)
		xADNet->MinTimingStepTime = MinTime - nowTime;
		else xADNet->MinTimingStepTime  = 0xffffffff;
	}	
#endif
	return MaxStep;
}

/*设置空步骤时回调*/
/*设置自动重发*/
/**
 *@xADNet				:	网络
 *@f1					:	网络空闲时调用
 *@f2					:	网络从空闲到不空闲转变时调用
*/
void ADNetSetIdleAndReadyCallBack(ADNet *xADNet,void *f1,void *f2)
{
	if(xADNet	==	NULL)
	return ;
	
	xADNet->isIdleCallBack	=	(ADNetIdleAndReadyCallBack)f1;
	xADNet->isReadyCallBack	=	(ADNetIdleAndReadyCallBack)f2;
}

void ADNetPrintf(ADNetStep *step)
{
	printf("\r\n");
	printf("打印步骤:\r\n");
	printf("就绪状态：%d\r\n",step->isReady);

	
	printf("参数类型：%d\r\n",step->para.formalPara.fpType);
	printf("ID：%d\r\n",step->para.formalPara.id);
	printf("参数长度：%d\r\n",step->para.formalPara.fpLenght);

};
uint8_t S_ADNetRunStep(void  *f,ADNetPassParameterBody *para)
{
	uint8_t result = ADNet_NO;
	
	switch(para->fpType)
	{
		case ADNet_ParaNone				:	result = (( uint8_t (*)(void* ,uint32_t))f)(0,0);break;
		case ADNet_ParaUint8_t			:	result = (( uint8_t (*)(uint8_t temp))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint8_t));									break;
		case ADNet_ParaUint16_t			:	result = (( uint8_t (*)(uint16_t temp))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint16_t));									break;
		case ADNet_ParaUint32_t			:	result = (( uint8_t (*)(uint32_t temp))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint32_t));									break;
		case ADNet_ParaFloat			:	result = (( uint8_t (*)(float temp))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaFloat));										break;
		case ADNet_ParaDouble			:	result = (( uint8_t (*)(double temp))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaDouble));										break;
		case ADNet_ParaPointer			:	result = (( uint8_t (*)(void *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaPointer),para->fpLenght);		break;
		case ADNet_ParaDynamicPointer   :	result = (( uint8_t (*)(void *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer),para->fpLenght);break;

		case ADNet_ParaUint8_tPointer   :	result = (( uint8_t (*)(uint8_t *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint8_tPointer),para->fpLenght);break;
		case ADNet_ParaUint16_tPointer  :	result = (( uint8_t (*)(uint16_t *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint16_tPointer),para->fpLenght);break;
		case ADNet_ParaUint32_tPointer  :	result = (( uint8_t (*)(uint32_t *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaUint32_tPointer),para->fpLenght);break;
		case ADNet_ParaFloatPointer    	:	result = (( uint8_t (*)(float *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaFloatPointer),para->fpLenght);			break;
		case ADNet_ParaDoublePointer    :	result = (( uint8_t (*)(double *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaDoublePointer),para->fpLenght);break;
		case ADNet_ParaCharPointer		:	result = (( uint8_t (*)(char *temp,uint32_t lenght))f)(para->fp.ADNet_CombinationUnion(fp,ADNet_ParaCharPointer),para->fpLenght);break;
		default:result = (( uint8_t (*)(void*,uint32_t))f)(0,0);break;
	}
	return result;
}


/*循环执行步骤*/
/**
 *@f xADNet				:	网络
*/
uint32_t ADNetRun(ADNet *xADNet)
{
	ADNetStep *realityRunStep;					/*实际的步骤*/
	unsigned char Result;
	void *cfun;
	unsigned long int ResendTime = 0xffffffff;
	unsigned long int NextExecutedTime = 0xffffffff;
	/*检查网络是否不为空*/
	if(xADNet==NULL)
	return NextExecutedTime;
	
	/*获取优先级最高的就绪步骤*/
	realityRunStep = ADNetGetHighestPrioStep(xADNet,&cfun);
	
	
	if(realityRunStep!=NULL)														/*判断是否有就绪的步骤*/
	{				
		/*	复制步骤。
			在后续执行该步骤的时候，如果在执行时该步骤发生了新的置1，同时又在该步骤里面执行了清零。那执行步骤时的置1将会被失效。
			这违背最后一次置1一定要被执行的原则。
			因此，这里需要在执行该步骤前将步骤复制一份。在执行该步骤后比较步骤的id和提前复制的id。
			如果一致就可以将该步骤清零。不一致就说明，在执行该步骤时发生置1操作，就不执行清零操作。
		*/
		memcpy(&xADNet->backupsRunStep,realityRunStep,sizeof(ADNetStep));
		
		if(xADNet->backupsRunStep.para.formalPara.fpType	==	ADNet_ParaDynamicPointer
			&&xADNet->backupsRunStep.para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer)!=NULL)
		{
			xADNet->backupsRunStep.para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer)	=	ADNetMalloc(realityRunStep->para.formalPara.fpLenght);
			
			if(xADNet->backupsRunStep.para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer)==NULL)
			{
				printf("ADNetRun:堆空间不足\r\n");
			}
			else 
			{
				memcpy(xADNet->backupsRunStep.para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer),
						realityRunStep->para.formalPara.fp.ADNet_CombinationUnion(fp,ADNet_ParaDynamicPointer),
						xADNet->backupsRunStep.para.formalPara.fpLenght);
			}
		}
		
		/*开始执行步骤*/
		Result = S_ADNetRunStep(cfun,&(xADNet->backupsRunStep.para.formalPara));
		

		/*清除备份的参数*/
		S_ADNetFreeFormalPara(&xADNet->backupsRunStep.para.formalPara);
		
		if(Result == ADNet_YES)																		/*步骤执行成功*/
		{			
			S_ADNetClearStep(xADNet,realityRunStep,&(xADNet->backupsRunStep));	
			
		}
		else																						/*步骤执行失败*/
		{
			/*步骤失败*/
			if(xADNet->stepFailCallBack!=NULL)
			{
				/*统计失败次数*/
				realityRunStep->sendFailCnt++;
								
				/*发送失败次数够了，调用发送失败回调函数*/
				if(realityRunStep->sendFailCnt>=xADNet->stepFailMaxTimes)
				{
					realityRunStep->sendFailCnt = 0;
					xADNet->stepFailCallBack(xADNet,cfun,realityRunStep->para.formalPara.id);
				}
			}
			
		}	
		
	}
	else 																						/*所有的步骤都被清零*/
	{	
		/*所有步骤都被清零了*/
		xADNet->isIdle	=	ADNet_YES;
		if(xADNet->isIdleCallBack!=NULL)
		{
			xADNet->isIdleCallBack(xADNet);
		}	
	}
/****************************************************************************************************/
	/*计算ADNetRun下一次需要被执行的时间由下面三个因素决定，并取下面三个时间的最小值。
		1，是否有设置了重发，如果有，需要计算出最小重发时间。
		2，通讯是否为空闲，如果非空闲，那时间就是0。
		3，是否还有定时就绪的步骤，如果有，需要计算出定时就绪步骤的最小时间。
	*/
	
	/*获取需要重发的时间*/
	ResendTime = ADNetGetNextResendTime(xADNet);
	//printf("ResendTime=%lu\r\n",ResendTime);
	/*如果需要重发的时间为0，那就说明有重发需要执行，那就执行重发*/
	if(ResendTime == 0)
	ADNetResendRun(xADNet);
	
	NextExecutedTime = ResendTime;
	
	/*如果该通讯不为空闲*/
//	if(xADNet->isIdle == ADNet_YES)
//	printf("空\r\n");
//	else if(xADNet->isIdle == ADNet_NO)
//	{
//		printf("非空\r\n");
//	}
//	else printf("待定\r\n");
	//printf("xADNet->isIdle=%d\r\n",xADNet->isIdle);
	if(xADNet->isIdle != ADNet_YES)
		NextExecutedTime = 0;
#if ADNet_config_EnStepTiming == 1
	else
	{		
		/*下一个定时就绪时间*/
		//printf("xADNet->MinTimingStepTime=%d\r\n",xADNet->MinTimingStepTime);
		if(NextExecutedTime>xADNet->MinTimingStepTime)
		NextExecutedTime = xADNet->MinTimingStepTime;	
	}
#endif	
	return NextExecutedTime;
}

#if ADNet_config_UseMacroAPI == 0
/*检查步骤是否为空*/
uint8_t ADNetIsIdle(ADNet *xADNet)
{
	return xADNet->isIdle;	
}
#endif
/*获取重发计数*/
unsigned int ADNetGetResendCnt(ADNet *xADNet)
{
	if(xADNet->backupsRunStep.reSend!=NULL)
	return xADNet->backupsRunStep.reSend->reSendCnt;
	else return 0;
}

/*获取id*/
uint32_t ADNetGetStepId(ADNet *xADNet)
{
	return xADNet->backupsRunStep.para.formalPara.id;

}

//返回步骤指针。
ADNetStep *ADNetGetStepThis(ADNet *xADNet)
{
	return &xADNet->backupsRunStep;
}

/*设置最大失败次数和回调函数*/
/**
 *@xADNet				:	网络
 *@times				:	次数
 *@stepFailCallBack		:	发送失败超过设定的次数回调

*/
unsigned char ADNetSetStepFailTimes(ADNet *xADNet,unsigned short int times,void  *stepFailCallBack)
{
	if(xADNet!=NULL)
	{
		xADNet->stepFailMaxTimes	=	times;
		xADNet->stepFailCallBack	=	(ADNetStepFailCallBack)stepFailCallBack;
		return ADNet_YES;
	}
	else return ADNet_NO;
}

/************************************************************************************************************************************/
/*停止全部重发*/
/**
 *@xADNet				:	网络
 *@f					:	步骤函数
 */
void ADNetResendStopAll(ADNet *xADNet)
{
	unsigned int lenght =0,ii=0 ,yy;	
	if(xADNet==NULL)
	{
		return ;
	}
	
	for(ii=0;ii<ADNet_config_MaxPrioGroupNum;ii++)
	{
		lenght = xADNet->StepAssembly[ii].lenght;
		for(yy=0;yy<lenght;yy++)
		ADNetResendStop(xADNet,xADNet->StepAssembly[ii].sFunc[yy]);
	}	
}
/*停止重发*/
/**
 *@xADNet				:	网络
 *@f					:	步骤函数
 */
void ADNetResendStop(ADNet *xADNet,void *f)
{

	ADNetStep *Step;
	
	if(xADNet==NULL)
	{
		return ;
	}

	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);	
	
	if(Step!=NULL)
	{
			
		/*删除当前步骤参数*/
		S_ADNetFreeFormalPara(&(Step->para.formalPara));
	
		/*清零*/
		S_ADNetSetReady(xADNet,Step,ADNet_NO);
	
		S_ADNetResendInit(Step);	
		
	}
}

/*开始重发*/
/**
 *@xADNet				:	网络
 *@f					:	步骤函数
 */
void ADNetResendStart(ADNet *xADNet,void *f,uint8_t yn)
{
	
	ADNetStep *Step;
	
	if(xADNet==NULL)
	{
		return ;
	}

	/*找到步骤的位置*/
	Step = ADNetGetStep(xADNet,f);	
	
	if(Step!=NULL)
	{		
		S_ADNetResendStart(Step,yn);
	}
}

/*设置自动重发*/
/**
 *@xADNet				:	网络
 *@f					:	步骤函数
 *@ReSendTimes			:	重发次数
 *@ReSendPeriod			:	重发间隔
 *@ReSendFailCallBack	:	重发失败回调
*/
void ADNetSetResend(ADNet *xADNet,void *f,unsigned int ReSendTimes,unsigned int reSendPeriod,void *reSendFailCallBack)
{
	ADNetStep *Step;
	
	if(xADNet==NULL)
	{
		return ;
	}
	
		/*找到步骤的位置*/
	Step 	= ADNetGetStep(xADNet,f);
	
	if(Step!=NULL)
	{		
		if(Step->reSend==NULL)
		{
			Step->reSend	=	ADNetMalloc(sizeof(ADNetResend));
			if(Step->reSend	==	NULL)
			{
				printf("ADNetSetResend : 堆空间不足\r\n");
				return ;
			}	
		}
		Step->reSend->reSendFailCallBack		=	(ADNetReSendFailCallBack)reSendFailCallBack;
		Step->reSend->reSendPeriod				=	reSendPeriod;
		Step->reSend->reSendTimes				=	ReSendTimes;
		Step->reSend->isReady					=	ADNet_NO;
		Step->reSend->reSendCnt					=	0;
#if ADNet_config_ResendUseFastArith == 1		
		S_ADNetGenerateResendFlag(xADNet);
#endif
	}
	
}
/*下一次执行的时间*/
uint32_t ADNetGetNextResendTime(ADNet *xADNet)
{
#if ADNet_config_ResendUseFastArith == 1
	uint8_t ii = 0,flag;
	uint16_t slotSize,yy = 0,offset = 0;
	uint32_t reSendTime = 0xffffffff;
	int32_t reSendTimeRemaining = reSendTime;
	
	if(xADNet == NULL)
	return reSendTimeRemaining;
	
	flag 		= 	xADNet->resendFlag;
	slotSize	=	xADNet->resendSlotSize[0];

	for(ii=0;ii<ADNet_config_ResendFlagSlotNum;ii++)
	{		
		if(flag&(0x1<<ii))
		{
			for(yy = 0;yy<xADNet->resendSlotSize[ii];yy++)
			{
				offset = ii*slotSize;
				if(xADNet->Step[offset+yy].reSend!=NULL)
				{	
					if(xADNet->Step[offset+yy].reSend->isReady!=ADNet_NO)
					{
					
						if( xADNet->Step[offset+yy].reSend->reSendStartTime<reSendTime)
						{
							reSendTime	= xADNet->Step[offset+yy].reSend->reSendStartTime;
						};
					}
					
				}
			}
		}
	}
		//printf("reSendTime=%d\r\n",reSendTime);
	if(reSendTime!=0xffffffff)
	{
		reSendTimeRemaining = reSendTime-__ADNetGetTick; 
		
		if(reSendTimeRemaining<0)
		reSendTimeRemaining = 0;
	}
	else 
	{
		reSendTimeRemaining = 0xffffffff;
	}
	
	return reSendTimeRemaining;
#else
	uint16_t ii = 0,yy,lenght = 0;		
	uint32_t reSendTime = 0xffffffff;
	ADNetStep *Step;
	int32_t reSendTimeRemaining = reSendTime;
	if(xADNet==NULL)
	{
		return reSendTimeRemaining;
	}
	
	for(ii = 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
	{
		lenght	=	xADNet->StepAssembly[ii].lenght;
		Step	=	xADNet->StepAssembly[ii].Step;
		//printf("lenght=%d,%d,%d\r\n",ii,lenght,xADNet->StepAssembly[ii].lenght);
		for(yy = 0;yy<lenght;yy++)
		{		
			if(Step[yy].reSend!=NULL)
			{
				if(Step[yy].reSend->isReady!=ADNet_NO)
				{
					if( Step[yy].reSend->reSendStartTime<reSendTime)
					{
						reSendTime	= Step[yy].reSend->reSendStartTime;
					};
				}
			}
		}
	}
		
	//printf("reSendTime=%d\r\n",reSendTime);
	if(reSendTime!=0xffffffff)
	{
		reSendTimeRemaining = reSendTime-__ADNetGetTick; 
		
		if(reSendTimeRemaining<0)
		reSendTimeRemaining = 0;
	}
	else 
	{
		reSendTimeRemaining = 0xffffffff;
	}
	return reSendTimeRemaining;
#endif
}

/*执行重发机制*/
void ADNetResendRun(ADNet *xADNet)
{
#if ADNet_config_ResendUseFastArith == 1
	uint8_t ii = 0,flag;
	uint16_t slotSize,yy = 0,offset = 0;
	
	if(xADNet == NULL)
	return ;
	
	flag 		= 	xADNet->resendFlag;
	slotSize	=	xADNet->resendSlotSize[0];

	for(ii=0;ii<ADNet_config_ResendFlagSlotNum;ii++)
	{		
		if(flag&(0x1<<ii))
		{
			for(yy = 0;yy<xADNet->resendSlotSize[ii];yy++)
			{
				offset = ii*slotSize;
				if(xADNet->Step[offset+yy].reSend!=NULL)
				{					
					S_ADNetResendProcess(xADNet,&xADNet->Step[offset+yy],offset+yy);
				}
			}
		}
	}
#else
	uint16_t ii = 0,yy = 0,lenght = 0;
	ADNetStep *Step;
	ADNetStepF *sFunc;
	
	if(xADNet==NULL)
	{
		return ;
	}
	for(ii= 0;ii<ADNet_config_MaxPrioGroupNum;ii++)
	{
		lenght 	=	xADNet->StepAssembly[ii].lenght;
		Step	=	xADNet->StepAssembly[ii].Step;
		sFunc 	= 	(ADNetStepF *)xADNet->StepAssembly[ii].sFunc;
		for(yy = 0;yy<lenght;yy++)
		{		
			if(Step[yy].reSend!=NULL)
			{
				S_ADNetResendProcess(xADNet,&Step[yy],sFunc[yy]);
			}
		}
	}	
#endif
}
/*查看步骤是否开启重发*/
uint8_t ADNetGetStepIsResendStart(ADNet *xADNet,void *f)
{
	ADNetStep *Step;
	if(xADNet==NULL)
	{
		return ADNet_NO;
	}
	
		/*找到步骤的位置*/
	Step 	= ADNetGetStep(xADNet,f);
	if(Step!=NULL)
	{
			if(Step->reSend != NULL)
			{
				return Step->reSend->isReady;
			}
			else 
			{
				return ADNet_NO;
			}
	}
	else return ADNet_NO;	
}
