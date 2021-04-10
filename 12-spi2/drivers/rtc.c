#include "rtc.h"
#include "asm/io.h"
#include "stdio.h"
#include "intc.h"

/*
* 0 Inner losc=16/32/N N=inlosc prescale
* 1 External OSC
*/
void rtc_losc_src(u32 sel)
{
	//unlock bit 0 
	u32 reg=readl(LOSC_CTRL_REG);
	reg|=(0x16aa<<16);
	writel(reg,LOSC_CTRL_REG);

	if(sel==0)
		{
			reg&=(~1);
			writel(reg,LOSC_CTRL_REG);
		}
	else
	{
		reg|=(1);
		writel(reg,LOSC_CTRL_REG);
	}

	//lock 
	reg&=(0x0000ffff);
	writel(reg,LOSC_CTRL_REG);
}

void set_alarm0(u32 sec )
{

	writel(sec,ALARM0_COUNTER_REG);
	
}

void enable_alarm0( )
{
	//printf("%s\n",__func__);

	
	writel(0x01,ALARM0_ENABLE_REG);	

	//clear irq pend bit	
	writel(1,ALARM0_IRQ_STA_REG);
	writel(0x01,ALARM0_IRQ_EN_REG);
	
}

void disable_alarm0()
{
	writel(0x01,ALARM0_ENABLE_REG);
	writel(0x01,ALARM0_IRQ_EN_REG);
}

s32 alarm0_isr_handler(void* argv)
{
	u32 val= readl(ALARM0_IRQ_STA_REG);

	//clean irq pend bit
	writel(1,ALARM0_IRQ_STA_REG);
	if(val&0x01)
		{
		printf("\n%s\n",__func__);
		disable_alarm0();
		}
	return 0;
}



void set_alarm1(u8 hh, u8 mm, u8 ss )
{
	//unlock
	u32 reg=readl(LOSC_CTRL_REG);
	reg&=~(1<<9);
	writel(reg,LOSC_CTRL_REG);
	
	reg=0;
	reg=(hh<<16)|(mm<<8)|(ss);
	writel(reg,ALARM1_WK_HH_MM_SS);


	//unlock
	reg=readl(LOSC_CTRL_REG);
	reg|=(1<<9);
	writel(reg,LOSC_CTRL_REG);
}


void enable_alarm1_weekday(u8 weekday)
{
	u32 reg=readl(ALARM1_ENABLE_REG);
	reg|=(1<<weekday);
	writel(reg,ALARM1_ENABLE_REG);

}

void disable_alarm1_weekday(u8 weekday)
{
u32 reg=readl(ALARM1_ENABLE_REG);
	reg&=~(1<<weekday);
	writel(reg,ALARM1_ENABLE_REG);
}

void enable_alarm1_irq()
{
	writel(1,ALARM1_IRQ_EN_REG);
}


void disable_alarm1_irq()
{
	writel(0,ALARM1_IRQ_EN_REG);
}


void enable_alarm_wakeup()
{
	writel(1,ALARM_CONFIG_REG);
}

void disable_alarm_wakeup()
{
	writel(0,ALARM_CONFIG_REG);
}

s32 alarm1_isr_handler(void* argv)
{
	u32 reg= readl(ALARM1_IRQ_STA_REG);
	writel(1,ALARM1_IRQ_STA_REG);
	if(reg==0x01)
		{
		printf("\n%s\n",__func__);
		}
return 0;
}


void set_ddhhmmss(u8 week_day , u8 hh, u8 mm, u8 ss)
{
	//unlock
	u32 reg=readl(LOSC_CTRL_REG);
	reg&=~(1<<8);
	writel(reg,LOSC_CTRL_REG);

	//set ddhhmmss
	reg=0;
	reg=(week_day<<29)|(hh<<16)|(mm<<8)|(ss);
	writel(reg,RTC_HH_MM_SS_REG);
	
	//lock
	reg=readl(LOSC_CTRL_REG);
	reg|=(1<<8);
	writel(reg,LOSC_CTRL_REG);

}

void get_ddhhmmss(u8 *week_day , u8 *hh, u8 *mm, u8 *ss)
{
	//unlock
	u32 reg;
	//set ddhhmmss

	reg=readl(RTC_HH_MM_SS_REG);
	*week_day=(reg>>29)&0x07;
	*hh=(reg>>16)&0x1f;
	*mm=(reg>>8)&0x3f;
	*ss=reg&0x3f;

}



void set_yymmdd(u32 yy,u32 mm,u32 dd)
{
	//unlock
	u32 reg=readl(LOSC_CTRL_REG);
	reg&=~(1<<7);
	writel(reg,LOSC_CTRL_REG);

	//set yymmdd
	u32 leap_year=0;
	if(((yy%4)==0)&&((yy%100)!=0))
		leap_year=1;
	else
		leap_year=0;
	
	reg=0;
	
	reg=(leap_year<<22)|((yy-1970)<<16)|(mm<<8)|(dd);

	writel(reg,RTC_YY_MM_DD_REG);
	
	//lock
	reg=readl(LOSC_CTRL_REG);
	reg|=(1<<7);
	writel(reg,LOSC_CTRL_REG);

}

void get_yymmdd(u32 *yy,u8 *mm,u8 *dd)
{
	u32 reg=readl(RTC_YY_MM_DD_REG);
	//printf("%x\n",reg);
	*yy=((reg>>16)&0x3f)+1970;
	//printf("%x\n",*yy);
	*mm=(reg>>8)&0xf;
	*dd=reg&0x1f;
}


void rtc_init()
{
	u32 val=readl(GP_DATA_REG_ADDR(0));
	
	if(val==0xaabbccdd)
		{
		printf("rtc no need init\n");
		}
	else
		{
		writel(0xaabbccdd,GP_DATA_REG_ADDR(0));
		rtc_losc_src(1);
		set_yymmdd(2017,9,23);
		set_ddhhmmss(SATURDAY,00,51,59);
	}

	//register alarm0 interrupt
	eGon2_InsINT_Func(AW_IRQ_RALARM0,(int*)alarm0_isr_handler,0);
	eGon2_EnableInt(AW_IRQ_RALARM0);

	//register alarm1 interrupt
	eGon2_InsINT_Func(AW_IRQ_RALARM1,(int*)alarm1_isr_handler,0);
	eGon2_EnableInt(AW_IRQ_RALARM1);
}



void rtc_show()
{
		u32 val=0;

	u32 year=0;
	u8 mon,day,week,hour,min,sec;

	get_yymmdd(&year,&mon,&day);
	get_ddhhmmss(&week,&hour,&min,&sec);
	
	printf("%04d-%02d-%02d %02d:%02d:%02d W%01d",year,mon,day,hour,min,sec,week+1);
	//printf("\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b\b");

	val=readl(ALARM0_CUR_VAL_REG);
	printf("alarm0 val %d\n",val);

	val=readl(ALARM0_COUNTER_REG);
	printf("alarm0 cnt %d\n",val);
	
}

void rtc_test()
{

	rtc_init();
	set_alarm0(10);
	enable_alarm0();

//	set_alarm1(00,59,00);
//	enable_alarm1_weekday(SATURDAY);
//	enable_alarm1_irq();
//	enable_alarm_wakeup();


	
}
