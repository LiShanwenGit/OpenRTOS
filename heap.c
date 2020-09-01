#include "heap.h"

/***********************************************************************************************
                               动态内存分配管理
主要有3个函数：请求内存分配函数，释放内存函数，获取内存使用字节函数
其中获取内存使用字节函数主要是为了获取内存使用率，如果内存已满，则此时千万不能再强行分配
空间，否则可能导致程序崩溃，所以在进行内存申请前，应该先获取内存使用情况，再进行内存空间
的申请。
************************************************************************************************/
UINT8 DynamicMem[DynMemAllSizeByte]={0};  //分配4K大小内存作为所有的动态内存
INT8 MemAllocIndex[DynMemAllSizeByte/8]={0}; //动态内存索引（位图）作为控制块  0：未使用  1：已使用


/***############################################################################
功能：申请内存分配
参数：申请大字节大小，最大为DynMemAllSizeByte
返回：申请内存的地址，且地址类型为CHAR8U * 型
############################################################################***/
INT8 * MemAlloc(INT16 SizeByte)
{
   INT16 i=0,j=0,IsUseFulNum=0;
   while(i<(DynMemAllSizeByte-SizeByte))
   {
     if((MemAllocIndex[i/8]&(INT8)(1<<(7-(i%8))))==0)
     {
        IsUseFulNum=0;
        for(j=i;j<i+SizeByte;j++)
        {
            if((MemAllocIndex[j/8]&(INT8)(1<<(7-(j%8))))==0)
               IsUseFulNum++;
            else
            {
                i=i+j+1;
                break;
            }
        }
        if(IsUseFulNum==SizeByte)
        {
          for(j=i;j<i+SizeByte;j++)
          {
            MemAllocIndex[j/8]|=(INT8)(1<<(7-(j%8)));
          }
          return  (INT8 *)DynamicMem+i;
        }
     }
     else
       i++;
   }

   return 0;
}


/*****############################################################################
功能：释放之前分配的内存空间
参数：需要释放内存的指针void *，    之前申请大字节大小Size
返回：无
############################################################################*****/

void MemFree(void *MemAddr,INT16 Size)
{
    #define MemAddrOffset  (unsigned int)((unsigned char*)(MemAddr)-DynamicMem)
    INT16 addr=0;
    for(addr=MemAddrOffset;addr<Size+MemAddrOffset;addr++)
    {
      MemAllocIndex[addr/8]&=(INT8)(~(1<<(7-(addr%8))));
    }
}

/*****###########################################################################
功能：获取内存使用情况
参数：无
返回：已经使用了的字节数
##########################################################################******/
INT16 MemGetUsed(void)
{
   INT16 i=0,MemUsed=0;
   for(i=0;i<DynMemAllSizeByte;i++)
   {
      if((MemAllocIndex[i/8]&(INT8)(1<<(7-i%8)))!=0)
        MemUsed++;
   }
   return MemUsed;
}
