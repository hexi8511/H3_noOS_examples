#include "delay.h"

void delay()  
{  
    volatile int i = 0x10000;  
    while (i--);  
}  


void udelay(int tm)  
{  
    volatile int i = 1000;  
    while (i--);  
}  

