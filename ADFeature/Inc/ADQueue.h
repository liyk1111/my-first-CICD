#ifndef __ADQUEUE_H
#define __ADQUEUE_H

#define config_En_ASSERT		1

#if config_En_ASSERT == 1
	#define assert_ADQueue(Q)	((Q) ? (void)0U : assert_failed((uint8_t *)__FILE__, __LINE__))
#else 
	#define assert_ADQueue(Q)	((void)0U)
#endif

#define ADQueue_DynamicPara				0//动态参数
#define ADQueue_StaticPara				1//静态参数

#define ADQueue_uint8_t			0
#define ADQueue_uint16_t		1
#define ADQueue_uint32_t		2
#define ADQueue_float			3
#define ADQueue_double			4


#define ADQMalloc 	malloc
#define ADQFree 	free

#define YES 1
#define NO	0
	
typedef struct 
{
	void *QueueBody;
	signed int front;      //头元素下标 
	signed int rear;		//尾元素下标 若队列不空，指向队列尾元素的下一个位置
	unsigned int QueueSize;
	unsigned char ItemSize;
	unsigned char BodyType;

}ADQueue;


/*创建一个队列*/
//QueueSize:队列长度
//ItemSize 队列元素的长度
ADQueue *ADQueueCreat(unsigned char ItemSize,unsigned int QueueSize);

/*创建一个队列,不使用动态内存*/
//QueueSize:队列长度
//ItemSize 队列元素的长度
void ADQueueCreatStatic(ADQueue *Q,void *Body,unsigned char ItemSize,unsigned int QueueSize);

//************************************
//brief:	  获取队列长度
// Author:    詹乾坤
//************************************
unsigned int ADQueueGetLength(ADQueue *Q);

//************************************
//brief:	  向队列尾插入一个元素
// Author:    詹乾坤
//************************************
unsigned char ADQueueInsertElem(ADQueue *Q,void *e);


//************************************
//brief:	  向队列头插入一个元素(插队)
// Author:    詹乾坤
//************************************
unsigned char ADQueuePreemptInsertElem(ADQueue *Q,void *e);

//************************************
//brief:	  向队列尾插入一个元素,如果满了就挤掉第一个
// Author:    詹乾坤
//************************************
unsigned char ADQueueInsertElemS(ADQueue *Q,void *e);


//************************************
//brief:	  向队列头插入一个元素(插队),如果满了就挤掉第一个
// Author:    詹乾坤
//************************************
unsigned char ADQueuePreemptInsertElemS(ADQueue *Q,void *e);

//************************************
//brief:	  向队列读取并删除一个元素
// Author:    詹乾坤
//************************************
unsigned char  ADQueueDeleteElem(ADQueue *Q, void *e);


//************************************
//brief:	  删除链表中全部元素
// Author:    詹乾坤
//************************************
unsigned char  ADQueueDeleteAllElem(ADQueue *Q);

//删除整个队列并释放内存
unsigned char ADQueueDeleteQueue(ADQueue *Q);

/*获取队列中某个元素*/
//n:第几个元素
unsigned char ADQueueGetElem(ADQueue *Q,unsigned int n,void *e);
/*获取整个队列*/
unsigned int ADQueueGetQueue(ADQueue *Q,void *xQueue);
/*找到某一个元素，并返回在队列的位置*//*找到某一个元素，并返回在队列的位置,返回0则代表没有这个元素*/
unsigned int ADQueueFineXElem(ADQueue *Q,void *e);
/*************************************************************************/
//对队列求和
unsigned char ADQueueSum(ADQueue *Q,unsigned char ItemType,void *Result);
//对队列求平均
unsigned char ADQueueAverage(ADQueue *Q,unsigned char ItemType,void *Result);
/*对队列求最大值*/
unsigned char ADQueueMaxValue(ADQueue *Q,unsigned char ItemType,void *Result);
/*对队列求最小值*/
unsigned char ADQueueMinValue(ADQueue *Q,unsigned char ItemType,void *Result);
/*对队列求标准差*/
unsigned char ADQueueStandardDeviation(ADQueue *Q,unsigned char ItemType,void *Result);
/*对队列求方差*/
unsigned char ADQueueVariance(ADQueue *Q,unsigned char ItemType,void *Result);
#endif
