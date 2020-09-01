#ifndef __HEAP_H
#define __HEAP_H

#include "type.h"

#define DynMemAllSizeByte     1024*3            //自定义总动态内存区（内存池）大小（bytes）且必须为8的倍数

extern INT8 MemAllocIndex[DynMemAllSizeByte/8];

extern INT8 * MemAlloc(INT16 SizeByte);
extern void MemFree(void *MemAddr,INT16 Size);
extern INT16 MemGetUsed(void);

#endif

