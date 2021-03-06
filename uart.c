#include "uart.h"

/*串口通信（USART1）
***************************************************************************
串口1测试程序
目标器件：ATmega128
晶振:RC8MHZ
选用的波特率：9600（也可以另外设定),改了波特率后需要将电源拔了再插上方可使用
*********************************包含头文件********************************/

/***********************************宏定义**********************************/
#define fosc 8000000  //晶振8MHZ
#define baud 9600     //波特率

/****************************************************************************
函数功能:uart1初始化程序
入口参数:
出口参数:
****************************************************************************/
void uart1_init(void) //USART1初始化
{
 UCSR1B = 0x00;   //关闭USART1
 UCSR1A = 0x00;   //不适使用倍速发送
 UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);//数据位为八位
 UBRR1L=(fosc/16/(baud+1))%256;//异步正常模式下，UBRR的计算公式
 UBRR1H=(fosc/16/(baud+1))/256;
 UCSR1B =(1<<RXEN1)|(1<<TXEN1); //接收使能，传送使能
}

/****************************************************************************
函数功能:uart1发送单字节数据
入口参数:c
出口参数:
****************************************************************************/
void putchar1(unsigned char c)//串口1发送字符
 {
     while (!(UCSR1A&(1<<UDRE1)));//表明发送器一准备就绪
  UDR1=c;
 }

/****************************************************************************
函数功能:uart1接收单字节数据
入口参数:
出口参数:
****************************************************************************/
unsigned char getchar1(void) //串口1接回收数据
   {
  while(!(UCSR1A& (1<<RXC1)));
     return UDR1;   //将接收到的字符返回
 }

/****************************************************************************
函数功能:uart1发送字符串数据
入口参数:*s
出口参数:
****************************************************************************/
void puts1(char *s)
 {
 while (*s)
  {
  putchar1(*s);
  s++;
  }
        putchar1(0x0a);//回车换行
     putchar1(0x0d);
 }


 void puts1_PrintNumber(unsigned long number)
{
      unsigned char usart_save[32]={0};
        unsigned char *usart_p;
        unsigned char i=1;

        unsigned int num=0;
        num=number;

        while(num/10)
        {
            num/=10;
            i++;
        }
        usart_p=usart_save;
        for(;i>0;i--)
        {
            usart_save[i-1]=number%10+48;
            number/=10;
        }
        puts1(usart_save);

}

