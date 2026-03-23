#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "main.h"

#define QUEUE_LENGTH 			16		//队列长度限制-实际15个队列，若大于255，要修改变量类型
#define QUEUE_BUF_LENGTH_MAX 	100		//每个队列数据长度限制
#define QUEUE_LIST_LENGTH		2		//队列个数

//结构体声明
typedef struct
{ 
	uint8_t strHeadPtr; 																				//头指针 
	uint8_t strTailPtr; 																				//尾指针 
	uint16_t strQueueLenghtBuf[QUEUE_LENGTH];
	uint8_t strQueueBuf[QUEUE_LENGTH][QUEUE_BUF_LENGTH_MAX];		
}QUEUE_LIST;

extern QUEUE_LIST gQueueList[QUEUE_LIST_LENGTH];

void Queue_Data_Clear(QUEUE_LIST *);
extern uint8_t Queue_Data_GetLength(QUEUE_LIST *);
extern uint8_t Queue_Data_Fill(QUEUE_LIST *,uint8_t *buf, uint16_t length, uint8_t num);
extern uint8_t Queue_Data_Get(QUEUE_LIST *,uint8_t *buf, uint16_t *length);


#endif

