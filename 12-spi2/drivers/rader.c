#include "cache.h"
#include "gpio_reg.h"
#include "basic_reg.h"
#include "uart.h"
#include "stdio.h"
#include "string.h"
#include "led.h"
#include "cp15_v.h"
//#include "ths.h"
#include "ccu_reg.h"
#include <linux/types.h>
#include <linux/io.h>
#include "dram_sun8i_h3.h"
#include "ccu.h"
#include "asm/io.h"
#include "mmu.h"
#include "uart_reg.h"
#include "rtc.h"
#include "timer.h"
#include "intc.h"
#include "spi.h"

static int rader_start_flag=0;
static unsigned int rader_dis_cnt=0;
static unsigned int rader_dis_cnt_ok=0;
static unsigned int rader_dis_cnt_ok2=0;
static int update_dis_data=0;



static void rader_trigger_init()
{

	//gpio_A8 status
	volatile unsigned int tmp_reg;


	tmp_reg=__read_reg(PA_CFG1);
	__set_bit(0,BW3,IO_OUTPUT);
	__write_reg(PA_CFG1,tmp_reg);

	tmp_reg=__read_reg(PA_DRV0);
	__set_bit(16,BW2,DRV_LEV3);
	__write_reg(PA_DRV0,tmp_reg);


	tmp_reg=__read_reg(PA_PUL0);
	__set_bit(16,BW2,PUL_DW);
	__write_reg(PA_PUL0,tmp_reg);


}



static void rader_trigger_on()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);

	__set_bit(8,BW1,1);
	__write_reg(PA_DAT,tmp_reg);

}

static void rader_trigger_off()
{
	volatile unsigned int tmp_reg=__read_reg(PA_DAT);

	__set_bit(8,BW1,0);
	__write_reg(PA_DAT,tmp_reg);

}


/**
*GPIOA9
*/
static s32 gpio_a9_proc(void* argv)
{
	volatile unsigned int tmp_reg;
	//clear pa9_ext irq pend flag
	tmp_reg=__read_reg(PA_INT_STA);
	__set_bit(9,BW1,1);
	__write_reg(PA_INT_STA,tmp_reg);

	printf("GPIOA9\r\n");
rader_start_flag=3;

	//add you interrupt proccess function
	
	return 0;
}

static void gpio_a9_init()
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
	
	//pos_edge trigger
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



/**
*timer0 init
*/
static void timer_int_en(s32 index)
{
	u32 reg=0;

	reg=readl(TMR_IRQ_EN_REG);
	if(index==0)
		{
		writel(reg|0x01,TMR_IRQ_EN_REG);
		}
	else if(index==1)
		{
		writel(reg|0x02,TMR_IRQ_EN_REG);
		}
}

static void timer_clear_state_bit(s32 index)
{
	u32 reg=0;

	reg=readl(TMR_IRQ_STA_REG);
	if(index==0)
		{
		writel(reg|0x01,TMR_IRQ_STA_REG);
		}
	else if(index==1)
		{
		writel(reg|0x02,TMR_IRQ_STA_REG);
		}
}

static void timer0_stop_pause()
{
	u32 reg;
	//stop timer0
	reg=readl(TMR0_CTRL_REG);
	reg&=(~0x01);
	writel(reg,TMR0_CTRL_REG);
}

static void timer1_stop_pause()
{
	u32 reg;
	//stop timer0
	reg=readl(TMR1_CTRL_REG);
	reg&=(~0x01);
	writel(reg,TMR1_CTRL_REG);
}

#if 0
static void timer1_init()
{
	u32 reg;

	//enable timer0 interrupt
	timer_int_en(1);
	timer_clear_state_bit(1);

	//set timer0 interval val
	reg=24;//1us
	writel(reg,TMR1_INTV_VALUE_REG);

	//continue mode,24M clk,load reload value into inter reg of chip
	reg=(0<<7)|(0<<4)|(1<<2)|(1<0);
	writel(reg,TMR1_CTRL_REG);
	
	//wait for load value completed
	while(readl(TMR1_CTRL_REG)&(1<<1));

	//enable timer0
	reg=readl(TMR1_CTRL_REG);
	reg|=0x01;
	writel(reg,TMR1_CTRL_REG);
	
}
#endif

static s32 timer1_int_proc2(void* argv)
{
	timer_clear_state_bit(1);
//	printf("[%s] cur_tm_val 0x%08x\n",__func__,readl(TMR1_CUR_VALUE_REG));
	rader_dis_cnt++;


//rader_dis_cnt++;
	return 0;
}


static int count=0;
uint32_t rader_task_tick=0;
 void rader_task()
{
	//init PA9 ext interrupt 
	gpio_a9_init();

	printf("init Timer1\n");
	timer1_init();
	eGon2_InsINT_Func(AW_IRQ_EINTA,(int*)gpio_a9_proc,0);
	eGon2_InsINT_Func(AW_IRQ_TIMER1,(int*)timer1_int_proc2,0);
	eGon2_EnableInt(AW_IRQ_EINTA);

	eGon2_EnableInt(AW_IRQ_TIMER1);

	rader_task_tick=sys_get_tick();
	printf("rader_trigger_init\n");
	rader_trigger_init();

while(1)
	{
	//led_blue_flash();
	//watchdog0_feed();

	if((sys_get_tick()-rader_task_tick)>1000)
	{
	rader_task_tick=sys_get_tick();
	printf("start tigger\n");
//	led_blue_flash();
	rader_trigger_off();
	delay();
	
	rader_trigger_on();
	rader_start_flag=1;
	delay();
	rader_trigger_off();
	}


	if(rader_start_flag==1)
	{
		rader_dis_cnt=0;
		//get state time val;
		rader_start_flag=2;
		rader_dis_cnt_ok=rader_dis_cnt;
		//rader_start_flag=3;
	}

	 if(rader_start_flag==3)
	{
		rader_start_flag=0;
		rader_dis_cnt_ok2=rader_dis_cnt;
		rader_dis_cnt=0;
		update_dis_data=1;
	}
#if 0
if(rader_dis_cnt>1000)
{
rader_dis_cnt=0;
led_blue_flash();
}
#endif
#if 1
	if(update_dis_data==1)
	{	

		update_dis_data=0;
	
	//340*t/2=dis
		printf("dis:%d (mm)\n",(rader_dis_cnt_ok2-rader_dis_cnt_ok)*17/10);//mm

		led_blue_flash();
	}
#endif
	//read_temp();
	count++;
	}
}

