#ifndef  __uart_h
#define  __uart_h

#include "OpenRTOS.h"

void uart1_init(void);
void puts1(char *s);


 void puts1_PrintNumber(unsigned long number);
#endif // __uart_h
