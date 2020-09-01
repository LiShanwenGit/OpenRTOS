#ifndef __OPENRTOSCONFIG_H
#define __OPENRTOSCONFIG_H

/*******任务个数*************/
#define PRIO_DIFF_SIZE      4  //定义不同优先级个数  0作为最低优先级供空闲任务使用
#define PRIO_SAME_SIZE      2   //定义相同优先级个数

/*******空闲任务************/
#define IDLE_STACK_SIZE     64  //定义空闲任务的栈空间
#define IDLE_CALL_ENABLE    0   //默认空闲回调函数不使能


/*******信号量**************/
#define SEMAPHORE_ENABLE    0   //默认不使能信号量
#define SEMAPHORE_SIZE      8   //默认支持八个信号量

/*******延时函数***********/
#define SYS_LONG_DELAY      0   //是否开启长延时函数


/*******系统时钟配置*******/
#define SYS_CLK_TIMER       1   //系统时钟为定时器0
#define SYS_CLK_FREQUENCE   100 //系统定时器定时周期为100ms


#endif

