/**********************************************
 * 文件名：task.c
 * 编辑者：Li
 * 更新时间：2019.12.22
 * 说明：此文件为操作系统进程操作API
**********************************************/
#include "task.h"
#include "list.h"

//当前正在运行的任务指针
pTCB *SwitchRun;              //该指针为切换前的任务记录,当切换任务所在队列中还存在任务是起作用
pTCB *RunningTask;            //此处只是指向正在运行的任务
//空闲任务句柄
TaskHandler IdleTaskHandle;

/*****空闲任务回调函数*******/
#if(IDLE_CALL_ENABLE==1)
void IdleTaskApp(void *Idle)
{

}
#endif

/******空闲任务初始化***********************/
void IdleTaskInit(void)
{
    ENTER_CRITICAL();               //进入临界区
    UINT8 *Stack=(UINT8 *)MemAlloc(IDLE_STACK_SIZE); //分配内存空间给人工堆栈
    pTCB *TaskTcb=GetBlankTCB();    //从空白链表中取出一个任务控制块内存
    TaskTcb->TCBName="IdleTask";    //任务名初始化
    TaskTcb->TCBPrio=0;             //任务优先级初始化
    TaskTcb->TCBState=1;            //任务状态初始化 1表示就绪状态
    #if(IDLE_CALL_ENABLE==1)
        TaskTcb->TCBTask=IdleTaskApp;   //任务函数指针初始化
    #endif
    IdleTaskHandle=(TaskHandler)TaskTcb;         //任务句柄初始化  也叫ID号
    TaskTcb->TCBStack=Stack;        //任务人工堆栈初始化
    TaskTcb->TCBDlyCount=0;         //任务延时初始化 0
    RdyListIndex[0]=(pTCB *)IdleTaskHandle;   //将最低优先级任务指向空闲任务
    InsertRdyList(0,TaskTcb);       //将新建的任务插入到任务就绪链表中
    /******下面为任务人工堆栈初始化********/
    EXIT_CRITICAL();                //推出临界区
}


/*******判断任务所在的队列中是否为空********/
static UINT8 TaskListIsEmpty(pTCB *Handler)
{
    if(RdyListNum[Handler->TCBPrio])
    {
        return FAILURE;             //表示此时运行的任务所在队列不为空
    }
    else
    {
        return SUCCESS;             //表示此时运行任务所在的队列已为空
    }
}


/*******获取任务所在的队列的优先级********/
static UINT8 GetTaskListPrio(pTCB *Task)
{
    return Task->TCBPrio;
}


/*****获取第一个需要运行的任务********/
pTCB *GetFirstHighTask(void)
{
    UINT8 i=0;
    for(i=PRIO_DIFF_SIZE;i>0;i--)      //对所有任务进行遍历，查找就绪表，确定任务是否处于就绪态
    {
        if(RdyListNum[i])              //如果不为零，则返回当前的优先级
            return RdyListIndex[i];
    }
    return RdyListIndex[0];
}


/****获取下一个就绪任务********/
static pTCB *GetNextRdyHighTask(void)
{
    UINT8 i=0;
    if(TaskListIsEmpty(RunningTask))     //如果当前任务就绪链表已空
    {
        for(i=PRIO_DIFF_SIZE;i>0;i--)    //对所有任务进行遍历，查找就绪表，确定任务是否处于就绪态
        {
            if(RdyListNum[i])            //如果不为零，则返回当前的优先级
                return RdyListIndex[i];
        }
    }
    else
    {
        return RunningTask=SwitchRun;
    }
    return RdyListIndex[0];              //返回最低优先级，即空闲任务的优先级
}


/*********下面为静态创建任务***********************/
UINT8 pCreateTaskStc(INT8 *Name,FUN Fun,UINT8 *Stack,UINT8 Prio,TaskHandler *Handler)
{
    ENTER_CRITICAL();                   //进入临界区
    pTCB *TaskTcb=GetBlankTCB();        //从空白链表中取出一个任务控制块内存

    if(TaskTcb!=NULL)
    {
        TaskTcb->TCBName=Name;          //任务名初始化
        TaskTcb->TCBPrio=Prio;          //任务优先级初始化
        TaskTcb->TCBState=1;            //任务状态初始化 1表示就绪状态
        TaskTcb->TCBTask=Fun;           //任务函数指针初始化

        if(Handler!=NULL)
            *Handler=(TaskHandler)TaskTcb;  //任务句柄初始化  实质上是任务控制块的首地址
        TaskTcb->TCBStack=Stack;        //任务人工堆栈初始化
        TaskTcb->TCBDlyCount=0;         //任务延时初始化 0
        InsertRdyList(Prio,TaskTcb);    //将新建的任务插入到任务就绪链表中
        RdyListNum[Prio]++;             //任务就绪状态个数加1
        /******下面为任务人工堆栈初始化********/
        *(Stack)=(unsigned int)Fun;
        *(Stack-1)=((unsigned int)Fun)>>8;
        Stack-=36;
        TaskTcb->TCB_TopSP=(unsigned int)Stack;
        EXIT_CRITICAL();                //退出临界区
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}

/********下面动态创建任务******************************/
UINT8 pCreateTaskDyn(INT8 *Name,FUN Fun,UINT16 StackSize,UINT8 Prio,TaskHandler *Handler)
{
    ENTER_CRITICAL();                       //进入临界区
    UINT8 *Stack=(UINT8 *)MemAlloc(StackSize);//分配内存空间给人工堆栈
    pTCB *TaskTcb=GetBlankTCB();            //从空白链表中取出一个任务控制块内存
    if(Stack!=NULL&&TaskTcb!=NULL)
    {
        TaskTcb->TCBName=Name;              //任务名初始化
        TaskTcb->TCBPrio=Prio;              //任务优先级初始化
        TaskTcb->TCBState=1;                //任务状态初始化 1表示就绪状态
        TaskTcb->TCBTask=Fun;               //任务函数指针初始化
         if(Handler!=NULL)
            *Handler=(TaskHandler)TaskTcb;  //任务句柄初始化  实质上是任务控制块的首地址
        TaskTcb->TCBStack=Stack;            //任务人工堆栈初始化
        TaskTcb->TCBDlyCount=0;             //任务延时初始化 0

        InsertRdyList(Prio,TaskTcb);        //将新建的任务插入到任务就绪链表中
        RdyListNum[Prio]++;                 //任务就绪状态个数加1
        /******下面为任务人工堆栈初始化********/
        *(Stack--)=&Fun;
        *(Stack--)=((unsigned int)&Fun)>>8;
        Stack-=35;
        TaskTcb->TCB_TopSP=&Stack;
        EXIT_CRITICAL();                    //退出临界区
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}


/*****删除任务********/
void pDeleteTask(TaskHandler Handler)
{
    pTCB* p=(pTCB*)(Handler);
    ENTER_CRITICAL();                     //进入临界区
    RdyListNum[GetTaskListPrio(p)]--;     //相应的就绪表减一
    if(TaskListIsEmpty(p))                //如果当前删除的任务所在的队列是空的，也就是删除的任务已经是当前优先级里面的最后一个任务了
    {
        RunningTask=GetNextRdyHighTask(); //获取下一个就绪任务中最高优先级的任务
        TaskContexSwitch();               //强制进行任务切换
    }
    else if(Handler!=RunningTask)
    {
        TaskContexSwitch();
        ListRemoveNode(p);                //删除此任务
        InsertBlankList(p);               //回收删除的任务空间到空白链表
        if(p==RunningTask)
            TaskContexSwitch();           //强制进行任务切换
    }
    else
    {
        TaskContexSwitch();               //强制进行任务切换
        ListRemoveNode(p);                //在就绪链表删除此任务
        InsertSuspList(p);                //将挂起的任务挂载到挂起链表中
    }
    EXIT_CRITICAL();                      //退出临界区
}


/******任务挂起函数********/
void pSuspendTask(TaskHandler Handler)
{
    pTCB* p=(pTCB*)(Handler);
    ENTER_CRITICAL();                     //进入临界区
    RdyListNum[GetTaskListPrio(p)]--;     //相应优先级的就绪表减一
    p->TCBState=0;                        //变为阻塞状态
    if(TaskListIsEmpty(p))                //如果当前挂起的任务所在的队列是空的，也就是挂起的任务已经是当前优先级里面的最后一个任务了
    {
        RunningTask=GetNextRdyHighTask(); //获取下一个就绪任务中最高优先级的任务
        TaskContexSwitch();               //强制进行任务切换
    }
    else if(Handler!=RunningTask)         //如果此时挂起的任务不是正在运行的任务
    {
        ListRemoveNode(p);                //在就绪链表删除此任务
        InsertSuspList(p);                //将挂起的任务挂载到挂起链表中
        if(p==RunningTask)
            TaskContexSwitch();           //强制进行任务切换
    }
    else                                  //如果此时挂起的任务是正在运行的任务
    {
        TaskContexSwitch();               //强制进行任务切换
        ListRemoveNode(p);                //在就绪链表删除此任务
        InsertSuspList(p);                //将挂起的任务挂载到挂起链表中
    }
    EXIT_CRITICAL();                      //退出临界区
}


/******刷新阻塞链表*********/
inline void RefreshBlkList(void)
{
    UINT8 i;
    pTCB *p=pBlkList->TCBNext;
    ENTER_CRITICAL();
    while(p->TCBState!=255)
    {
        if(p->TCBDlyCount>0)
        {
            p->TCBDlyCount--;
            p=p->TCBNext;
        }
        else
        {
            ListRemoveNode(p);                     //从阻塞链表删除
            p->TCBState=1;
            InsertRdyList(GetTaskListPrio(p),p);   //插入到就绪链表中
            RdyListNum[GetTaskListPrio(p)]++;      //就绪队列中相应优先级的任务数加1
            for(i=PRIO_DIFF_SIZE;i>=0;i--)         //对所有任务进行遍历，查找就绪表，确定任务是否处于就绪态
            {
                if(RdyListNum[i])                  //如果不为零，则返回当前的优先级
                {
                    RunningTask=RdyListIndex[i]->TCBNext;
                    //puts1(RunningTask->TCBName);
                    //puts1("56888");
                    break;
                }
            }
            p=pBlkList->TCBNext;
        }
    }
    EXIT_CRITICAL();
}

/******系统延时函数********/
void pDelayTask(UINT16 Time)
{
    ENTER_CRITICAL();                           //进入临界区
    SwitchRun=RunningTask;
    RdyListNum[GetTaskListPrio(RunningTask)]--; //相应优先级的就绪表减一
    RunningTask->TCBState=0;                    //变为阻塞状态
    RunningTask->TCBDlyCount=Time;              //保存延时时间
    ListRemoveNode(RunningTask);                //在就绪链表删除此任务
    InsertBlkList(RunningTask);                 //将延时的任务挂载到延时链表中
    RunningTask=GetNextRdyHighTask();           //获取下一个优先级最高任务
    TaskContexSwitch();                         //强制进行任务切换
    EXIT_CRITICAL();
}

#if(SYS_LONG_DELAY==1)
/******系统长延时函数********/
void pLongDelayTask(UINT32 Time)
{
    SwitchRun=RunningTask->TCBNext;
    ENTER_CRITICAL();                           //进入临界区
    RdyList[GetTaskListPrio(RunningTask)]--;    //相应优先级的就绪表减一
    RunningTask->TCBState=0;                    //变为阻塞状态
    RunningTask->TCBLongDlyCount=Time;          //保存延时时间
    ListRemoveNode(RunningTask);                //在就绪链表删除此任务
    ListInsertHead(pBlkList,RunningTask);       //将延时的任务挂载到延时链表中
    RunningTask=GetNextRdyHighTask();           //获取下一个优先级最高任务
    TaskContexSwitch();                         //强制进行任务切换
}
#endif





