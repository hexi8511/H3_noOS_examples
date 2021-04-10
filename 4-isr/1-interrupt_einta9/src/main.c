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


/**
*GPIOA9
*/
s32 gpio_a9_proc(void* argv)
{
	volatile unsigned int tmp_reg;
	//clear pa9_ext irq pend flag
	tmp_reg=__read_reg(PA_INT_STA);
	__set_bit(9,BW1,1);
	__write_reg(PA_INT_STA,tmp_reg);

	printf("GPIOA9 INT\n");

	//add you interrupt proccess function
	
	return 0;
}

void gpio_a9_init()
{
//gpio_A9 status
	volatile unsigned int tmp_reg;

	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(4,BW3,IO_EXT_INT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(20,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);

	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(20,BW2,PUL_UP);
	__write_reg(PA_PUL0,tmp_reg);
	
	//neg_edge trigger
	tmp_reg=__read_reg(PA_INT_CFG1);
	__set_bit(4,BW4,NEG_EDGE);
	__write_reg(PA_INT_CFG1,tmp_reg);
	
	//enable pa9_ext
	tmp_reg=__read_reg(PA_INT_CTL);
	__set_bit(9,BW1,1);
	__write_reg(PA_INT_CTL,tmp_reg);

	//ext interrupt clk select OSC24
	tmp_reg=__read_reg(PG_INT_DEB);
	__set_bit(0,BW1,1);
	__write_reg(PG_INT_DEB,tmp_reg);
	
}

void dump_ext_pa_regs()
{
	unsigned int reg;

	reg=readl(PA_INT_STA);
	printf("PA_INT_STA=0x%08x\n",reg);
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

	for (int i=0;i<20;i++)
	{
	delay();
	}
	printf("===== ISR Vector Copy TEST APP FOR EZPI-A1 ======\r\n");
	
	printf("[%d] Hello interrupt test for EINTA9!\n",count);
	
	//init PA9 ext interrupt 
	gpio_a9_init();

	//init blue led color
	led_init();

	
	printf("init gic\n");
	//init GIC
	eGon2_Int_Init();

	//copy vector into 0x00000000
	cp_int_table();

	//set vector table address to 0x00000000
	set_vect_low_addr();

	//open system irq
	open_sys_int();

	dump_vector_mm();

	printf("register pa9 int process function\n");
	eGon2_InsINT_Func(AW_IRQ_EINTA,(int*)gpio_a9_proc,0);

	printf("enable pa9 interrupt\n");

	eGon2_EnableInt(AW_IRQ_EINTA);

	while(1)
	{
	led_red_flash();
	count++;
	}

	return 0;
}
