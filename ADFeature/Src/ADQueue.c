#include "ADQueue.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

/*
*作者：詹乾坤

*V2.0.1-2023-06-07-1:添加求方差函数ADQueueVariance
*V2.0.2-2024-03-12-1:修复BUG,添加标准差接口
*/
void S_ADQueueClearAllElem(ADQueue *Q)
{
	if(Q == NULL)
	return ;
	Q->front = -1;
	Q->rear = -1;
}

/*创建一个队列*/
//QueueSize:队列长度
//ItemSize 队列元素的长度
ADQueue *ADQueueCreat(unsigned char ItemSize,unsigned int QueueSize)
{
	ADQueue *Q;
	Q = ADQMalloc(sizeof(ADQueue));
	if(Q!=NULL)
	{
		//QueueSize = QueueSize + 1;
		S_ADQueueClearAllElem(Q);
		Q->ItemSize = ItemSize;
		Q->QueueSize = QueueSize;
		Q->QueueBody =  ADQMalloc((QueueSize)*ItemSize);
		if(Q->QueueBody != NULL)
		{
			memset(Q->QueueBody,0,QueueSize*ItemSize);
			Q->BodyType = ADQueue_DynamicPara;
			return Q;
		}
		else ADQFree(Q);
		return NULL;
	}
	return NULL;
}

/*创建一个队列,不使用动态内存*/
//QueueSize:队列长度,但是实际可插入的元素个数只有QueueSize-1
//ItemSize 队列元素的长度
void ADQueueCreatStatic(ADQueue *Q,void *Body,unsigned char ItemSize,unsigned int QueueSize)
{
	S_ADQueueClearAllElem(Q);
	Q->ItemSize = ItemSize;
	Q->QueueSize = QueueSize;
	Q->QueueBody = Body;
	Q->BodyType = ADQueue_StaticPara;
}

//brief:	  获取队列长度
unsigned int ADQueueGetLength(ADQueue *Q)
{

	if(Q==NULL)
		return 0;
		
	if(Q->front == -1 || Q->rear==-1)
	{
		return 0;
	}
	else if(Q->rear >= Q->front )
	{
		return Q->rear - Q->front + 1;
	}
	else 
	{
		return  Q->QueueSize -  ((Q->front - Q->rear) -1) ;
	}

}


//************************************
//brief:	  向队列插入一个元素
// Author:    詹乾坤
//************************************
unsigned char ADQueueInsertElem(ADQueue *Q,void *e)
{
	unsigned char *p;
	if(Q == NULL)
		return NO;
	if(Q->front == -1 && Q->rear==-1)
	{
		Q->front = 0;
		Q->rear = 0;
	}
	else 
	{
		if ((Q->rear + 1) % Q->QueueSize == Q->front) 	//队列满的判断
			return NO;
		else Q->rear = (Q->rear + 1) % Q->QueueSize ;	  	//rear指针向后移动一个位置 //若到最后则转到数组头部
	}
	
	
	
	/*将元素写入队列*/
	p = Q->QueueBody;
	p = p+Q->ItemSize*Q->rear;	
	memcpy(p,e,Q->ItemSize);				//复制元素值
	
	
	
																				
	return YES;
}
//************************************
//brief:	  向队列头插入一个元素(插队)
// Author:    詹乾坤
//************************************
unsigned char ADQueuePreemptInsertElem(ADQueue *Q,void *e)
{
	unsigned char *p;
	
	if(Q == NULL)
		return NO;
	
	if ((Q->rear + 1) % Q->QueueSize == Q->front) 	//队列满的判断
	{
		return NO;
	}
	
	if(Q->front == -1)
	{
		Q->front = 0;
		Q->rear = 0;
	}
	else if(Q->front == 0)
	{
		Q->front = Q->QueueSize - 1;
	}
	else 
	{
		Q->front = Q->front - 1;
	}
	
	p = Q->QueueBody;
	p = p+Q->ItemSize*Q->front;
	
	memcpy(p,e,Q->ItemSize);				//复制元素值
																				
	return YES;
}
//************************************
//brief:	  向队列尾插入一个元素,如果满了就挤掉对列头一个元素
// Author:    詹乾坤
//************************************
unsigned char ADQueueInsertElemS(ADQueue *Q,void *e)
{
	unsigned char *p;
	if(Q == NULL)
		return NO;

	if(Q->front == -1 && Q->rear==-1)
	{
		Q->front = 0;
		Q->rear = 0;
	}
	else 
	{
		if ((Q->rear + 1) % Q->QueueSize == Q->front) 	//队列满的判断
		{
			Q->front = (Q->front + 1) % Q->QueueSize ;	  	//rear指针向后移动一个位置 //若到最后则转到数组头部
		}			
		Q->rear = (Q->rear + 1) % Q->QueueSize ;	  	//rear指针向后移动一个位置 //若到最后则转到数组头部
	}
	
	
	
	/*将元素写入队列*/
	p = Q->QueueBody;
	p = p+Q->ItemSize*Q->rear;	
	memcpy(p,e,Q->ItemSize);				//复制元素值
	
	
	
																			
	return YES;
}
//************************************
//brief:	  向队列头插入一个元素(插队),如果满了就挤掉队列尾部最后一个元素
// Author:    詹乾坤
//************************************
unsigned char ADQueuePreemptInsertElemS(ADQueue *Q,void *e)
{
	unsigned char *p;
	if(Q == NULL)
		return NO;
	
	
	if(Q->front == -1)/*队列为空*/
	{
		Q->front = 0;
		Q->rear = 0;
	}
	else if ((Q->rear + 1) % Q->QueueSize == Q->front) 	//队列已满
	{
		if(Q->rear == 0)
		Q->rear = Q->QueueSize - 1;
		else Q->rear = Q->rear - 1;
		
		if(Q->front == 0)
		Q->front = Q->QueueSize - 1;
		else Q->front = Q->front - 1;
	}
	else 
	{
		if(Q->front == 0)
		Q->front = Q->QueueSize - 1;
		else Q->front = Q->front - 1;
	}
	
	

	
	p = Q->QueueBody;
	p = p+Q->ItemSize*Q->front;
	
	memcpy(p,e,Q->ItemSize);				//复制元素值
																				
	return YES;
}

//************************************
//brief:	  向队列读取并删除一个元素
// Author:    詹乾坤
//************************************
unsigned char  ADQueueDeleteElem(ADQueue *Q, void *e)
{
	unsigned char *p;
	if (ADQueueGetLength(Q) == 0)					//判断队列是否为空
		return NO;
	
	/*在队列中找到元素*/
	p = Q->QueueBody;
	p = p+Q->ItemSize*Q->front;
	memcpy(e,p,Q->ItemSize);			//把队头元素进行出队操作
	
	if((Q->front ) % Q->QueueSize == Q->rear)
	{
		S_ADQueueClearAllElem(Q);
	}
	else
	Q->front = (Q->front + 1) % Q->QueueSize;	//删除一个元素
	return YES;
}
//************************************
//brief:	  删除链表中全部元素
// Author:    詹乾坤
//************************************
unsigned char  ADQueueDeleteAllElem(ADQueue *Q)
{
	if (ADQueueGetLength(Q) == 0)					//判断队列是否为空
		return NO;

	S_ADQueueClearAllElem(Q);
	return YES;
}
//删除整个队列,如果是动态内存，那就释放内存
unsigned char ADQueueDeleteQueue(ADQueue *Q)
{
	if(Q==NULL)
		return NO;
	
	if(Q->BodyType == ADQueue_DynamicPara)	//动态内存
	{
		S_ADQueueClearAllElem(Q);
		ADQFree(Q->QueueBody);
		ADQFree(Q);		
	}
	else 
	{
		S_ADQueueClearAllElem(Q);
	}
	
	return YES;
}


/*获取队列中某个元素*/
//n:第几个元素
unsigned char ADQueueGetElem(ADQueue *Q,unsigned int n,void *e)
{
	unsigned char *p;
	unsigned int offest;
	unsigned int lenght = 0;
	
	if(Q==NULL)
	return NO;
	
	lenght = ADQueueGetLength(Q);	
	/*找不到这个元素，或者队列为空*/
	if(lenght<n||n==0||lenght==0)				//没有这个元素
		return NO;
	
	offest = n - 1 + Q->front;
	offest = offest%Q->QueueSize;
	
	p = Q->QueueBody;
	p = p+Q->ItemSize*offest;
	
	memcpy(e,p,Q->ItemSize);			//把队头元素进行出队操作
	
	return YES;
}

/*获取整个队列*/
/*并返回队列长度*/
unsigned int ADQueueGetQueue(ADQueue *Q,void *xQueue)
{
	unsigned char *p,*q;
	unsigned int lenght;
	
	if(Q == NULL)
		return NO;
	lenght = ADQueueGetLength(Q);
	if (lenght==0)					//判断队列是否为空
		return NO;
	

	p = Q->QueueBody;
	p = p +Q->front*Q->ItemSize;
	q = xQueue;
	
	
	if(Q->rear>=Q->front)/*当rear大于或等于front时*/
		memcpy(q,p,Q->ItemSize*lenght);	
	else 
	{
		unsigned int temp;
		/*当rear小于front时，那就分成两段复制*/
		temp = Q->QueueSize - Q->front;
		memcpy(q,p,Q->ItemSize*temp);
		
		q = q + Q->ItemSize*temp;
		p = Q->QueueBody;
		
		temp = lenght - temp;
		 
		memcpy(q,p,Q->ItemSize*temp);
	}
		
	return lenght;
}
/*找到某一个元素，并返回在队列的位置,返回0则代表没有这个元素*/
unsigned int ADQueueFineXElem(ADQueue *Q,void *e)
{
	unsigned char *p;
	unsigned int offest;
	unsigned int temp = 1;
	unsigned int lenght =0 ;
	

	lenght = ADQueueGetLength(Q);
	if (lenght ==0)					//判断队列是否为空
		return NO;
		
	offest = Q->front;
	
	p = Q->QueueBody;	
	p = p + offest*Q->ItemSize;
	while(1)
	{		
		if(memcmp(p,e,Q->ItemSize)==0)
		{					
			break;
		}
		else 
		{
			offest = (offest+1) % Q->QueueSize;			
			if(offest == Q->rear)
			{
				temp = NO;
				break;
			}
			p = Q->QueueBody;
			p = p+ Q->ItemSize*offest;	
			temp++;
		}
	}
	return temp;
}
/****************************************************************************************/
//对队列求和
unsigned char ADQueueSum(ADQueue *Q,unsigned char ItemType,void *Result)
{
	unsigned char offest = 0;
	unsigned int Temp32 = 0;
	float TempFloat = 0;
	double TempDouble = 0;
	if (ADQueueGetLength(Q) == 0)					//判断队列是否为空
	{
		return NO;
	}
	
	offest = Q->front;
	switch(ItemType)
	{
		case ADQueue_uint8_t	:	
		{
			unsigned char *p = Q->QueueBody;
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + p[offest];				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned int*)Result = Temp32;
			
		}break;
		case ADQueue_uint16_t	:	
		{
			unsigned short int *p = Q->QueueBody;
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + p[offest];				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned int*)Result = Temp32;
		}break;
		case ADQueue_uint32_t	:	
		{
			unsigned long int *p = Q->QueueBody;
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + p[offest];				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned long int*)Result = Temp32;
		}break;
		case ADQueue_float		:	
		{
			float *p = Q->QueueBody;
			TempFloat = 0;
			
			while(1)
			{
				TempFloat = TempFloat + p[offest];								
				if(offest == Q->rear)
				{						
					break;		
				}
				offest = (offest+1)%Q->QueueSize;					
			}
			*(float*)Result = TempFloat;
		}break;
		case ADQueue_double		:	
		{
			double *p = Q->QueueBody;
			TempDouble = 0;
			while(1)
			{
				TempDouble = TempDouble + p[offest];				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(double*)Result = TempDouble;
		}break;
	}
	return YES;
}

//对队列求平均
unsigned char ADQueueAverage(ADQueue *Q,unsigned char ItemType,void *Result)
{
	unsigned char offest = 0;
	unsigned int Temp32 = 0;
	float TempFloat = 0;
	double TempDouble = 0;
	unsigned int len;
	if (ADQueueGetLength(Q) == 0)						//判断队列是否为空
		return NO;
	len = ADQueueGetLength(Q);
	offest = Q->front;
	switch(ItemType)
	{
		case ADQueue_uint8_t	:	
		{
			unsigned char *p = Q->QueueBody;
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + p[offest];
				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned char*)Result = Temp32/len;
			
		}break;
		case ADQueue_uint16_t	:	
		{
			unsigned short int *p = Q->QueueBody;
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + p[offest];				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned short int*)Result = Temp32/len;
		}break;
		case ADQueue_uint32_t	:	
		{
			unsigned long int *p = Q->QueueBody;
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + p[offest];
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned long int*)Result = Temp32/len;
		}break;
		case ADQueue_float		:	
		{
			float *p = Q->QueueBody;
			TempFloat = 0;
			while(1)
			{
				TempFloat = TempFloat + p[offest];				
				if(offest == Q->rear)
					break;			
				offest = (offest+1)%Q->QueueSize;		
			}
			*(float*)Result = TempFloat/len;
		}break;
		case ADQueue_double		:	
		{
			double *p = Q->QueueBody;
			TempDouble = 0;
			while(1)
			{
				TempDouble = TempDouble + p[offest];				
				if(offest == Q->rear)
					break;			
				offest = (offest+1)%Q->QueueSize;		
			}
			*(double*)Result = TempDouble/len;
		}break;
	}
	return YES;
	
}

/*对队列求最大值*/
unsigned char ADQueueMaxValue(ADQueue *Q,unsigned char ItemType,void *Result)
{
	unsigned char offest = 0;
	unsigned int Temp32 = 0;
	float TempFloat = 0;
	double TempDouble = 0;
	if (ADQueueGetLength(Q) == 0)					//判断队列是否为空
		return NO;
	
	offest = Q->front;
	switch(ItemType)
	{
		case ADQueue_uint8_t	:	
		{
			unsigned char *p = Q->QueueBody;
			Temp32 = p[offest];
			while(1)
			{
				if(Temp32<p[offest])
				{
					Temp32 = p[offest];
				}
				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned char*)Result = Temp32;
			
		}break;
		case ADQueue_uint16_t	:	
		{
			unsigned short int *p = Q->QueueBody;
			Temp32 = p[offest];
			while(1)
			{
				if(Temp32<p[offest])
				{
					Temp32 = p[offest];
				}
				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned short int*)Result = Temp32;
		}break;
		case ADQueue_uint32_t	:	
		{
			unsigned long int *p = Q->QueueBody;
			Temp32 = p[offest];
			while(1)
			{
				if(Temp32<p[offest])
				{
					Temp32 = p[offest];
				}
				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned long int*)Result = Temp32;
		}break;
		case ADQueue_float		:	
		{
			float *p = Q->QueueBody;
			TempFloat = p[offest];
			while(1)
			{
				if(TempFloat<p[offest])
				{
					TempFloat = p[offest];
				}
				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(float*)Result = TempFloat;
		}break;
		case ADQueue_double		:	
		{
			double *p = Q->QueueBody;
			TempDouble = p[offest];
			while(1)
			{
				if(TempDouble<p[offest])
				{
					TempDouble = p[offest];
				}
				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(double*)Result = TempDouble;
		}break;
		default:return NO;
	}
	return YES;
}
/*对队列求最小值*/
unsigned char ADQueueMinValue(ADQueue *Q,unsigned char ItemType,void *Result)
{
	unsigned char offest = 0;
	unsigned int Temp32 = 0;
	float TempFloat = 0;
	double TempDouble = 0;
	if (ADQueueGetLength(Q) == 0)						//判断队列是否为空
		return NO;
	
	offest = Q->front;
	switch(ItemType)
	{
		case ADQueue_uint8_t	:	
		{
			unsigned char *p = Q->QueueBody;
			Temp32 = p[offest];
			while(1)
			{
				if(Temp32>p[offest])
				{
					Temp32 = p[offest];
				}
				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned char*)Result = Temp32;
			
		}break;
		case ADQueue_uint16_t	:	
		{
			unsigned short int *p = Q->QueueBody;
			Temp32 = p[offest];
			while(1)
			{
				if(Temp32>p[offest])
				{
					Temp32 = p[offest];
				}
				
				if(offest == Q->rear)
					break;			
				offest = (offest+1)%Q->QueueSize;		
			}
			*(unsigned short int*)Result = Temp32;
		}break;
		case ADQueue_uint32_t	:	
		{
			unsigned long int *p = Q->QueueBody;
			Temp32 = p[offest];
			while(1)
			{
				if(Temp32>p[offest])
				{
					Temp32 = p[offest];
				}
				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned long int*)Result = Temp32;
		}break;
		case ADQueue_float		:	
		{
			float *p = Q->QueueBody;
			TempFloat = p[offest];
			while(1)
			{
				if(TempFloat>p[offest])
				{
					TempFloat = p[offest];
				}
				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(float*)Result = TempFloat;
		}break;
		case ADQueue_double		:	
		{
			double *p = Q->QueueBody;
			TempDouble = p[offest];
			while(1)
			{
				if(TempDouble>p[offest])
				{
					TempDouble = p[offest];
				}
				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(double*)Result = TempDouble;
		}break;
		default:return NO;
	}
	return YES;
}

/*对队列求标准差*/
unsigned char ADQueueStandardDeviation(ADQueue *Q,unsigned char ItemType,void *Result)
{
	unsigned char offest = 0;
	unsigned int Temp32 = 0;
	float TempFloat = 0;
	double TempDouble = 0;
	unsigned char AverageValue8 = 0;
	unsigned short int AverageValue16 = 0;
	unsigned int AverageValue32 = 0;
	double AverageValueDouble =0;
	float AverageValueFloat =0;
	unsigned int len;
	if (ADQueueGetLength(Q) == 0)					//判断队列是否为空
		return NO;
	len = ADQueueGetLength(Q);
	offest = Q->front;
	
	switch(ItemType)
	{
		case ADQueue_uint8_t	:	
		{
			unsigned char *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValue8);
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + (p[offest]-AverageValue8)*(p[offest]-AverageValue8);				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned char*)Result = sqrt(Temp32/len);
			
		}break;
		case ADQueue_uint16_t	:	
		{
			unsigned short int *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValue16);
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + (p[offest]-AverageValue16)*(p[offest]-AverageValue16);				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned short int*)Result = sqrt(Temp32/len);
		}break;
		case ADQueue_uint32_t	:	
		{
			unsigned long int *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValue32);
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + (p[offest]-AverageValue32)*(p[offest]-AverageValue32);
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned long int*)Result = sqrt(Temp32/len);
		}break;
		case ADQueue_float		:	
		{
			float *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValueFloat);
			//printf("AverageValueFloat=%.2f\r\n",AverageValueFloat);
			TempFloat = 0;
			while(1)
			{
				TempFloat = TempFloat + (AverageValueFloat-p[offest])*(AverageValueFloat-p[offest]);				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(float*)Result = sqrt(TempFloat/len);
		}break;
		case ADQueue_double		:	
		{
			double *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValueDouble);
			TempDouble = 0;
			while(1)
			{
				TempDouble = TempDouble + (AverageValueDouble-p[offest])*(AverageValueDouble-p[offest]);				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(double*)Result = sqrt(TempDouble/len);
		}break;
		default:return NO;
	}
	return YES;
}
/*对队列求方差*/
unsigned char ADQueueVariance(ADQueue *Q,unsigned char ItemType,void *Result)
{
	unsigned char offest = 0;
	unsigned int Temp32 = 0;
	float TempFloat = 0;
	double TempDouble = 0;
	unsigned char AverageValue8 = 0;
	unsigned short int AverageValue16 = 0;
	unsigned int AverageValue32 = 0;
	double AverageValueDouble =0;
	float AverageValueFloat =0;
	unsigned int len;
	if (ADQueueGetLength(Q) == 0)					//判断队列是否为空
		return NO;
	len = ADQueueGetLength(Q);
	offest = Q->front;
	
	switch(ItemType)
	{
		case ADQueue_uint8_t	:	
		{
			unsigned char *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValue8);
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + (p[offest]-AverageValue8)*(p[offest]-AverageValue8);				
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned char*)Result = Temp32/len;
			
		}break;
		case ADQueue_uint16_t	:	
		{
			unsigned short int *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValue16);
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + (p[offest]-AverageValue16)*(p[offest]-AverageValue16);				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(unsigned short int*)Result = Temp32/len;
		}break;
		case ADQueue_uint32_t	:	
		{
			unsigned long int *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValue32);
			Temp32 = 0;
			while(1)
			{
				Temp32 = Temp32 + (p[offest]-AverageValue32)*(p[offest]-AverageValue32);
				if(offest == Q->rear)
					break;	
				offest = (offest+1)%Q->QueueSize;				
			}
			*(unsigned long int*)Result = Temp32/len;
		}break;
		case ADQueue_float		:	
		{
			float *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValueFloat);
			//printf("AverageValueFloat=%.2f\r\n",AverageValueFloat);
			TempFloat = 0;
			while(1)
			{
				TempFloat = TempFloat + (AverageValueFloat-p[offest])*(AverageValueFloat-p[offest]);				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(float*)Result = TempFloat/len;
		}break;
		case ADQueue_double		:	
		{
			double *p = Q->QueueBody;
			ADQueueAverage(Q,ItemType,&AverageValueDouble);
			TempDouble = 0;
			while(1)
			{
				TempDouble = TempDouble + (AverageValueDouble-p[offest])*(AverageValueDouble-p[offest]);				
				if(offest == Q->rear)
					break;		
				offest = (offest+1)%Q->QueueSize;			
			}
			*(double*)Result = TempDouble/len;
		}break;
	}
	return YES;
	
}


