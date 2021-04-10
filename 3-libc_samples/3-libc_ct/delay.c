#include "delay.h"

void delay()  
{  
    volatile int i = 0x10000;  
    while (i--);  
}  

