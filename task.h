/**********************************************
 * 文件名：task.h
 * 编辑者：Li
 * 更新时间：2019.12.22
 * 说明：此文件为操作系统进程操作API
**********************************************/
#ifndef  __TASK_H
#define  __TASK_H

#include "OpenRTOS.h"


extern pTCB *RunningTask;             //此处只是指向正在运行的任务
extern TaskHandler IdleTaskHandle;          //空闲任务句柄


void IdleTaskApp(void *pIdle);        //空闲任务回调函数


/******空闲任务初始化***********************/
void IdleTaskInit(void);
/*****获取第一个需要运行的任务********/
pTCB *GetFirstHighTask(void);
/*********下面为静态创建任务***********************/
UINT8 pCreateTaskStc(INT8 *Name,FUN Fun,UINT8 *Stack,UINT8 Prio,TaskHandler *Handler);
/********下面动态创建任务******************************/
UINT8 pCreateTaskDyn(INT8 *Name,FUN Fun,UINT16 StackSize,UINT8 Prio,TaskHandler *Handler);
/*****删除任务********/
void pDeleteTask(TaskHandler Handler);
/******任务挂起函数********/
void pSuspendTask(TaskHandler Handler);
/******刷新阻塞链表*********/
void RefreshBlkList(void);
/******系统延时函数********/
void pDelayTask(UINT16 Time);
#if(SYS_LONG_DELAY==1)
    /******系统长延时函数********/
    void pLongDelayTask(UINT32 Time);
#endif


#endif


