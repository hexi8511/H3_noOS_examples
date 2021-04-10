
#include "timer.h"



static volatile u32 sys_tick=0;

/**
*timer0 init
*/
void timer_int_en(s32 index)
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

void timer_clear_state_bit(s32 index)
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

void timer0_stop_pause()
{
	u32 reg;
	//stop timer0
	reg=readl(TMR0_CTRL_REG);
	reg&=(~0x01);
	writel(reg,TMR0_CTRL_REG);
}

void timer1_stop_pause()
{
	u32 reg;
	//stop timer0
	reg=readl(TMR1_CTRL_REG);
	reg&=(~0x01);
	writel(reg,TMR1_CTRL_REG);
}


s32 timer0_int_proc(void* argv)
{
	timer_clear_state_bit(0);
	//printf("[%s] cur_tm_val 0x%08x\n",__func__,readl(TMR0_CUR_VALUE_REG));
	sys_tick++;
	return 0;
}

void timer0_init()
{
	u32 reg;
	

	//enable timer0 interrupt
	timer_int_en(0);
	timer_clear_state_bit(0);

	//set timer0 interval val
	reg=24000;//1ms
	writel(reg,TMR0_INTV_VALUE_REG);

	//continue mode,24M clk,load reload value into inter reg of chip
	reg=(0<<7)|(0<<4)|(1<<2)|(1<1);
	writel(reg,TMR0_CTRL_REG);
	
	//wait for load value completed
	while(readl(TMR0_CTRL_REG)&(1<<1));

	//enable timer0
	reg=readl(TMR0_CTRL_REG);
	reg|=0x01;
	writel(reg,TMR0_CTRL_REG);
	eGon2_InsINT_Func(AW_IRQ_TIMER0,(int*)timer0_int_proc,0);
	eGon2_EnableInt(AW_IRQ_TIMER0);

}





s32 timer1_int_proc(void* argv)
{
	timer_clear_state_bit(1);
	//printf("[%s] cur_tm_val 0x%08x\n",__func__,readl(TMR1_CUR_VALUE_REG));
	
	return 0;
}

void timer1_init()
{
	u32 reg;

	//enable timer0 interrupt
	timer_int_en(1);
	timer_clear_state_bit(1);

	//set timer0 interval val
	reg=240;//1ms
	writel(reg,TMR1_INTV_VALUE_REG);

	//continue mode,24M clk,load reload value into inter reg of chip
	reg=(0<<7)|(0<<4)|(1<<2)|(1<1);
	writel(reg,TMR1_CTRL_REG);
	
	//wait for load value completed
	while(readl(TMR1_CTRL_REG)&(1<<1));

	//enable timer0
	reg=readl(TMR1_CTRL_REG);
	reg|=0x01;
	writel(reg,TMR1_CTRL_REG);


	eGon2_InsINT_Func(AW_IRQ_TIMER1,(int*)timer1_int_proc,0);
	eGon2_EnableInt(AW_IRQ_TIMER1);
}



u32 sys_get_tick(void )
{
	return sys_tick;
}



static volatile u32 last_tick2=0;
static volatile u32 cur_tick2=0;

void delay_ms(u32 tms)
{
	last_tick2=sys_get_tick();
	while(1)
		{	
			cur_tick2=sys_get_tick();
			if((cur_tick2-last_tick2)>tms)
				break;
			else
				{
			//printf("tick %u,%u\n",cur_tick2,last_tick2);
				;}
				
		}
}
