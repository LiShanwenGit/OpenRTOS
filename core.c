/**********************************************
 * 文件名：core.c
 * 编辑者：Li
 * 更新时间：2019.12.22
 * 说明：此文件为操作系统核心文件，滴答时钟初始化，进程切换，进入/退出临界区
**********************************************/
#include "core.h"
#include "OpenRTOS.h"
#include "uart.h"

/***********************
系统时钟初始化
该时钟作为系统滴答时钟
***********************/
static void SysClkInit(void)    //定时器作为滴答时钟
{
    TCCR1B|=0X03;
    TCNT1=0xFE00;
    TIMSK|=(1<<2);
}

/*****关闭中断*****/
void DisableIntrp(void)
{
    SREG&=(unsigned char)(~(1<<7));   //关总中断
}

/*****开启中断*****/
void EnableIntrp(void)
{
    SREG|=(1<<7);
}

/******上下文切换***********/
inline void TaskContexSwitch(void)
{
    ENTER_CRITICAL();       //进入临界区
    asm volatile("push  r1");        //R1入栈
    asm volatile("push	r0");        //R0入栈
    asm volatile("in	r0, 0x3f");      //SREG入栈
    asm volatile("push	r0");
    asm volatile("lds	r0, 0x5B");//RAMPZ入栈
    asm volatile("push	r0");
    asm volatile("eor	r1, r1");    //清零R1

    asm volatile("push  r2");         //R2入栈
    asm volatile("push	r3");         //R3入栈
    asm volatile("push  r4");         //R4入栈
    asm volatile("push	r5");         //R5入栈
    asm volatile("push  r6");         //R6入栈
    asm volatile("push	r7");         //R7入栈
    asm volatile("push  r8");         //R8入栈
    asm volatile("push	r9");         //R9入栈
    asm volatile("push  r10");        //R10入栈
    asm volatile("push	r11");        //R11入栈
    asm volatile("push  r12");        //R12入栈
    asm volatile("push	r13");        //R13入栈
    asm volatile("push  r14");        //R14入栈
    asm volatile("push	r15");        //R15入栈
    asm volatile("push  r16");        //R16入栈
    asm volatile("push	r17");        //R17入栈
    asm volatile("push  r18");        //R18入栈
    asm volatile("push	r19");        //R19入栈
    asm volatile("push  r20");        //R20入栈
    asm volatile("push	r21");        //R21入栈
    asm volatile("push  r22");        //R22入栈
    asm volatile("push	r23");        //R23入栈
    asm volatile("push	r24");        //R24入栈
    asm volatile("push  r25");        //R25入栈
    asm volatile("push	r26");        //R26入栈
    asm volatile("push	r27");        //R27入栈
    asm volatile("push	r30");        //R30入栈
    asm volatile("push	r31");        //R31入栈

    RunningTask->TCB_TopSP=SP;
    RunningTask=RunningTask->TCBNext;
    while(RunningTask->TCBState==255)
    {
        RunningTask=RunningTask->TCBNext;
    }
    SP=RunningTask->TCB_TopSP;
    asm volatile("pop	r31");
    asm volatile("pop	r30");
    asm volatile("pop	r27");
    asm volatile("pop	r26");
    asm volatile("pop   r25");
    asm volatile("pop	r24");
    asm volatile("pop	r23");
    asm volatile("pop   r22");
    asm volatile("pop	r21");
    asm volatile("pop   r20");
    asm volatile("pop	r19");
    asm volatile("pop   r18");
    asm volatile("pop	r17");
    asm volatile("pop   r16");
    asm volatile("pop	r15");
    asm volatile("pop   r14");
    asm volatile("pop	r13");
    asm volatile("pop   r12");
    asm volatile("pop	r11");
    asm volatile("pop   r10");
    asm volatile("pop	r9");
    asm volatile("pop   r8");
    asm volatile("pop	r7");
    asm volatile("pop   r6");
    asm volatile("pop	r5");
    asm volatile("pop   r4");
    asm volatile("pop	r3");
    asm volatile("pop   r2");
    asm volatile("pop	r0");
    asm volatile("sts	0x5B, r0");
    asm volatile("pop	r0");
    asm volatile("out	0x3f, r0");
    asm volatile("pop	r0");

    asm volatile("pop	r1");
    //  TIFR|=(1<<2);
    EXIT_CRITICAL();
    asm volatile("pop	r28");
    asm volatile("pop	r29");
    asm volatile ("RET");

}


#if(SYS_CLK_TIMER==0)
ISR(TIMER0_OVF_vect)
{
    TaskContexSwitch();
    RefreshBlkList();     //刷新阻塞任务状态
    EXIT_CRITICAL();
}
#endif
#if(SYS_CLK_TIMER==1)
ISR(TIMER1_OVF_vect)
{
    ENTER_CRITICAL();       //进入临界区
    TCNT1=0xFE00;
    TIFR|=(1<<2);
    TaskContexSwitch();
    RefreshBlkList();     //刷新阻塞任务状态
    EXIT_CRITICAL();
}
#endif
#if(SYS_CLK_TIMER==2)
ISR(TIMER2_OVF_vect)
{
    RefreshBlkList();    //刷新阻塞任务状态
    TaskContexSwitch();  //任务切换
}
#endif

void OpenRTOSInit(void)
{
    BlankRegisterAll();                 //注册所有任务，即为所有可能的任务分配空间
    RdyListIndexInit();                 //初始化任务查询表
    BlkLisInit();
    RdyListNumInit();                   //任务就绪链表初始化
    SuspListInit();                     //阻塞链表初始化
    IdleTaskInit();                   //空闲任务初始化
}

/******任务开始调度函数********/
void pStartSchedule(void)
{
    ENTER_CRITICAL();                   //进入临界区
    RunningTask=GetFirstHighTask();     //获取第一个最高优先级的任务
    SysClkInit();                       //初始化时钟
    TaskContexSwitch();                 //进行一次任务切换
    EXIT_CRITICAL();
}





