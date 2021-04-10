#include "gpio_reg.h"
#include "basic_reg.h"
#include "ccu_reg.h"
#include "uart.h"
#include "ccu.h"



#include "led.h"
#include "stdio.h"

unsigned int testdata[8]={33,34,35,16,17};

void config_pll1_para()
{
}

unsigned int geta()
{
return testdata[1];
}

unsigned int getb()
{
return testdata[4];
}

#define BOOT_STANDBY_RUN_ADDR	(0x00040000)
int main()
{
unsigned volatile int a=100;
unsigned volatile int b=23;
unsigned volatile int c=1;
char str_buf[12]={0,};
int count=0;

for(int i=0;i<20;i++)
delay();

//init blue led color
led_init();
a=geta();
b=getb();
c=a/b;


//init uart 
uart0_init(115200);

printf("=============== Self libc test for EZPI A1 ===========\n");
printf("%d\n",c);
printf("addr buf=%p\n",str_buf);
printf("0x%x\n",readl(PA_CFG0));
printf("0x%x\n",readl(PC_CFG0));
printf("0x%x\n",readl(PD_CFG0));
printf("0x%x\n",readl(PE_CFG0));
printf("0x%x\n",readl(PF_CFG0));
printf("0x%x\n",readl(PG_CFG0));
while(1){

led_ss_flash();
printf("[%d] Hello Libc!\n",count);
count++;
}

	
return 0;
}
