/**********************************************
 * 文件名：list.h
 * 编辑者：Li
 * 更新时间：2019.12.22
 * 说明：此文件为操作系统所有链表相关文件
 * 包括就绪链表，阻塞链表，挂起链表
**********************************************/
#ifndef __LIST_H
#define __LIST_H

#include "type.h"
#include "OpenRTOSconfig.h"

/******任务控制块结构体定义*******/
typedef struct PCB_STRUCT
{
    struct PCB_STRUCT *TCBPrev;       //前趋指针
    struct PCB_STRUCT *TCBNext;       //后继指针
    UINT8             *TCBStack;      //人工堆栈
    INT8              *TCBName;       //任务名
    FUN                TCBTask;       //任务函数指针
    #if(SYS_LONG_DELAY==1)
        UINT32         TCBLongDlyCount;   //长延时计数器
    #endif
    UINT16             TCB_TopSP;     //任务当前的SP指针
    UINT16             TCBDlyCount;   //延时计数器
    UINT8              TCBState;      //任务状态       255为头部尾部    0表示阻塞态，1表示就绪态，2表示挂起态
    UINT8              TCBPrio;       //任务优先级


}pTCB;
extern pTCB *RdyListIndex[PRIO_DIFF_SIZE+1];    //就绪链表索引
extern UINT8 RdyListNum[PRIO_DIFF_SIZE+1];      //就绪任务表    0下标表示空闲任务
extern pTCB *pBlkList;                          //阻塞链表
/****删除链表一个特定的节点后****/
void ListRemoveNode(pTCB *Remove);
/****将控制块插入到就绪链表头部后****/
void InsertRdyList(UINT8 Prio,pTCB *Insert);
/****阻塞链表插入******/
void InsertBlkList(pTCB *Insert);
/****从空白链表的表头获取一个任务控制块********/
pTCB *GetBlankTCB(void);
/****插入到空白链表中************/
void InsertBlankList(pTCB *Insert);
/****插入到挂起链表************/
void InsertSuspList(pTCB *Insert);
/***对所有的任务进行注册并存放到空白链表中*******/
UINT8 BlankRegisterAll(void);
/***初始化就绪链表索引，将所有任务插入到索引表中*******/
void RdyListIndexInit(void);
/****初始化就绪任务表，将空闲任务设置为就绪态，其他所有空白任务设置为阻塞状态******/
void RdyListNumInit(void);
/****阻塞链表初始化*******/
void BlkLisInit(void);
/****挂起链表初始化********/
void SuspListInit(void);

void RdyListSprint(void);

#endif


