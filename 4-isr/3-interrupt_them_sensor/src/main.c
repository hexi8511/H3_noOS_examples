#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"
#include "intc.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "cp15_v.h"
#include "ths.h"
#include "ccu_reg.h"

void config_pll1_para(void)
{

}
/**
*key ADC 
*/
struct keyadc_regs
{
	u32 ctrl;
	u32 intc;
	u32 ints;
	u32 data1;

};

struct keyadc_regs *kreg=(struct keyadc_regs*)0x01c21800;

void keyadc_init()
{
	kreg->ctrl=0;
	kreg->ctrl=(0x10<<24)|(1<<0)|(3<<6);
	//kreg->intc=0x00000000;//disable all interrupt
	kreg->intc=0x0000001f;//enable all interrupt
	kreg->ints=0x0000001f;
}

s32 key_proc(void* argv)
{
	kreg->ints=0x0000001f;
	printf("KEY INT\nvalue=0x%08x\n",kreg->data1);
	return 0;
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

/**
*THS init
*/

void ths_init()
{
	//open ths clk gate 
	unsigned int reg;

	reg=readl(BUS_CLK_GATING_REG2);
	reg|=(1<<8);
	writel(reg,BUS_CLK_GATING_REG2);


	//reset ths mode
	reg=readl(BUS_SOFT_RST_REG3);
	reg&=(~(1<<8));
	writel(reg,BUS_SOFT_RST_REG3);
	reg|=(1<<8);
	writel(reg,BUS_SOFT_RST_REG3);

	//THS clk register config
	reg=readl(THS_CLK_REG);
	reg|=(1<<31);//24Mhz open ths clk
	writel(reg,THS_CLK_REG);

	//THS device mode regs config 
	reg=0x3f;//0xff;
	writel(reg,THS_CTRL0);//acq0 clk_in/0x100=0.09375Mhz=10.67us

	reg=readl(THS_CTRL2);//acq1 clk_in/5=4.8Mhz=0.208us, conv_tm=0.508us
	reg|=0x01|(0x3f<<16);
	writel(reg,THS_CTRL2);

	//open interrupt
	reg=(0x1FFF<<12)|(1<<8);//                            |(1<<4)|(1<<0);//enable measure int/shutdown int/alarm int
										//4096*0x10000/24M=11.184s
	writel(reg,THS_INT_CTRL);

	//clean status bits
	reg=(1<<12)|(1<<8)|(1<<4)|(1<<0);//enable measure int/shutdown int/alarm int
	writel(reg,THS_STAT);

	//set alarm threshold
	reg=(0x5a0<<16)|(0x684);//enable measure int/shutdown int/alarm int
	writel(reg,THS_ALARM_CTRL);

	//set shutdown threshold
	reg=(0x4e9<<16);//enable measure int/shutdown int/alarm int
	writel(reg,THS_SHUTDOWN_CTRL);

	//enable avg filter 
	reg=(1<<2)|(2);//enable measure int/shutdown int/alarm int 8avg
	writel(reg,THS_FILTER);

}

u32 temp_update_flag=0;
u32 temp_reg=0;
u32 state_reg;

void read_temp()
{
	eGon2_EnableInt(63);
	
	if(temp_update_flag)
	{
		printf("****** Read temp state 0x%08x ******\n",state_reg);

		if(state_reg&(1<<0))
			{
			printf("Alarm interrupt pending\n");
			}

		if(state_reg&(1<<4))
			{
			printf("Shutdown interrupt pending\n");
			}

		if(state_reg&(1<<8))
			{
			printf("Measure data finished interrupt pending\n");
			}

		if(state_reg&(1<<12))
			{
			printf("Alarm off interrupt pending\n");
			}


		int temp=217-temp_reg*1000/8253;//get this from linux kernel codes
		printf("temp:0x%08x\n",temp_reg);
		printf("temp:%d\n",temp);
		temp_update_flag=0;
	}
}

s32 ths_int_proc(void* argv)
{
	eGon2_DisableInt(63);

	printf("****** Thermal isr ******\n");

	state_reg=readl(THS_STAT);
	state_reg=(1<<12)|(1<<8)|(1<<4)|(1<<0);//enable measure int/shutdown int/alarm int
	writel(state_reg,THS_STAT);
	temp_reg=readl(THS_DATA);
	
	temp_update_flag=1;
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
	int count=0;
	
	//init uart 
	uart0_init(115200);

	for (int i=0;i<20;i++)
	{
	delay();
	}

	printf("===== ISR Read ARM Tempture TEST APP FOR EZPI-A1 ======\r\n");

	//init PA9 ext interrupt 
	gpio_a9_init();

	//init blue led color
	led_init();
	printf("[%d] Hello thermal sensor interrupt test!\n",count);
	
	keyadc_init();
	printf("eGon2_Int_Init\n");

	printf("init THS\n");
	ths_init();

	eGon2_Int_Init();

	//int init
	cp_int_table();
	set_vect_low_addr();
	open_sys_int();

	//dump_vector_mm();

	printf("eGon2_InsINT_Func\n");

	//register ths interrupt process function
	eGon2_InsINT_Func(AW_IRQ_LRADC,(int *)key_proc,0);
	eGon2_InsINT_Func(AW_IRQ_EINTA,(int*)gpio_a9_proc,0);
	eGon2_InsINT_Func(63,(int*)ths_int_proc,0);

	printf("eGon2_EnableInt\n");

	//enable interrupt
	eGon2_EnableInt(AW_IRQ_LRADC);
	eGon2_EnableInt(AW_IRQ_EINTA);
	
	while(1)
	{
	led_red_flash();
	read_temp();
	count++;
	}

	return 0;
}
