#include "delay.h"
#include "timer.h"

void delay()  
{  
    volatile int i = 0x10000;  
    while (i--);  
}  


void udelay(int tm)  
{  
    volatile int i = 0;  
    while (i--);  
}  

#ifdef BOOT1

#endif
