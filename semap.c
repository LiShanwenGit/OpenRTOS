#include "semap.h"

/*****定义信号量空间******/
#if(SEMAPHORE_ENABLE==1)

pSemap *Semaphore[SEMAPHORE_SIZE];    //定义信号量指针数组

/****初始化信号量初值****/
void  SemaphoreInit(pSemap *Smp,UINT8 Value)
{
    Smp->Value=Value;
}

/*****获取一个信号量*******/
UINT8 TakeSignal(pSemap *Smp)
{
    if(Smp->Value<0)
    {
        return 0;
    }
    else
    {
        Smp->Value--;
        return 1;
    }
}

/*****释放一个信号量******/
UINT8 ReleaseSignal(pSemap *Smp)
{
    if(Smp->Value>254)
    {
        return 0;
    }
    else
    {
        Smp->Value++;
        return 1;
    }
}

/******清空信号量为0*****/
void ClearSignal(pSemap *Smp)
{
    Smp->Value=0;
}

#endif

