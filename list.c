/**********************************************
 * 文件名：list.c
 * 编辑者：Li
 * 更新时间：2019.12.22
 * 说明：此文件为操作系统所有链表相关文件
 * 包括就绪链表，阻塞链表，挂起链表
**********************************************/
#include  "list.h"
#include  "task.h"
#include  "stdio.h"

pTCB *RdyListIndex[PRIO_DIFF_SIZE+1];    //就绪链表索引

UINT8 RdyListNum[PRIO_DIFF_SIZE+1];      //就绪任务个数   0下标表示空闲任务 该数组记录相应优先级的任务数量

pTCB *pBlkList;                          //阻塞链表
static pTCB *pSuspList;                  //挂起链表
static pTCB *pBlankList;                 //空白链表


/****删除链表一个特定的节点后****/
void ListRemoveNode(pTCB *Remove)
{
    Remove->TCBPrev->TCBNext=Remove->TCBNext;
    Remove->TCBNext->TCBPrev=Remove->TCBPrev;
}

/*****新建一个就绪任务控制块链表，包括两个假任务控制块******/
static pTCB *NewRdyVoidList(void)
{
    pTCB *NewListHead=(pTCB *)MemAlloc(sizeof(pTCB));
    pTCB *NewListTail=(pTCB *)MemAlloc(sizeof(pTCB));
    if(NewListHead!=NULL&&NewListTail!=NULL)
    {
        NewListHead->TCBNext=NewListTail;
        NewListHead->TCBPrev=NewListTail;
        NewListTail->TCBNext=NewListHead;
        NewListTail->TCBPrev=NewListHead;
        NewListHead->TCBState=255;
        NewListTail->TCBState=255;
        NewListHead->TCBName="Head";
        NewListTail->TCBName="Tail";
        return NewListHead;
    }
    else
    {
        return NULL;
    }
}

/*****新建一个阻塞和挂起任务控制块链表，包括两个假任务控制块******/
static pTCB *NewBlkSupVoidList(void)
{
    pTCB *NewListHead=(pTCB *)MemAlloc(sizeof(pTCB));
    pTCB *NewListTail=(pTCB *)MemAlloc(sizeof(pTCB));
    if(NewListHead!=NULL&&NewListTail!=NULL)
    {
        NewListHead->TCBNext=NewListTail;
        NewListHead->TCBPrev=NULL;
        NewListTail->TCBNext=NULL;
        NewListTail->TCBPrev=NewListHead;
        NewListHead->TCBState=255;
        NewListTail->TCBState=255;
        NewListHead->TCBName="Head";
        NewListTail->TCBName="Tail";
        return NewListHead;
    }
    else
    {
        return NULL;
    }
}


/****将控制块插入到就绪链表头部后****/
void InsertRdyList(UINT8 Prio,pTCB *Insert)
{
    Insert->TCBNext=RdyListIndex[Prio]->TCBNext;
    Insert->TCBPrev=RdyListIndex[Prio];
    RdyListIndex[Prio]->TCBNext->TCBPrev=Insert;
    RdyListIndex[Prio]->TCBNext=Insert;
}

/****阻塞链表插入******/
void InsertBlkList(pTCB *Insert)
{
    Insert->TCBNext=pBlkList->TCBNext;
    Insert->TCBPrev=pBlkList;
    pBlkList->TCBNext->TCBPrev=Insert;
    pBlkList->TCBNext=Insert;
}

/****从空白链表的表头获取一个任务控制块********/
pTCB *GetBlankTCB(void)
{
    pTCB *GetReturn;
    GetReturn=pBlankList->TCBNext;        //保存需要返回的节点
    if(GetReturn!=NULL)
    {
        pBlankList->TCBNext=pBlankList->TCBNext->TCBNext;
        pBlankList->TCBNext->TCBPrev=pBlankList;
        return GetReturn;
    }
    else
    {
        return NULL;
    }
}

/****插入到空白链表中************/
void InsertBlankList(pTCB *Insert)
{
    Insert->TCBNext=pBlankList->TCBNext;
    Insert->TCBPrev=pBlankList;
    pBlankList->TCBNext->TCBPrev=Insert;
    pBlankList->TCBNext=Insert;
}

/****插入到挂起链表************/
void InsertSuspList(pTCB *Insert)
{
    Insert->TCBNext=pSuspList->TCBNext;
    Insert->TCBPrev=pSuspList;
    pSuspList->TCBNext->TCBPrev=Insert;
    pSuspList->TCBNext=Insert;
}

/***对所有的任务进行注册并存放到空白链表中*******/
UINT8 BlankRegisterAll(void)
{
    UINT16 AllTaskNum=0;
    pBlankList=NewBlkSupVoidList();
    if(pBlankList!=NULL)
    {
        for(AllTaskNum=0;AllTaskNum<PRIO_DIFF_SIZE*PRIO_SAME_SIZE;AllTaskNum++)
        {
            pTCB *NewTask=(pTCB *)MemAlloc(sizeof(pTCB));
            if(NewTask!=NULL)
            {
                InsertBlankList(NewTask);
            }
            else
            {
                return FAILURE;
            }
        }
        return SUCCESS;
    }
    else
    {
        return FAILURE;
    }
}


/***初始化就绪链表索引，将所有任务插入到索引表中*******/
void RdyListIndexInit(void)
{
    UINT8 i=0;
    for(i=PRIO_DIFF_SIZE;i>0;i--)
    {
        RdyListIndex[i]=NewRdyVoidList();
    }
}

/****初始化就绪任务表，将空闲任务设置为就绪态，其他所有空白任务设置为阻塞状态******/
void RdyListNumInit(void)
{
    UINT8 i=0;
    for(i=PRIO_DIFF_SIZE;i>0;i--)
    {
        RdyListNum[i]=0;
    }
    RdyListNum[0]=1;    //将空闲任务初始化为就绪状态
}

/****阻塞链表初始化*******/
void BlkLisInit(void)
{
    pBlkList=NewBlkSupVoidList();
}

/****挂起链表初始化********/
void SuspListInit(void)
{
    pSuspList=NewBlkSupVoidList();
}
