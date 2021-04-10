#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"
#include "intc.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "cp15_v.h"	

void config_pll1_para(void)
{

}

/**
*copy vector into 0x00000000
*/
extern unsigned int __arm_start;
extern unsigned int __arm_end;

#define int_tbl_sz ((&__arm_end)-(&__arm_start))


void dump_vector_src(void);

void cp_int_table()
{
	memset((char*)0,0,int_tbl_sz*4);
	memcpy((char *)0x0,(char *)(&__arm_start),int_tbl_sz*4);
	printf("vector table size %d\n",int_tbl_sz*4);
	dump_vector_src();
}

void dump_vector_mm(void)
{
	//printf("0x00=%x\n",readl(0x00));
	printf("0x04=%x\n",readl(0x04));
	printf("0x08=%x\n",readl(0x08));
	printf("0x0c=%x\n",readl(0x0c));
	printf("0x10=%x\n",readl(0x10));
	printf("0x14=%x\n",readl(0x14));
	printf("0x18=%x\n",readl(0x18));
}


void dump_vector_src(void)
{
	int i=0;
	char *pchar=(char*)&__arm_start;

	printf("__arm_start:0x%08x\n",&__arm_start);
	printf("__arm_end  :0x%08x\n",&__arm_end);
	for(i=0;i<int_tbl_sz*4;i++)
		{
		if(((i%16)==0)&&(i!=0))
			{
			printf("\n");
			}
	
		printf("0x%02x ",*pchar);
		pchar++;
		}
	printf("\n");
}

extern void enable_icache();
extern void 	flush_dcache();
extern void 	enable_dcache();

int main(void)
{
	int count=0;

	asm volatile(
		"mrc p15, 0, r0, c1, c0, 1\n"
		"orr r0, r0, #1 << 6\n"
		"mcr p15, 0, r0, c1, c0, 1\n"
		::: "r0");

	enable_icache();
	flush_dcache();
	enable_dcache();
	
	//init uart 
	uart0_init(115200);

	

	//init blue led color
	led_init();


	for (int i=0;i<20;i++)
	{
	delay();
	}
	printf("===== ISR Vector Copy TEST APP FOR EZPI-A1 ======\r\n");
	printf("[%d] Hello copy vector into 0x00000000 test!\n",count);

	printf("eGon2_Int_Init\n");
	
	//init gic
	eGon2_Int_Init();

	//copy vector into the address 0x000000
	cp_int_table();
	
	set_vect_low_addr();
	
	open_sys_int();

	dump_vector_mm();

	while(1)
	{
	led_ss_flash();
	led_red_flash();
	led_blue_flash();
	printf("ISR test\n");
	count++;
	}

	return 0;
}
