#ifndef __ADLINKLIST_H
#define __ADLINKLIST_H
#include "stdint.h"

#define ADLinkList_Malloc 	malloc
#define ADLinkList_Free 	free

#define ADLinkList_ERR  0
#define ADLinkList_OK	1


#define ADLinkList_uint8_t			0
#define ADLinkList_uint16_t			1
#define ADLinkList_uint32_t			2
#define ADLinkList_float			3
#define ADLinkList_double			4

#define ADLinkList_char				5
#define ADLinkList_string			6


typedef struct ADLinkListElm
{
	void *LinkListBody;
	struct ADLinkListElm *next;
}ADLinkListElm;


typedef struct 
{
	ADLinkListElm *LinkListHead;
	unsigned char ItemSize;
}ADLinkList;


/*第一个元素不使用*/
void *ADListLinkCreat(uint8_t ItemSize);
/*打印链表*/
void ADLinkListPrintf(void *xADLinkList,uint8_t type);
/*插入元素*/
uint8_t ADLinkListInsert(void *xADLinkList,uint16_t n,void *e);
/*删除并获取元素*/
uint8_t ADLinkListDelete(void *xADLinkList,uint16_t n,void *e);
/*链表求和*/
uint8_t  ADLinkListSum(void *xADLinkList,uint8_t type,void *Sum);
/*链表求平均*/
uint8_t  ADLinkListAverage(void *xADLinkList,uint8_t type,void *average);
#endif
