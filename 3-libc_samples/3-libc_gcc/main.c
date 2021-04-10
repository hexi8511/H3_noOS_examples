#include "gpio_reg.h"
#include "basic_reg.h"
#include "ccu_reg.h"
#include "uart.h"
#include "ccu.h"



#include "led.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

void config_pll1_para()
{
}



#define BOOT_STANDBY_RUN_ADDR	(0x00040000)
int main()
{
char str_buf[1024]={0,};
int count=0;

//int a=abs(count);
#if 1
//init blue led color
led_init();



//init uart 
uart0_init(115200);

printf("addr buf=%p\n\r",str_buf);
printf("0x%x\n\r",readl(PA_CFG0));
printf("0x%x\n\r",readl(PC_CFG0));
printf("0x%x\n\r",readl(PD_CFG0));
printf("0x%x\n\r",readl(PE_CFG0));
printf("0x%x\n\r",readl(PF_CFG0));
printf("0x%x\n\r",readl(PG_CFG0));
while(1){

led_red_flash();
printf("[%d] Hello Libc!\n\r",count);
count++;

}
#endif
	
return 0;
}
