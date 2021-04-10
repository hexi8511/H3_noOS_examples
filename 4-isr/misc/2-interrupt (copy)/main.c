#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"
#include "intc.h"
#include "stdio.h"
#include "string.h"
#include "led.h"

void config_pll1_para(void)
{

}

struct keyadc_regs
{
	u32 ctrl;
	u32 intc;
	u32 ints;
	u32 data1;
	u32 data2;
};

struct keyadc_regs *kreg=(struct keyadc_regs*)0x01c21800;

void keyadc_init()
{
	kreg->ctrl=0xffffffff;
	kreg->intc=0xffffffff;
}

s32 key_proc(void* argv)
{
	printf("KEY INT\n");
	return 0;
}

extern unsigned int __arm_start;
extern unsigned int __arm_end;

#define int_tbl_sz ((&__arm_end)-(&__arm_start))

void dump_vector_src()
{
	int i=0;
	char *pchar=(char*)&__arm_start;

	printf("__arm_start:0x%08x\n",&__arm_start);
	printf("__arm_end  :0x%08x\n",&__arm_end);
	for(i=0;i<int_tbl_sz*4;i++)
		{
		if(((i+1)%32)==0)
			{
			printf("\n");
			}
	
		printf("0x%x ",*pchar);
		pchar++;
		}
	printf("\n");
}

void cp_int_table()
{
	memset((char*)0,0,int_tbl_sz*4);
	memcpy((char *)0x0,(char *)(&__arm_start),int_tbl_sz*4);
	printf("vector table size %d\n",int_tbl_sz*4);
	dump_vector_src();
}

void dump_vector_mm()
{
	//printf("0x00=%x\n",readl(0x00));
	printf("0x04=%x\n",readl(0x04));
	printf("0x08=%x\n",readl(0x08));
	printf("0x0c=%x\n",readl(0x0c));
	printf("0x10=%x\n",readl(0x10));
	printf("0x14=%x\n",readl(0x14));
	printf("0x18=%x\n",readl(0x18));
}

int main()
{
	char str_buf[12]={0,};
	int count=0;
	
	//init uart 
	uart0_init(115200);

	//init blue led color
	led_init();
	printf("[%d] Hello Libc!\n",count);
	printf("addr buf=%p\n",str_buf);
	printf("[*] ctrl=0x%08x,intc=0x%08x,data1=0x%08x,data2=0x%08x\n",kreg->ctrl,kreg->intc,kreg->data1,kreg->data2);


	keyadc_init();
	printf("eGon2_Int_Init\n");

	eGon2_Int_Init();

	//int init
	cp_int_table();
	set_vect_low_addr();
	open_sys_int();

	dump_vector_mm();

	printf("eGon2_InsINT_Func\n");

	eGon2_InsINT_Func(62,(int *)key_proc,0);

	printf("eGon2_EnableInt\n");
	eGon2_EnableInt(62);

	while(1)
	{
	led_blue_flash();
	
	printf("ctrl=0x%08x,intc=0x%08x,ints=0x%08x,data1=0x%08x,data2=0x%08x\n",kreg->ctrl,kreg->intc,kreg->data1,kreg->data2);
	count++;
	}

	return 0;
}
