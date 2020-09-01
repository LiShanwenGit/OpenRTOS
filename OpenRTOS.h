/****************************************
 * 文件名：OpenRTOS.h
 * 编写人：
 * 更新时间：2019.12.22
 * 说明：本文件为整个操作系统所有的头文件包含信息
 **************************************/
#ifndef __OPENRTOS_H
#define __OPENRTOS_H

#include "type.h"
#include "list.h"
#include "heap.h"
#include "task.h"
#include "core.h"
#include "OpenRTOSconfig.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/iom128.h>

#include "stdio.h"

#define  ENTER_CRITICAL()   DisableIntrp()    //关中断
#define  EXIT_CRITICAL()    EnableIntrp()     //开中断

#endif


