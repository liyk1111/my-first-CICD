#include "../Inc/queue.h"
#include "main.h"
#include "retarget.h"



QUEUE_LIST gQueueList[QUEUE_LIST_LENGTH];

//清消息队列-会被打断，而且会嵌套
void Queue_Data_Clear(QUEUE_LIST *tmpQueueList)
{ 
	tmpQueueList->strHeadPtr = 0;
	tmpQueueList->strTailPtr = 0;
}

//获取队列长度-会被打断，而且会嵌套
uint8_t Queue_Data_GetLength(QUEUE_LIST *tmpQueueList)
{ 
	uint8_t ucLen = 0; 

	if((tmpQueueList->strHeadPtr >= QUEUE_LENGTH) || (tmpQueueList->strTailPtr >= QUEUE_LENGTH)) 	//理论上修复后就不会进入这个函数了。
		Queue_Data_Clear(tmpQueueList); 

	if( tmpQueueList->strTailPtr >= tmpQueueList->strHeadPtr) 				//这个长度只是作为能否写或者能否读而已，是否丢包而已，不会导致死机
		ucLen = tmpQueueList->strTailPtr - tmpQueueList->strHeadPtr; 
	else 
		ucLen = tmpQueueList->strTailPtr + QUEUE_LENGTH - tmpQueueList->strHeadPtr; 

	return ucLen;
}

//往队列里添加消息
uint8_t Queue_Data_Fill(QUEUE_LIST *tmpQueueList, uint8_t *buf, uint16_t length, uint8_t num)
{ 
	uint8_t ucLen = 0; 
	uint16_t cnt;
	ucLen = Queue_Data_GetLength(tmpQueueList); 

	if(ucLen >= (QUEUE_LENGTH-1)) //满 
	{ 
		printf("Queue%d is Full!\r\n", num);
		return 0; 
	} 

	if(length > (QUEUE_BUF_LENGTH_MAX-1)) 
		length = (QUEUE_BUF_LENGTH_MAX-1); 

	tmpQueueList->strQueueLenghtBuf[tmpQueueList->strTailPtr] = length; 

	for(cnt = 0; cnt < length; cnt++) 
		tmpQueueList->strQueueBuf[tmpQueueList->strTailPtr][cnt] = buf[cnt];

	if(tmpQueueList->strTailPtr == QUEUE_LENGTH-1)
		tmpQueueList->strTailPtr = 0;
	else
		tmpQueueList->strTailPtr++; 

	return 1;
}

//从队列获取消息-会被打断
uint8_t Queue_Data_Get(QUEUE_LIST *tmpQueueList, uint8_t *buf, uint16_t *length)
{ 
	uint8_t ucLen;
	uint16_t cnt,dataLength; 
	ucLen = Queue_Data_GetLength(tmpQueueList); 

	if(ucLen == 0) //空 
	{ 
		return 0; 
	} 
	
	*length = tmpQueueList->strQueueLenghtBuf[tmpQueueList->strHeadPtr]; 
	dataLength = *length; 

	for(cnt = 0; cnt < dataLength; cnt++)
		buf[cnt] = tmpQueueList->strQueueBuf[tmpQueueList->strHeadPtr][cnt]; 

	if(tmpQueueList->strHeadPtr == QUEUE_LENGTH-1)
		tmpQueueList->strHeadPtr = 0;
	else
		tmpQueueList->strHeadPtr++; 

	return 1;
}




