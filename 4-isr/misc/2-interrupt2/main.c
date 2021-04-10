#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"
#include "intc.h"


void config_pll1_para(void)
{

}

struct keyadc_regs
{
	u32 ctrl;
	u32 intc;
	u32 ints;
	u32 data;
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
}

extern int __arm_start;
extern int __arm_end;

#define int_tbl_sz ((&__arm_end)-(&__arm_start))

void cp_int_table()
{
	memset((void*)(&__arm_start),0,int_tbl_sz*4);
	memcpy((void *)0x0,(void *)(&__arm_start),int_tbl_sz*4);
}

int main()
{
	char str_buf[12]={0,};
	int count=0;
	//int init
	cp_int_table();

	//init blue led color
	led_init();

	//init uart 
	uart0_init(115200);
	printf("[%d] Hello Libc!\n",count);
	printf("addr buf=%p\n",str_buf);
	printf("[*] ctrl=0x%08x,intc=0x%08x,data=0x%08x\n",kreg->ctrl,kreg->intc,kreg->data);
	
	//	
	keyadc_init();
	printf("eGon2_Int_Init\n");

	eGon2_Int_Init();

	printf("eGon2_InsINT_Func\n");

	eGon2_InsINT_Func(62,(int *)key_proc,0);

	printf("eGon2_EnableInt\n");
	eGon2_EnableInt(62);
	while(1){

	led_blue_flash();
	
	printf("ctrl=0x%08x,intc=0x%08x,ints=0x%08x,data=0x%0x\n",kreg->ctrl,kreg->intc,kreg->ints,kreg->data);
	count++;
	}

	return 0;
}
