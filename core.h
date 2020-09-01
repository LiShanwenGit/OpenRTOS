/**********************************************
 * 文件名：core.h
 * 编辑者：Li
 * 更新时间：2019.12.22
 * 说明：此文件为操作系统核心文件，滴答时钟初始化，进程切换，进入/退出临界区
**********************************************/
#ifndef __CORE_H
#define __CORE_H

#include "OpenRTOS.h"

//关系统中断
void DisableIntrp(void);
//开系统中断
void EnableIntrp(void);

/*****进行任务上下文切换******/
void TaskContexSwitch(void);
void TaskContexSwitchDelay(void);
/******任务开始调度函数********/
void pStartSchedule(void);
void OpenRTOSInit(void);
#endif

