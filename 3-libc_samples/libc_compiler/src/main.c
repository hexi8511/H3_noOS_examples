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

uint8_t strbuf[]="1234";
uint8_t strbuf2[]="12.34";


void config_pll1_para()
{
}

void libc_test()
{
	uint8_t * pbuf=malloc(1024*1024*178);

	if(pbuf==0)
	{
		printf("malloc memory error!\n");
		return;
	}

	printf("malloc memory ok!\n");

	for(int i=0;i<4;i++)

	memset(pbuf,0,1024*1024*32);

	printf("clear memory ok!\n");
	
	int num=atol(strbuf);
	printf("str num=%d\n",num);

	float fnum=atof(strbuf2);
	printf("str num=%f\n",fnum);
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

printf("=============== stander libc test for EZPI A1 ================\n\r");

		printf("addr buf=%p\n\r",str_buf);
		printf("0x%x\n\r",readl(PA_CFG0));
		printf("0x%x\n\r",readl(PC_CFG0));
		printf("0x%x\n\r",readl(PD_CFG0));
		printf("0x%x\n\r",readl(PE_CFG0));
		printf("0x%x\n\r",readl(PF_CFG0));
		printf("0x%x\n\r",readl(PG_CFG0));

	

		libc_test();

		while(1)
			{
			led_ss_flash();
			led_red_flash();
			printf("[%d] Hello Libc!\n\r",count);
			count++;

			}
		#endif
	
return 0;
}
