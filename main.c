#include "OpenRTOS.h"
#include "uart.h"


void task1(void *p)
{
  while(1)
  {
    ENTER_CRITICAL();     //进入临界区
    puts1("task1\r\n");
    EXIT_CRITICAL();      //退出临界区
  }
}

void task2(void *p)       //任务函数2
{
  while(1)
  {
    ENTER_CRITICAL();     //进入临界区
    puts1("task2\r\n");
    EXIT_CRITICAL();      //退出临界区
  }
}

void task3(void *p)       //任务函数2
{
  while(1)
  {
    ENTER_CRITICAL();     //进入临界区
    puts1("task3\r\n");
    EXIT_CRITICAL();      //退出临界区
  }
}

char re=10,ra=10;
unsigned char STACK1[100];
unsigned char STACK2[100];
unsigned char STACK3[100];

//unsigned int task1

int main(void)
{
    uart1_init();
    OpenRTOSInit();

    DDRC=0XFF;
    PORTC|=0xff;

    re=pCreateTaskStc("task1",task1,&STACK1[99],1,NULL);
    ra=pCreateTaskStc("task2",task2,&STACK2[99],1,NULL);
    re=pCreateTaskStc("task3",task3,&STACK3[99],1,NULL);

    pStartSchedule();
    while(re&&ra)
    {
        puts1("5565655\r\n");
    }
    return 0;
}
